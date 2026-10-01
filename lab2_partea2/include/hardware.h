#pragma once

#include <Arduino.h>

// Pinii butonului si ai celor doua LED-uri.
const uint8_t BUTTON_PIN = 2;
const uint8_t LED1_PIN   = 9;
const uint8_t LED2_PIN   = 8;

// Configureaza butonul cu pull-up si porneste cu LED-urile stinse.
void initializeHardware();
