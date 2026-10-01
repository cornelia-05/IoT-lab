#pragma once

#include "sensor_signals.h"

// Mesajele afișate la pornire și pentru fiecare raport periodic.
namespace SystemReport {
void printStartup();
void print(const SensorSignals &snapshot);
}
