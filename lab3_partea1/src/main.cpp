#include <Arduino.h>

#include "acquisition_task.h"
#include "alert_led.h"
#include "report_task.h"
#include "serial_console.h"
#include "signal_store.h"
#include "system_report.h"
#include "ultrasonic_sensor.h"

// Pregătim componentele înainte ca FreeRTOS să pornească task-urile.
void setup() {
    UltrasonicSensor::begin();
    AlertLed::begin();
    SerialConsole::begin();
    SystemReport::printStartup();
    if (!SignalStore::begin()) {
        SerialConsole::stopWithError("EROARE: mutexul nu a putut fi creat.");
    }
    const bool acquisitionCreated = AcquisitionTask::start();
    const bool reportCreated = ReportTask::start();
    if (!acquisitionCreated || !reportCreated) {
        SerialConsole::stopWithError("EROARE: task-urile nu au putut fi create.");
    }
    // Arduino_FreeRTOS pornește automat planificatorul după setup().
}

void loop() {
    // Achiziția și raportarea rulează în task-urile FreeRTOS.
}
