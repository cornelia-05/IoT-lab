#include "Scheduler.h"
#include <Arduino.h>

void Scheduler::begin(uint32_t now) {
    skippedReleases_ = 0;
    for (size_t i = 0; i < count_; ++i) {
        tasks_[i].nextRelease = now + tasks_[i].offsetMs;
    }
}

void Scheduler::dispatch() {
    for (size_t i = 0; i < count_; ++i) {
        ScheduledTask& task = tasks_[i];
        const uint32_t now = millis();
        // Comparatie modulo 2^32, inclusiv la overflow millis().
        // Presupune perioade si pauze de executie mai mici de 2^31 ms.
        if (int32_t(now - task.nextRelease) < 0) continue;

        const uint32_t skipped = (now - task.nextRelease) / task.periodMs;
        skippedReleases_ += skipped;
        task.nextRelease += (skipped + 1) * task.periodMs;
        task.run(now); // apel normal: task-ul ruleaza pana la return
    }
}
