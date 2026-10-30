#include <Arduino.h>
#include <Keypad.h>
#include <stdio.h>

#include "keypad_input.h"
#include "lcd_display.h"
#include "motor_control.h"
#include "reporting.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void initKeypad() {
    keypad.setDebounceTime(30);
}

void readKeypad() {
    char key = keypad.getKey();
    if (key == NO_KEY) return;
    printf("\n[KEYPAD] Tasta: %c\n", key);

    switch (key) {
        case 'A': increaseMotorPower(); reportChange(); break;
        case 'B': decreaseMotorPower(); reportChange(); break;
        case 'C': reverseMotorDirection(); printf("[OK] Directie schimbata!\n"); reportChange(); break;
        case 'D': stopMotor(); printf("[STOP] Motor oprit!\n"); reportChange(); break;
        default:
            printf("[EROARE] Tasta neutilizata: %c\n", key);
            displayError("Foloseste A-D");
            break;
    }
}
