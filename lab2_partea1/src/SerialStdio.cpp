#include "SerialStdio.h"
#include "Config.h"
#include <Arduino.h>
#include <stdio.h>

namespace {
FILE serialOutput;

int serialPutChar(char character, FILE* stream) {
    (void)stream;
    if (character == '\n') Serial.write('\r');
    Serial.write(static_cast<uint8_t>(character));
    return 0;
}
}

void initializeSerialStdio() {
    Serial.begin(Config::SerialBaud);
    fdev_setup_stream(&serialOutput, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &serialOutput;
}
