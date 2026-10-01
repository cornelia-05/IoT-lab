#include <Arduino.h>
#include "AppTasks.h"
#include "Config.h"
#include "IdleTask.h"
#include "Scheduler.h"
#include "SerialStdio.h"

namespace {
// Providerii T1 si T3 preced consumatorul T2 in fereastra de 20 ms.
ScheduledTask tasks[] = {
    {taskButtonLed, Config::ButtonPeriodMs, 0},
    {taskAdjustDuration, Config::ButtonPeriodMs, 2},
    {taskBlinkLed, Config::BlinkPeriodMs, 4},
};
Scheduler scheduler(tasks, sizeof(tasks) / sizeof(tasks[0]));
}

void setup() {
    initializeSerialStdio();
    const uint32_t now = millis();
    initializeTasks(now);
    scheduler.begin(now);
}

void loop() {
    scheduler.dispatch();
    taskIdle(millis(), scheduler.skippedReleases());
    delay(1); // doar in IDLE; niciun task periodic nu contine delay/printf
}
