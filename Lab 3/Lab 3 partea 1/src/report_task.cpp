#include "report_task.h"

#include <Arduino_FreeRTOS.h>
#include "config.h"
#include "distance_display.h"
#include "signal_store.h"
#include "system_report.h"

namespace {
void run(void *parameters) {
    (void)parameters;
    // Lăsăm achiziția să înceapă înaintea primului raport.
    vTaskDelay(pdMS_TO_TICKS(Config::REPORT_OFFSET_MS));
    TickType_t lastWakeTime = xTaskGetTickCount();
    for (;;) {
        const SensorSignals snapshot = SignalStore::snapshot();
        DistanceDisplay::update(snapshot);
        SystemReport::print(snapshot);
        vTaskDelayUntil(&lastWakeTime,
                       pdMS_TO_TICKS(Config::REPORT_PERIOD_MS));
    }
}
}

namespace ReportTask {
bool start() {
    return xTaskCreate(run, "Report", Config::REPORT_STACK_SIZE,
                       nullptr, Config::REPORT_PRIORITY, nullptr) == pdPASS;
}
}
