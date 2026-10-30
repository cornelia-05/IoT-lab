#pragma once

#include <Arduino.h>

constexpr uint8_t MOTOR_ENA = 5;
constexpr uint8_t MOTOR_IN1 = 6;
constexpr uint8_t MOTOR_IN2 = 7;

constexpr int MIN_POWER = -100;
constexpr int MAX_POWER = 100;
constexpr int POWER_STEP = 10;

constexpr unsigned long REPORT_INTERVAL = 3000;
