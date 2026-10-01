#pragma once

#include <Arduino.h>

// Afiseaza mesaje din task-uri folosind mutexul Serial.
void printMessage(const __FlashStringHelper *message);
void printTask2N(uint8_t n);

// Afiseaza configuratia initiala inainte de pornirea task-urilor.
void printStartupMessages();
