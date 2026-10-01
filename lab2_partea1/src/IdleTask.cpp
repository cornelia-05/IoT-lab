#include "IdleTask.h"
#include "AppState.h"
#include "Config.h"
#include <Arduino.h>
#include <stdio.h>

void taskIdle(uint32_t now, uint32_t skippedReleases) {
    static uint32_t lastReport = 0;
    if (uint32_t(now - lastReport) < Config::ReportPeriodMs) return;
    // Linia are cel mult 55 octeti, inclusiv CR/LF. Asteptam spatiu in TX
    // pentru a evita asteptarea dupa transmiterea seriala in printf.
    if (Serial.availableForWrite() < 60) return;
    lastReport = now;
    printf("t=%lu L1=%u L2=%u N=%u k=%u skip=%lu\n",
           static_cast<unsigned long>(now),
           static_cast<unsigned int>(appState.led1On),
           static_cast<unsigned int>(appState.led2On),
           static_cast<unsigned int>(appState.holdTicks),
           static_cast<unsigned int>(appState.elapsedTicks),
           static_cast<unsigned long>(skippedReleases));
}
