#pragma once
#include <stddef.h>
#include <stdint.h>

using TaskFunction = void (*)(uint32_t now);

struct ScheduledTask {
    TaskFunction run;
    uint32_t periodMs;
    uint32_t offsetMs;
    uint32_t nextRelease = 0;
    ScheduledTask(TaskFunction function, uint32_t period, uint32_t offset)
        : run(function), periodMs(period), offsetMs(offset) {}
};

class Scheduler {
public:
    Scheduler(ScheduledTask* tasks, size_t count) : tasks_(tasks), count_(count) {}
    void begin(uint32_t now);
    void dispatch();
    uint32_t skippedReleases() const { return skippedReleases_; }
private:
    ScheduledTask* tasks_;
    size_t count_;
    uint32_t skippedReleases_ = 0;
};
