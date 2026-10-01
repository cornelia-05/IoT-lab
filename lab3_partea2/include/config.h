#pragma once

#include <Arduino.h>

namespace Config {
constexpr uint8_t TRIG_PIN = 6;
constexpr uint8_t ECHO_PIN = 7;
constexpr uint8_t LED_PIN = 9;
constexpr uint8_t POT_PIN = A0;
constexpr unsigned long SERIAL_BAUD_RATE = 115200UL;

constexpr uint16_t SENSOR_PERIOD_MS = 100;
constexpr uint16_t REPORT_PERIOD_MS = 500;
constexpr uint16_t POT_OFFSET_MS = 30;
constexpr uint16_t REPORT_OFFSET_MS = 60;
constexpr unsigned long ECHO_TIMEOUT_US = 30000UL;

// Intervalul ultrasonic este aproximativ 2-400 cm.
constexpr int ECHO_MIN_US = 116;
constexpr int ECHO_MAX_US = 23200;
constexpr int POT_MIN_MV = 1000;
constexpr int POT_MAX_MV = 4000;
constexpr long ADC_MAX = 1023;
constexpr long REFERENCE_MV = 5000;
constexpr int ANGLE_MAX_DEG = 270;
constexpr int ANGLE_OFFSET_DEG = 135;
constexpr uint16_t ALERT_DISTANCE_MM = 100;

constexpr uint8_t MEDIAN_SIZE = 3;
constexpr uint8_t AVERAGE_SIZE = 4;
// De la cea mai noua valoare la cea mai veche.
constexpr uint8_t WEIGHTS[AVERAGE_SIZE] = {50, 25, 15, 10};

constexpr uint16_t ULTRASONIC_STACK_SIZE = 320;
constexpr uint16_t POTENTIOMETER_STACK_SIZE = 320;
constexpr uint16_t REPORT_STACK_SIZE = 512;
constexpr uint8_t ULTRASONIC_PRIORITY = 3;
constexpr uint8_t POTENTIOMETER_PRIORITY = 2;
constexpr uint8_t REPORT_PRIORITY = 1;
}
