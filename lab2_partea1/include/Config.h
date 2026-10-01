#pragma once
#include <stdint.h>

namespace Config {
constexpr uint8_t ToggleButtonPin = 2;
constexpr uint8_t IncreaseButtonPin = 3;
constexpr uint8_t DecreaseButtonPin = 4;
constexpr uint8_t PrimaryLedPin = 8;
constexpr uint8_t BlinkLedPin = 9;
constexpr uint32_t DebounceMs = 30;
constexpr uint32_t ButtonPeriodMs = 10;
constexpr uint32_t BlinkPeriodMs = 20;
constexpr uint16_t InitialHoldTicks = 25;
constexpr uint16_t MinHoldTicks = 5;
constexpr uint16_t MaxHoldTicks = 100;
constexpr uint32_t ReportPeriodMs = 250;
constexpr unsigned long SerialBaud = 115200;
}
