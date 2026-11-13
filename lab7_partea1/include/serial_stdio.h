#ifndef SERIAL_STDIO_H
#define SERIAL_STDIO_H

#include <Arduino.h>
#include <stdio.h>

// Trimiterea unui caracter catre Serial
inline int serial_putchar(
    char c,
    FILE *stream
) {
    (void)stream;

    if (c == '\n') {
        Serial.write('\r');
    }

    Serial.write(c);

    return 0;
}

// Initializare STDIO
inline void STDIO_Init(
    unsigned long baudRate = 9600
) {
    static FILE serial_stdout;

    Serial.begin(baudRate);

    fdev_setup_stream(
        &serial_stdout,
        serial_putchar,
        NULL,
        _FDEV_SETUP_WRITE
    );

    stdout = &serial_stdout;
}

#endif