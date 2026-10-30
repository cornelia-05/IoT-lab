#include <Arduino.h>
#include "keypad_input.h"
#include "lcd_display.h"
#include "relay.h"
#include "reporting.h"
#include "serial_input.h"
#include "stdio_redirect.h"
void setup() { Serial.begin(9600); initStdio(); initRelay(); initLcd(); initKeypad(); displayRelayState(); printf("\nSistem initializat!\n"); printf("Stare initiala: OFF\n"); showMenu(); printf("\nIntrodu o comanda: "); }
void loop() { readSerialCommand(); readKeypad(); }
