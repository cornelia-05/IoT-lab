#pragma once

#include <stdint.h>

// Pinii corespund conexiunilor din schema Wokwi.
namespace Config {
constexpr uint8_t TRIG_PIN = 6;
constexpr uint8_t ECHO_PIN = 7;
constexpr uint8_t LED_PIN = 9;
constexpr unsigned long SERIAL_BAUD_RATE = 115200UL;

// Raportarea începe puțin mai târziu decât achiziția.
constexpr uint16_t ACQUISITION_PERIOD_MS = 100;
constexpr uint16_t REPORT_PERIOD_MS = 500;
constexpr uint16_t REPORT_OFFSET_MS = 50;

constexpr uint16_t ALERT_DISTANCE_MM = 100; // 10 cm
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;

// Păstrăm memoria și prioritățile alocate inițial task-urilor.
constexpr uint16_t ACQUISITION_STACK_SIZE = 256;
constexpr uint16_t REPORT_STACK_SIZE = 384;
constexpr uint8_t ACQUISITION_PRIORITY = 2;
constexpr uint8_t REPORT_PRIORITY = 1;
}
