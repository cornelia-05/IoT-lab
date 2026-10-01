#include "potentiometer_task.h"

#include <Arduino_FreeRTOS.h>
#include "config.h"
#include "potentiometer_sensor.h"
#include "signal_filter.h"
#include "signal_store.h"

namespace {
SignalFilter filter;

void acquireMeasurement(unsigned long count) {
    PotentiometerSignals measurement = {};
    measurement.count = count;
    measurement.rawAdc = PotentiometerSensor::readAdc();
    measurement.voltageMv =
        PotentiometerSensor::convertToMillivolts(measurement.rawAdc);
    filter.process(measurement.voltageMv, Config::POT_MIN_MV, Config::POT_MAX_MV,
                   measurement.saturatedMv, measurement.medianMv,
                   measurement.averageMv);
    measurement.rawAngleDeg =
        PotentiometerSensor::convertToAngle(measurement.voltageMv);
    measurement.filteredAngleDeg =
        PotentiometerSensor::convertToAngle(measurement.averageMv);
    SignalStore::publish(measurement);
}

void run(void *parameters) {
    (void)parameters;
    vTaskDelay(pdMS_TO_TICKS(Config::POT_OFFSET_MS));
    TickType_t lastWakeTime = xTaskGetTickCount();
    unsigned long count = 0;
    for (;;) {
        acquireMeasurement(++count);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(Config::SENSOR_PERIOD_MS));
    }
}
}

namespace PotentiometerTask {
bool start() {
    return xTaskCreate(run, "Potentiometer", Config::POTENTIOMETER_STACK_SIZE,
                       nullptr, Config::POTENTIOMETER_PRIORITY, nullptr) == pdPASS;
}
}
