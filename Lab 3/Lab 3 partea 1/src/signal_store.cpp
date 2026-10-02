#include "signal_store.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

namespace {
SensorSignals signals = {};
SemaphoreHandle_t signalsMutex = nullptr;
}

namespace SignalStore {
bool begin() {
    signalsMutex = xSemaphoreCreateMutex();
    return signalsMutex != nullptr;
}

void publish(const SensorSignals &measurement) {
    // Înlocuim toate câmpurile împreună și păstrăm contorul total.
    xSemaphoreTake(signalsMutex, portMAX_DELAY);
    const unsigned long nextCount = signals.measurementCount + 1;
    signals = measurement;
    signals.measurementCount = nextCount;
    xSemaphoreGive(signalsMutex);
}

SensorSignals snapshot() {
    // Eliberăm mutexul înainte ca raportarea să afișeze datele.
    xSemaphoreTake(signalsMutex, portMAX_DELAY);
    const SensorSignals copy = signals;
    xSemaphoreGive(signalsMutex);
    return copy;
}
}
