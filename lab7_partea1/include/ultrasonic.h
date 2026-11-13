#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include "protocol.h"

// Pinii senzorului HC-SR04
constexpr uint8_t TRIG_PIN = 7;
constexpr uint8_t ECHO_PIN = 8;

// Initializarea senzorului
void Ultrasonic_Init();

// Masurarea distantei
Measurement Ultrasonic_Read();

#endif