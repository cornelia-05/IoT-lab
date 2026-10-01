#include <Arduino.h>

#include "alert_led.h"
#include "potentiometer_sensor.h"
#include "potentiometer_task.h"
#include "report_task.h"
#include "serial_console.h"
#include "signal_store.h"
#include "system_report.h"
#include "ultrasonic_sensor.h"
#include "ultrasonic_task.h"

void setup() {
    UltrasonicSensor::begin();
    AlertLed::begin();
    PotentiometerSensor::begin();
    SerialConsole::begin();
    SystemReport::printStartup();
    if (!SignalStore::begin()) {
        SerialConsole::stopWithError("EROARE: creare mutex.");
    }
    const bool ultrasonicCreated = UltrasonicTask::start();
    const bool potentiometerCreated = PotentiometerTask::start();
    const bool reportCreated = ReportTask::start();
    if (!ultrasonicCreated || !potentiometerCreated || !reportCreated) {
        SerialConsole::stopWithError("EROARE: creare task-uri.");
    }
    // Arduino_FreeRTOS porneste automat planificatorul dupa setup().
}

void loop() {
    // Achizitia, conditionarea si raportarea ruleaza in task-urile FreeRTOS.
}
