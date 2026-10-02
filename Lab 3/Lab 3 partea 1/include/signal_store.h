#pragma once

#include "sensor_signals.h"

// Task-urile schimbă date doar prin aceste operații protejate.
namespace SignalStore {
bool begin();
void publish(const SensorSignals &measurement);
SensorSignals snapshot();
}
