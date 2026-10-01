#include "Led.h"
#include <Arduino.h>

void Led::begin() {
    digitalWrite(pin_, LOW);
    pinMode(pin_, OUTPUT);
}

void Led::set(bool on) {
    digitalWrite(pin_, on ? HIGH : LOW);
}
