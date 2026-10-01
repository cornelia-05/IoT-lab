// LAB 2.1 - LISTARE REUNITA PENTRU RAPORT
// Planificare cooperativa: task-urile ruleaza pe rand si isi incheie rapid executia.
// Starea comuna coordoneaza trei butoane si doua LED-uri, fara FreeRTOS.
#include <Arduino.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// 1. CONFIGURATIA HARDWARE SI TEMPORIZARILE
// Butoane: D2 comuta LED1, D3 mareste durata, D4 o micsoreaza.
// LED1 este conectat la D8, iar LED2 la D9. Butoanele sunt active la LOW.
namespace Config {
constexpr uint8_t ToggleButtonPin = 2;
constexpr uint8_t IncreaseButtonPin = 3;
constexpr uint8_t DecreaseButtonPin = 4;
constexpr uint8_t PrimaryLedPin = 8;
constexpr uint8_t BlinkLedPin = 9;
constexpr uint32_t DebounceMs = 30;
constexpr uint32_t ButtonPeriodMs = 10;
constexpr uint32_t BlinkPeriodMs = 20;
// Un tick reprezinta o executie a task-ului de clipire, programata la fiecare 20 ms.
// Durata nominala a unei stari LED2 este N * 20 ms: initial 500 ms, intre 100 si 2000 ms.
// Un ciclu complet aprins-stins dureaza nominal 2 * N * 20 ms.
constexpr uint16_t InitialHoldTicks = 25;
constexpr uint16_t MinHoldTicks = 5;
constexpr uint16_t MaxHoldTicks = 100;
constexpr uint32_t ReportPeriodMs = 250;
constexpr unsigned long SerialBaud = 115200;
}

// 2. STAREA COMUNA SI INTERFETELE
// holdTicks este durata unei stari; elapsedTicks numara executarile din starea curenta.
// Task-urile ruleaza secvential, deci accesul la aceasta stare nu necesita un mutex.
struct AppState {
    bool led1On = false;
    bool led2On = false;
    uint16_t holdTicks = Config::InitialHoldTicks;
    uint16_t elapsedTicks = 0;
};
extern AppState appState;
// Button memoreaza starea bruta si cea stabila pentru filtrarea contactelor mecanice.
// pressed() semnaleaza o singura data fiecare tranzitie validata spre starea apasat.
class Button {
public:
    explicit Button(uint8_t pin) : pin_(pin) {}
    void begin(uint32_t now);
    bool pressed(uint32_t now);
private:
    uint8_t pin_;
    bool lastRaw_ = false;
    bool stable_ = false;
    uint32_t changedAt_ = 0;
};
// Led izoleaza configurarea pinului si conversia starii logice in nivel HIGH/LOW.
class Led {
public:
    explicit Led(uint8_t pin) : pin_(pin) {}
    void begin();
    void set(bool on);
private:
    uint8_t pin_;
};
// Fiecare task primeste momentul executiei, exprimat in milisecunde.
// periodMs stabileste perioada, offsetMs decalajul initial, nextRelease urmatoarea activare.
using TaskFunction = void (*)(uint32_t now);
struct ScheduledTask {
    TaskFunction run;
    uint32_t periodMs;
    uint32_t offsetMs;
    uint32_t nextRelease = 0;
    ScheduledTask(TaskFunction function, uint32_t period, uint32_t offset)
        : run(function), periodMs(period), offsetMs(offset) {}
};
// Planificator cooperativ: parcurge tabelul si executa task-urile ajunse la termen.
// Nu intrerupe un task in executie; ordinea din tabel determina ordinea verificarilor.
class Scheduler {
public:
    Scheduler(ScheduledTask* tasks, size_t count) : tasks_(tasks), count_(count) {}
    void begin(uint32_t now);
    void dispatch();
    uint32_t skippedReleases() const { return skippedReleases_; }
private:
    ScheduledTask* tasks_;
    size_t count_;
    uint32_t skippedReleases_ = 0;
};
void initializeTasks(uint32_t now);
void taskButtonLed(uint32_t now);
void taskAdjustDuration(uint32_t now);
void taskBlinkLed(uint32_t now);
void taskIdle(uint32_t now, uint32_t skippedReleases);
void initializeSerialStdio();
AppState appState;

// 3. ACCESUL LA BUTOANE SI LED-URI
// INPUT_PULLUP mentine intrarea la HIGH; apasarea conecteaza pinul la GND.
// Preluarea starii initiale evita generarea unei apasari artificiale la pornire.
void Button::begin(uint32_t now) {
    pinMode(pin_, INPUT_PULLUP);
    lastRaw_ = stable_ = digitalRead(pin_) == LOW;
    changedAt_ = now;
}
// Debounce: orice schimbare bruta reporneste intervalul de stabilizare de 30 ms.
bool Button::pressed(uint32_t now) {
    const bool raw = digitalRead(pin_) == LOW;
    if (raw != lastRaw_) {
        lastRaw_ = raw;
        changedAt_ = now;
    }
    // Acceptam tranzitia numai dupa stabilizare. Eliberarea actualizeaza starea,
    // dar nu genereaza eveniment; mentinerea butonului apasat nu produce repetari.
    if (raw != stable_ && uint32_t(now - changedAt_) >= Config::DebounceMs) {
        stable_ = raw;
        return stable_;
    }
    return false;
}
// Pregatim nivelul LOW inainte de activarea iesirii, pentru a porni LED-ul stins.
void Led::begin() {
    digitalWrite(pin_, LOW);
    pinMode(pin_, OUTPUT);
}
void Led::set(bool on) {
    digitalWrite(pin_, on ? HIGH : LOW);
}

// 4. PLANIFICAREA PERIODICA
// Toate momentele initiale sunt raportate la aceeasi origine de timp.
void Scheduler::begin(uint32_t now) {
    skippedReleases_ = 0;
    for (size_t i = 0; i < count_; ++i) {
        tasks_[i].nextRelease = now + tasks_[i].offsetMs;
    }
}
void Scheduler::dispatch() {
    for (size_t i = 0; i < count_; ++i) {
        ScheduledTask& task = tasks_[i];
        const uint32_t now = millis();
        // Diferenta cu semn permite verificarea termenului si la revenirea millis() la zero,
        // pentru distante temporale mai mici de jumatate din domeniul de 32 de biti.
        if (int32_t(now - task.nextRelease) < 0) continue;
        // Daca executia a intarziat, numaram activarile ratate si rulam task-ul o singura data.
        // Urmatorul termen ramane pe grila initiala; nu acumulam deriva si nu rulam recuperari in rafala.
        const uint32_t skipped = (now - task.nextRelease) / task.periodMs;
        skippedReleases_ += skipped;
        task.nextRelease += (skipped + 1) * task.periodMs;
        task.run(now);
    }
}
namespace {
// Obiecte hardware si memorie interna pentru detectarea activarii sau schimbarii duratei.
Button toggleButton(Config::ToggleButtonPin);
Button increaseButton(Config::IncreaseButtonPin);
Button decreaseButton(Config::DecreaseButtonPin);
Led primaryLed(Config::PrimaryLedPin);
Led blinkLed(Config::BlinkLedPin);
bool wasEnabled = false;
uint16_t previousHoldTicks = Config::InitialHoldTicks;
}

// 5. INITIALIZAREA STARII APLICATIEI
// Resetam contoarele, stingem LED-urile si initializam toate cele trei butoane.
void initializeTasks(uint32_t now) {
    appState = AppState{};
    wasEnabled = false;
    previousHoldTicks = appState.holdTicks;
    primaryLed.begin();
    blinkLed.begin();
    toggleButton.begin(now);
    increaseButton.begin(now);
    decreaseButton.begin(now);
}

// 6. TASK 1 - COMUTAREA LED1 (PERIOADA: 10 ms)
// Fiecare apasare validata pe D2 inverseaza LED1. Cand LED1 se aprinde,
// LED2 este stins imediat si contorul sau de clipire este resetat.
void taskButtonLed(uint32_t now) {
    if (!toggleButton.pressed(now)) return;
    appState.led1On = !appState.led1On;
    if (appState.led1On) {
        appState.led2On = false;
        appState.elapsedTicks = 0;
        blinkLed.set(false);
    }
    primaryLed.set(appState.led1On);
}

// 7. TASK 2 - REGLAREA DURATEI (PERIOADA: 10 ms)
// D3 creste N cu o unitate, D4 il scade; fiecare pas modifica durata nominala cu 20 ms.
void taskAdjustDuration(uint32_t now) {
    const bool increase = increaseButton.pressed(now);
    const bool decrease = decreaseButton.pressed(now);
    // Nu modificam durata daca nu exista evenimente sau daca ambele apar la aceeasi verificare.
    // Limitele 5..100 previn durate prea mici si depasirea intervalului admis.
    if (increase == decrease) return;
    if (increase && appState.holdTicks < Config::MaxHoldTicks) {
        ++appState.holdTicks;
    } else if (decrease && appState.holdTicks > Config::MinHoldTicks) {
        --appState.holdTicks;
    }
}

// 8. TASK 3 - CLIPIREA LED2 (PERIOADA: 20 ms)
// LED2 clipeste numai cat timp LED1 este stins. Se numara executarile, nu timpul real scurs;
// de aceea, activarile ratate pot prelungi durata efectiva a unei stari.
void taskBlinkLed(uint32_t now) {
    (void)now;
    const bool enabled = !appState.led1On;
    // LED1 aprins: dezactivam clipirea si pregatim o noua pornire.
    if (!enabled) {
        appState.led2On = false;
        appState.elapsedTicks = 0;
        wasEnabled = false;
    } else if (!wasEnabled) {
        // La prima executie dupa activare, LED2 incepe cu starea aprins.
        appState.led2On = true;
        appState.elapsedTicks = 0;
        wasEnabled = true;
    } else if (appState.holdTicks != previousHoldTicks) {
        // La schimbarea lui N, reluam numararea fara sa inversam starea LED2.
        appState.elapsedTicks = 0;
    } else if (++appState.elapsedTicks >= appState.holdTicks) {
        // Dupa N executari, inversam LED2 si incepem numararea starii urmatoare.
        appState.elapsedTicks = 0;
        appState.led2On = !appState.led2On;
    }
    previousHoldTicks = appState.holdTicks;
    blinkLed.set(appState.led2On);
}
namespace {
// 9. REDIRECTIONAREA IESIRII STANDARD CATRE SERIAL
// Adaptorul permite folosirea printf(); sfarsitul de linie este transmis ca CR + LF.
FILE serialOutput;
int serialPutChar(char character, FILE* stream) {
    (void)stream;
    if (character == '\n') Serial.write('\r');
    Serial.write(static_cast<uint8_t>(character));
    return 0;
}
}
// Initializam UART la 115200 baud si asociem stdout cu functia serialPutChar().
void initializeSerialStdio() {
    Serial.begin(Config::SerialBaud);
    fdev_setup_stream(&serialOutput, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &serialOutput;
}

// 10. RAPORTAREA IN TIMPUL DISPONIBIL
// Functia este apelata dupa verificarea task-urilor, nu este un task separat in tabel.
// Emite cel mult un raport la 250 ms si il amana daca bufferul Serial nu are suficient spatiu.
void taskIdle(uint32_t now, uint32_t skippedReleases) {
    static uint32_t lastReport = 0;
    if (uint32_t(now - lastReport) < Config::ReportPeriodMs) return;
    if (Serial.availableForWrite() < 60) return;
    lastReport = now;
    // t: timpul; L1/L2: starile LED-urilor; N: durata setata; k: contorul curent;
    // skip: numarul cumulat al activarilor periodice ratate de planificator.
    printf("t=%lu L1=%u L2=%u N=%u k=%u skip=%lu\n",
           static_cast<unsigned long>(now),
           static_cast<unsigned int>(appState.led1On),
           static_cast<unsigned int>(appState.led2On),
           static_cast<unsigned int>(appState.holdTicks),
           static_cast<unsigned int>(appState.elapsedTicks),
           static_cast<unsigned long>(skippedReleases));
}
namespace {
// 11. TABELUL TASK-URILOR SI BUCLELE ARDUINO
// Perioade: 10, 10 si 20 ms. Decalaje initiale: 0, 2 si 4 ms, pentru distribuirea activarilor.
ScheduledTask tasks[] = {
    {taskButtonLed, Config::ButtonPeriodMs, 0},
    {taskAdjustDuration, Config::ButtonPeriodMs, 2},
    {taskBlinkLed, Config::BlinkPeriodMs, 4},
};
Scheduler scheduler(tasks, sizeof(tasks) / sizeof(tasks[0]));
}
// Initializarea se executa o singura data, folosind aceeasi referinta temporala.
void setup() {
    initializeSerialStdio();
    const uint32_t now = millis();
    initializeTasks(now);
    scheduler.begin(now);
}
// Fiecare iteratie verifica termenele, incearca raportarea si face o pauza de 1 ms.
// delay(1) suspenda intreaga bucla; precizia depinde si de durata task-urilor si a raportarii.
void loop() {
    scheduler.dispatch();
    taskIdle(millis(), scheduler.skippedReleases());
    delay(1);
}
