#include <Arduino.h>
#include <ctype.h>
#include <stdio.h>

#include "command_processor.h"
#include "lcd_display.h"
#include "serial_input.h"

char serialBuffer[48];
size_t serialIndex = 0;
bool inputOverflow = false;

void readSerialCommand() {
    while (Serial.available() > 0) {
        char c = (char)Serial.read();
        if (c == '\r' || c == '\n') {
            if (serialIndex == 0 && !inputOverflow) continue;
            printf("\n");
            if (inputOverflow) {
                printf("[EROARE] Comanda prea lunga!\n");
                displayError("Text prea lung");
            }
            else {
                serialBuffer[serialIndex] = '\0';
                processSerialCommand(serialBuffer);
            }
            serialIndex = 0;
            inputOverflow = false;
            printf("\nIntrodu o comanda: ");
        }
        else if (c == '\b' || c == 127) {
            if (serialIndex > 0) {
                serialIndex--;
                Serial.print("\b \b");
            }
        }
        else if (isprint((unsigned char)c)) {
            Serial.write(c);
            if (!inputOverflow) {
                if (serialIndex < sizeof(serialBuffer) - 1) serialBuffer[serialIndex++] = c;
                else inputOverflow = true;
            }
        }
    }
}
