#include <Arduino.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <stdio.h>

#include "lcd_display.h"
#include "system_state.h"

hd44780_I2Cexp lcd;
bool lcdAvailable = false;

void initLcd() {
    Wire.begin();
    int result = lcd.begin(16, 2);

    if (result != 0) {
        printf("[EROARE LCD] Cod: %d\n", result);
        lcdAvailable = false;
        return;
    }

    lcdAvailable = true;
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LABORATOR 4.2");
    lcd.setCursor(0, 1);
    lcd.print("MOTOR DC L293D");
    delay(1200);
}

void displayError(const char* message) {
    if (!lcdAvailable) return;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("EROARE!");
    lcd.setCursor(0, 1);
    lcd.print(message);
}

void displayMotorState() {
    if (!lcdAvailable) return;
    lcd.clear();
    lcd.setCursor(0, 0);

    if (motorPower > 0) lcd.print("DIR: FORWARD");
    else if (motorPower < 0) lcd.print("DIR: REVERSE");
    else lcd.print("MOTOR: STOP");

    lcd.setCursor(0, 1);
    lcd.print("POWER: ");
    lcd.print(abs(motorPower));
    lcd.print("%");
}
