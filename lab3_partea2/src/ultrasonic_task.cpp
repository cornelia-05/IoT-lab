#include "ultrasonic_task.h"

#include <Arduino_FreeRTOS.h>
#include "alert_led.h"
#include "config.h"
#include "signal_filter.h"
#include "signal_store.h"
#include "ultrasonic_sensor.h"

namespace {
SignalFilter filter;

void conditionMeasurement(UltrasonicSignals &measurement) {
    filter.process(static_cast<int>(measurement.rawUs),
                   Config::ECHO_MIN_US, Config::ECHO_MAX_US,
                   measurement.saturatedUs, measurement.medianUs,
                   measurement.averageUs);
    measurement.rawDistanceMm =
        UltrasonicSensor::convertToMillimeters(measurement.rawUs);
    measurement.filteredDistanceMm =
        UltrasonicSensor::convertToMillimeters(measurement.averageUs);
    measurement.alert =
        measurement.filteredDistanceMm < Config::ALERT_DISTANCE_MM;
}

void acquireMeasurement(unsigned long count) {
    UltrasonicSignals measurement = {};
    measurement.count = count;
    measurement.rawUs = UltrasonicSensor::readEchoDuration();
    measurement.valid = measurement.rawUs != 0;
    if (measurement.valid) {
        conditionMeasurement(measurement);
    } else {
        // Timeout-ul nu intra in filtre; urmatorul ecou reface istoricul.
        filter.reset();
    }
    AlertLed::setActive(measurement.alert);
    SignalStore::publish(measurement);
}

void run(void *parameters) {
    (void)parameters;
    TickType_t lastWakeTime = xTaskGetTickCount();
    unsigned long count = 0;
    for (;;) {
        acquireMeasurement(++count);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(Config::SENSOR_PERIOD_MS));
    }
}
}

namespace UltrasonicTask {
bool start() {
    return xTaskCreate(run, "Ultrasonic", Config::ULTRASONIC_STACK_SIZE,
                       nullptr, Config::ULTRASONIC_PRIORITY, nullptr) == pdPASS;
}
}
