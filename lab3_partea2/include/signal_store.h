#pragma once

#include "sensor_signals.h"

namespace SignalStore {
bool begin();
void publish(const UltrasonicSignals &measurement);
void publish(const PotentiometerSignals &measurement);
SystemSignals snapshot();
}
