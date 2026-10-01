#pragma once
#include <stdint.h>

// INPUT_PULLUP: apasat = LOW. Un singur eveniment la fiecare apasare.
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
