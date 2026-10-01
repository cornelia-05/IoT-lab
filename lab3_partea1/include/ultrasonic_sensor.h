#pragma once

#include <stdint.h>

// Accesul la senzor și conversia duratei ecoului în distanță.
namespace UltrasonicSensor {
void begin();
unsigned long readEchoDuration();
uint16_t convertToMillimeters(unsigned long durationUs);
}
