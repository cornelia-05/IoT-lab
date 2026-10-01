#include "report_task.h"

#include <Arduino_FreeRTOS.h>
#include "config.h"
#include "signal_store.h"
#include "system_report.h"

namespace {
void run(void *parameters) {
    (void)parameters;
    vTaskDelay(pdMS_TO_TICKS(Config::REPORT_OFFSET_MS));
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {
        const SystemSignals snapshot = SignalStore::snapshot();
        // Mutexul este deja eliberat inainte de afisare.
        SystemReport::print(snapshot);
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(Config::REPORT_PERIOD_MS));
    }
}
}

namespace ReportTask {
bool start() {
    return xTaskCreate(run, "Report", Config::REPORT_STACK_SIZE,
                       nullptr, Config::REPORT_PRIORITY, nullptr) == pdPASS;
}
}
