#include <Arduino.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "command_processor.h"
#include "lcd_display.h"
#include "relay.h"
#include "reporting.h"
void normalizeCommand(char* text) {
    size_t readIndex = 0, writeIndex = 0; bool previousSpace = false;
    while (text[readIndex] != '\0' && isspace((unsigned char)text[readIndex])) readIndex++;
    while (text[readIndex] != '\0') { char c = (char)tolower((unsigned char)text[readIndex++]); if (isspace((unsigned char)c)) { if (writeIndex > 0 && !previousSpace) { text[writeIndex++] = ' '; previousSpace = true; } } else { text[writeIndex++] = c; previousSpace = false; } }
    if (writeIndex > 0 && text[writeIndex - 1] == ' ') {
        writeIndex--;
    }

    text[writeIndex] = '\0';
}
void processSerialCommand(char* command) {
    normalizeCommand(command); if (command[0] == '\0') return; printf("\n[COMANDA] %s\n", command);
    char action[16] = "", argument[16] = "", extra[2] = "";
    int count = sscanf(command, "%15s %15s %1s", action, argument, extra);
    if (count == 2 && strcmp(action, "relay") == 0 && strcmp(argument, "on") == 0) activateRelay();
    else if (count == 2 && strcmp(action, "relay") == 0 && strcmp(argument, "off") == 0) deactivateRelay();
    else if (strcmp(command, "status") == 0) showStatus();
    else if (strcmp(command, "help") == 0) { showMenu(); showLcd("KEYPAD A-D", "A ON B OFF C D"); }
    else { displayError(); printf("[EROARE] Comanda invalida!\n"); printf("Utilizeaza: relay on / relay off\n"); printf("Starea releului ramane: %s\n", getRelayState() ? "ON" : "OFF"); }
}
