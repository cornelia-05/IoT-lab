#include "AppTasks.h"
#include "AppState.h"
#include "Button.h"
#include "Config.h"
#include "Led.h"

namespace {
Button toggleButton(Config::ToggleButtonPin);
Button increaseButton(Config::IncreaseButtonPin);
Button decreaseButton(Config::DecreaseButtonPin);
Led primaryLed(Config::PrimaryLedPin);
Led blinkLed(Config::BlinkLedPin);
bool wasEnabled = false;
uint16_t previousHoldTicks = Config::InitialHoldTicks;
}

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

void taskButtonLed(uint32_t now) {
    if (!toggleButton.pressed(now)) return;

    appState.led1On = !appState.led1On;
    if (appState.led1On) {
        // Interblocare imediata: LED2 se stinge INAINTE de aprinderea LED1.
        // Nu asteptam urmatoarea recurenta a Task 2.
        appState.led2On = false;
        appState.elapsedTicks = 0;
        blinkLed.set(false);
    }
    primaryLed.set(appState.led1On);
}

void taskAdjustDuration(uint32_t now) {
    const bool increase = increaseButton.pressed(now);
    const bool decrease = decreaseButton.pressed(now);
    if (increase == decrease) return; // evenimente simultane: se anuleaza

    if (increase && appState.holdTicks < Config::MaxHoldTicks) {
        ++appState.holdTicks;
    } else if (decrease && appState.holdTicks > Config::MinHoldTicks) {
        --appState.holdTicks;
    }
}

void taskBlinkLed(uint32_t now) {
    (void)now;
    const bool enabled = !appState.led1On;
    if (!enabled) {
        appState.led2On = false;
        appState.elapsedTicks = 0;
        wasEnabled = false;
    } else if (!wasEnabled) {
        appState.led2On = true;
        appState.elapsedTicks = 0;
        wasEnabled = true;
    } else if (appState.holdTicks != previousHoldTicks) {
        // Noua durata incepe de la zero, pastrand starea curenta a LED2.
        appState.elapsedTicks = 0;
    } else if (++appState.elapsedTicks >= appState.holdTicks) {
        appState.elapsedTicks = 0;
        appState.led2On = !appState.led2On;
    }
    previousHoldTicks = appState.holdTicks;
    blinkLed.set(appState.led2On);
}
