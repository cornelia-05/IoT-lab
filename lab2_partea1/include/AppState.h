#pragma once
#include <stdint.h>
#include "Config.h"

// Semnale provider/consumer. Acces exclusiv din bucla principala, nu din ISR.
struct AppState {
    bool led1On = false;
    bool led2On = false;
    uint16_t holdTicks = Config::InitialHoldTicks;
    uint16_t elapsedTicks = 0;
};

extern AppState appState;
