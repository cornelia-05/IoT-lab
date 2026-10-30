#include <Arduino.h>
#include <stdio.h>

#include "config.h"
#include "lcd_display.h"
#include "reporting.h"
#include "system_state.h"

void reportMotorState() {
    const char* direction;
    if (motorPower > 0) direction = "FORWARD";
    else if (motorPower < 0) direction = "REVERSE";
    else direction = "STOP";

    int pwmValue = map(abs(motorPower), 0, 100, 0, 255);
    printf("\n------- STARE MOTOR -------\n");
    printf("Directie: %s\n", direction);
    printf("Putere setata: %d%%\n", motorPower);
    printf("PWM: %d / 255\n", pwmValue);
    printf("Directie memorata: %s\n", motorDirection == 1 ? "FORWARD" : "REVERSE");
    printf("---------------------------\n");
    displayMotorState();
}

void reportChange() {
    reportMotorState();
    lastReport = millis();
}

void showMenu() {
    printf("\n================================\n");
    printf(" LABORATOR 4.2 - MOTOR L293D\n");
    printf("================================\n");
    printf("COMENZI SERIAL:\n");
    printf(" motor set N (-100..100)\n motor stop\n motor max\n motor inc\n motor dec\n status\n help\n");
    printf("\nCOMENZI KEYPAD:\n");
    printf(" A - Creste puterea +10%%\n B - Reduce puterea -10%%\n C - Schimba directia\n D - STOP imediat\n");
    printf("================================\n");
}

void periodicReport() {
    unsigned long now = millis();
    if (now - lastReport >= REPORT_INTERVAL) {
        reportMotorState();
        lastReport = now;
    }
}
