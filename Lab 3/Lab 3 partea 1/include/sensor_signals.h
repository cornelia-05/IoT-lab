#pragma once

#include <stdint.h>

// O măsurare completă, împreună cu starea alertei.
struct SensorSignals {
    unsigned long measurementCount;
    unsigned long echoDurationUs;
    uint16_t distanceMm;
    bool valid;
    bool alert;
};
