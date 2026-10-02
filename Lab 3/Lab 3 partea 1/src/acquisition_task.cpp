#include "acquisition_task.h"

#include <Arduino_FreeRTOS.h>
#include "alert_led.h"
#include "config.h"
#include "signal_store.h"
#include "ultrasonic_sensor.h"

namespace {
void acquireMeasurement() {
    SensorSignals measurement = {};
    measurement.echoDurationUs = UltrasonicSensor::readEchoDuration();
    measurement.valid = measurement.echoDurationUs != 0;
    // Un timeout lasă distanța la zero și nu activează alerta.
    if (measurement.valid) {
        measurement.distanceMm =
            UltrasonicSensor::convertToMillimeters(measurement.echoDurationUs);
    }
    measurement.alert = measurement.valid &&
                        measurement.distanceMm < Config::ALERT_DISTANCE_MM;
    AlertLed::setActive(measurement.alert);
    SignalStore::publish(measurement);
}

void run(void *parameters) {
    (void)parameters;
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {
        acquireMeasurement();
        // Următoarea execuție se raportează la momentul planificat anterior.
        vTaskDelayUntil(&lastWakeTime,
                       pdMS_TO_TICKS(Config::ACQUISITION_PERIOD_MS));
    }
}
}

namespace AcquisitionTask {
bool start() {
    return xTaskCreate(run, "Acquisition", Config::ACQUISITION_STACK_SIZE,
                       nullptr, Config::ACQUISITION_PRIORITY, nullptr) == pdPASS;
}
}
