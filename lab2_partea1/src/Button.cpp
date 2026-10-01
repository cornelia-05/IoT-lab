#include "Button.h"
#include "Config.h"
#include <Arduino.h>

void Button::begin(uint32_t now) {
    pinMode(pin_, INPUT_PULLUP);
    lastRaw_ = stable_ = digitalRead(pin_) == LOW;
    changedAt_ = now;
}

bool Button::pressed(uint32_t now) {
    const bool raw = digitalRead(pin_) == LOW;
    if (raw != lastRaw_) {
        lastRaw_ = raw;
        changedAt_ = now;
    }
    if (raw != stable_ && uint32_t(now - changedAt_) >= Config::DebounceMs) {
        stable_ = raw;
        return stable_;
    }
    return false;
}
