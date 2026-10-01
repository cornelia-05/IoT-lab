#include "serial_console.h"

#include <Arduino.h>
#include <stdio.h>
#include "config.h"

namespace {
FILE serialOutput;

int serialPutChar(char character, FILE *stream) {
    (void)stream;
    // Completăm sfârșitul de linie pentru monitorul serial.
    if (character == '\n') {
        Serial.write('\r');
    }
    Serial.write(character);
    return 0;
}
}

namespace SerialConsole {
void begin() {
    Serial.begin(Config::SERIAL_BAUD_RATE);
    // Caracterele produse de printf ajung în Serial.
    fdev_setup_stream(&serialOutput, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &serialOutput;
}

void stopWithError(const char *message) {
    printf("%s\n", message);
    Serial.flush();
    // Oprim inițializarea dacă lipsesc resurse esențiale.
    for (;;) {}
}
}
