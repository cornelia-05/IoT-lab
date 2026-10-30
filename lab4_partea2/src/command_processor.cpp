#include <Arduino.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_processor.h"
#include "config.h"
#include "lcd_display.h"
#include "motor_control.h"
#include "reporting.h"

void normalizeCommand(char* text) {
    size_t readIndex = 0;
    size_t writeIndex = 0;
    bool previousSpace = false;

    while (isspace((unsigned char)text[readIndex])) readIndex++;

    while (text[readIndex] != '\0') {
        char c = (char)tolower((unsigned char)text[readIndex++]);
        if (isspace((unsigned char)c)) {
            if (writeIndex > 0 && !previousSpace) {
                text[writeIndex++] = ' ';
                previousSpace = true;
            }
        }
        else {
            text[writeIndex++] = c;
            previousSpace = false;
        }
    }

    if (writeIndex > 0 && text[writeIndex - 1] == ' ') writeIndex--;
    text[writeIndex] = '\0';
}

void processSerialCommand(char* command) {
    normalizeCommand(command);
    if (command[0] == '\0') return;

    printf("\n[COMANDA] %s\n", command);
    char action[16] = "";
    char subcommand[16] = "";
    char valueText[16] = "";
    char extra[2] = "";
    int count = sscanf(command, "%15s %15s %15s %1s", action, subcommand, valueText, extra);

    if (count == 3 && strcmp(action, "motor") == 0 && strcmp(subcommand, "set") == 0) {
        char* endPtr;
        long value = strtol(valueText, &endPtr, 10);
        if (endPtr == valueText || *endPtr != '\0' || value < MIN_POWER || value > MAX_POWER) {
            printf("[EROARE] Puterea trebuie sa fie intre -100 si 100.\n");
            displayError("Valoare invalida");
            return;
        }
        setMotorPower((int)value);
        printf("[OK] Puterea motorului a fost setata.\n");
        reportChange();
    }
    else if (strcmp(command, "motor stop") == 0) {
        stopMotor();
        printf("[OK] Motor oprit!\n");
        reportChange();
    }
    else if (strcmp(command, "motor max") == 0) {
        maxMotor();
        printf("[OK] Putere maxima!\n");
        reportChange();
    }
    else if (strcmp(command, "motor inc") == 0) {
        increaseMotorPower();
        reportChange();
    }
    else if (strcmp(command, "motor dec") == 0) {
        decreaseMotorPower();
        reportChange();
    }
    else if (strcmp(command, "status") == 0) reportChange();
    else if (strcmp(command, "help") == 0) showMenu();
    else {
        printf("[EROARE] Comanda necunoscuta!\n");
        printf("Tasteaza help pentru comenzi.\n");
        displayError("Comanda invalida");
    }
}
