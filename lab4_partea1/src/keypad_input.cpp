#include <Arduino.h>
#include <Keypad.h>
#include <stdio.h>
#include "keypad_input.h"
#include "lcd_display.h"
#include "reporting.h"
const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {{'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'}};
byte rowPins[ROWS] = {22, 23, 24, 25}; byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void initKeypad() { keypad.setDebounceTime(30); }
void readKeypad() { char key = keypad.getKey(); if (key == NO_KEY) return; printf("\n[KEYPAD] Tasta: %c\n", key); switch (key) { case 'A': activateRelay(); break; case 'B': deactivateRelay(); break; case 'C': showStatus(); break; case 'D': showMenu(); showLcd("KEYPAD A-D", "A ON B OFF C D"); break; default: displayError(); printf("[EROARE] Tasta %c neutilizata!\n", key); printf("Foloseste A, B, C sau D.\n"); break; } }
