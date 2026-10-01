#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "Arduino.h"
#include "AppState.h"
#include "AppTasks.h"
#include "Button.h"
#include "Config.h"
#include "Scheduler.h"

static uint8_t pins[70];
static uint32_t fakeNow;
static unsigned checks;
#define CHECK(expression) do { ++checks; assert(expression); } while (0)

void pinMode(uint8_t, uint8_t) {}
int digitalRead(uint8_t pin) { return pins[pin]; }
void digitalWrite(uint8_t pin, uint8_t value) {
    pins[pin] = value;
    // Verifica inclusiv ordinea scrierilor, nu numai starea finala.
    if (pin == Config::PrimaryLedPin || pin == Config::BlinkLedPin) {
        CHECK(!(pins[Config::PrimaryLedPin] && pins[Config::BlinkLedPin]));
    }
}
uint32_t millis() { return fakeNow; }

static void reset() {
    for (auto& pin : pins) pin = HIGH;
    pins[Config::PrimaryLedPin] = pins[Config::BlinkLedPin] = LOW;
    fakeNow = 0;
    initializeTasks(fakeNow);
}

static void pulse(uint8_t pin, TaskFunction task) {
    pins[pin] = LOW;
    task(fakeNow);
    fakeNow += Config::DebounceMs;
    task(fakeNow);
    pins[pin] = HIGH;
    task(++fakeNow);
    fakeNow += Config::DebounceMs;
    task(fakeNow);
    ++fakeNow;
}

static unsigned calls[3];
static void taskA(uint32_t) { ++calls[0]; }
static void taskB(uint32_t) { ++calls[1]; }
static void taskC(uint32_t) { ++calls[2]; }

int main() {
    reset();
    Button button(2);
    button.begin(0);
    pins[2] = LOW;
    CHECK(!button.pressed(1));
    pins[2] = HIGH;
    CHECK(!button.pressed(5));
    pins[2] = LOW;
    CHECK(!button.pressed(9));
    CHECK(!button.pressed(38));
    CHECK(button.pressed(39));
    CHECK(!button.pressed(500)); // mentinerea nu repeta evenimentul
    pins[2] = HIGH;
    CHECK(!button.pressed(501));
    CHECK(!button.pressed(531));
    pins[2] = LOW;
    CHECK(!button.pressed(532));
    CHECK(button.pressed(562));

    pins[2] = HIGH;
    button.begin(UINT32_MAX - 20);
    pins[2] = LOW;
    CHECK(!button.pressed(UINT32_MAX - 10));
    CHECK(button.pressed(19)); // 30 ms, traversand overflow-ul

    reset();
    taskBlinkLed(4);
    CHECK(appState.led2On);
    for (unsigned i = 1; i < 25; ++i) taskBlinkLed(4 + 20 * i);
    CHECK(appState.led2On && appState.elapsedTicks == 24);
    taskBlinkLed(504);
    CHECK(!appState.led2On && appState.elapsedTicks == 0);
    for (unsigned i = 0; i < 25; ++i) taskBlinkLed(524 + 20 * i);
    CHECK(appState.led2On);
    pulse(2, taskButtonLed);
    CHECK(appState.led1On && !appState.led2On);
    CHECK(pins[8] == HIGH && pins[9] == LOW);
    for (unsigned i = 0; i < 100; ++i) taskBlinkLed(0);
    CHECK(!appState.led2On && appState.elapsedTicks == 0);
    pulse(2, taskButtonLed);
    taskBlinkLed(0);
    CHECK(!appState.led1On && appState.led2On);

    pulse(3, taskAdjustDuration);
    CHECK(appState.holdTicks == 26);
    taskBlinkLed(0);
    CHECK(appState.elapsedTicks == 0 && appState.led2On);
    for (unsigned i = 0; i < 150; ++i) pulse(3, taskAdjustDuration);
    CHECK(appState.holdTicks == 100);
    for (unsigned i = 0; i < 150; ++i) pulse(4, taskAdjustDuration);
    CHECK(appState.holdTicks == 5);

    reset();
    pins[3] = pins[4] = LOW;
    taskAdjustDuration(0);
    taskAdjustDuration(30);
    CHECK(appState.holdTicks == 25);

    ScheduledTask tasks[] = {{taskA, 10, 0}, {taskB, 10, 2}, {taskC, 20, 4}};
    Scheduler scheduler(tasks, 3);
    fakeNow = 0;
    scheduler.begin(fakeNow);
    for (fakeNow = 0; fakeNow < 40; ++fakeNow) scheduler.dispatch();
    CHECK(calls[0] == 4 && calls[1] == 4 && calls[2] == 2);
    CHECK(scheduler.skippedReleases() == 0);
    fakeNow = 100;
    scheduler.dispatch();
    CHECK(calls[0] == 5 && calls[1] == 5 && calls[2] == 3);
    CHECK(scheduler.skippedReleases() == 13); // 6 + 5 + 2
    CHECK(tasks[0].nextRelease == 110 && tasks[1].nextRelease == 102);

    calls[0] = calls[1] = calls[2] = 0;
    const uint32_t origin = UINT32_MAX - 5;
    scheduler.begin(origin);
    for (uint32_t i = 0; i < 40; ++i) {
        fakeNow = origin + i;
        scheduler.dispatch();
    }
    CHECK(calls[0] == 4 && calls[1] == 4 && calls[2] == 2);
    CHECK(scheduler.skippedReleases() == 0);
    printf("PASS: %u verificari (GPIO simulat; fara placa fizica).\n", checks);
}
