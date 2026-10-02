#pragma once

#include "sensor_signals.h"

namespace DistanceDisplay {
void begin();
void update(const SensorSignals &snapshot);
}
