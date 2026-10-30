#include <Arduino.h>
#include <stdio.h>

#include "stdio_redirect.h"

static FILE serialOutput;

int serialPutChar(char c, FILE* stream) {
    (void)stream;

    if (c == '\n') {
        Serial.write('\r');
    }

    Serial.write(c);
    return 0;
}

void initStdio() {
    fdev_setup_stream(&serialOutput, serialPutChar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serialOutput;
}
