#pragma once

#include <stdint.h>

struct UltrasonicSignals {
    unsigned long count;
    unsigned long rawUs;
    int saturatedUs;
    int medianUs;
    int averageUs;
    uint16_t rawDistanceMm;
    uint16_t filteredDistanceMm;
    bool valid;
    bool alert;
};

struct PotentiometerSignals {
    unsigned long count;
    int rawAdc;
    int voltageMv;
    int saturatedMv;
    int medianMv;
    int averageMv;
    int rawAngleDeg;
    int filteredAngleDeg;
};

// Ambele masurari sunt copiate sub acelasi mutex.
struct SystemSignals {
    UltrasonicSignals ultrasonic;
    PotentiometerSignals potentiometer;
};
