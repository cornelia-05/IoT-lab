#pragma once

#include "config.h"

// Fiecare instanta pastreaza istoricul unui singur senzor.
class SignalFilter {
public:
    void reset();
    void process(int input, int minimum, int maximum,
                 int &saturatedValue, int &medianValue, int &averageValue);

private:
    int medianBuffer[Config::MEDIAN_SIZE] = {};
    int averageBuffer[Config::AVERAGE_SIZE] = {};
    bool initialized = false;

    void initialize(int value);
    int median(int value);
    int average(int value);
};
