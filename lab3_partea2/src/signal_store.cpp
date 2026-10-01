#include "signal_store.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

namespace {
SystemSignals signals = {};
SemaphoreHandle_t signalsMutex = nullptr;
}

namespace SignalStore {
bool begin() {
    signalsMutex = xSemaphoreCreateMutex();
    return signalsMutex != nullptr;
}

void publish(const UltrasonicSignals &measurement) {
    xSemaphoreTake(signalsMutex, portMAX_DELAY);
    signals.ultrasonic = measurement;
    xSemaphoreGive(signalsMutex);
}

void publish(const PotentiometerSignals &measurement) {
    xSemaphoreTake(signalsMutex, portMAX_DELAY);
    signals.potentiometer = measurement;
    xSemaphoreGive(signalsMutex);
}

SystemSignals snapshot() {
    xSemaphoreTake(signalsMutex, portMAX_DELAY);
    const SystemSignals copy = signals;
    xSemaphoreGive(signalsMutex);
    return copy;
}
}
