#pragma once

#include "sensor_signals.h"

namespace SystemReport {
void printStartup();
void print(const SystemSignals &snapshot);
}
