#include "potentiometer_sensor.h"

#include <Arduino.h>
#include "config.h"

namespace PotentiometerSensor {
void begin() {
    pinMode(Config::POT_PIN, INPUT);
}

int readAdc() {
    return analogRead(Config::POT_PIN);
}

int convertToMillivolts(int adcValue) {
    return static_cast<int>(
        (static_cast<long>(adcValue) * Config::REFERENCE_MV) / Config::ADC_MAX);
}

int convertToAngle(int voltageMv) {
    // 0-5000 mV -> 0-270 grade, apoi centrare la -135...+135 grade.
    return static_cast<int>(
        (static_cast<long>(voltageMv) * Config::ANGLE_MAX_DEG)
        / Config::REFERENCE_MV) - Config::ANGLE_OFFSET_DEG;
}
}
