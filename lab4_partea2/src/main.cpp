#include <Arduino.h>

#include "keypad_input.h"
#include "lcd_display.h"
#include "motor_control.h"
#include "motor_driver.h"
#include "reporting.h"
#include "serial_input.h"
#include "stdio_redirect.h"

void setup() {
    Serial.begin(9600);
    initStdio();
    initMotorDriver();
    initLcd();
    initKeypad();

    stopMotor();
    displayMotorState();

    printf("\nSistem initializat!\n");
    printf("Motor initial: STOP\n");
    showMenu();
    reportChange();
    printf("\nIntrodu o comanda: ");
}

void loop() {
    readSerialCommand();
    readKeypad();
    periodicReport();
}
