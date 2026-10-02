#include "alert_led.h"

#include <Arduino.h>
#include "config.h"

namespace AlertLed {
void begin() {
    pinMode(Config::LED_PIN, OUTPUT);
    setActive(false); // La pornire nu avem încă o alertă.
}

void setActive(bool active) {
    digitalWrite(Config::LED_PIN, active ? HIGH : LOW);
}
}
