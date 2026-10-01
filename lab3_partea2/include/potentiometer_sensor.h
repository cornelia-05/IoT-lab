#pragma once

namespace PotentiometerSensor {
void begin();
int readAdc();
int convertToMillivolts(int adcValue);
int convertToAngle(int voltageMv);
}
