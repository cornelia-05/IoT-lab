#pragma once
#include <stdint.h>

class Led {
public:
    explicit Led(uint8_t pin) : pin_(pin) {}
    void begin();
    void set(bool on);
private:
    uint8_t pin_;
};
