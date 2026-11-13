#include <Arduino.h>
#include <stdio.h>
#include "fsm.h"

// Redirectarea printf catre Serial
int serial_putchar(char c, FILE *stream) {
    (void)stream;

    if (c == '\n') {
        Serial.write('\r');
    }

    Serial.write(c);
    return 0;
}

FILE serial_stdout;

void setup() {

    Serial.begin(9600);

    fdev_setup_stream(
        &serial_stdout,
        serial_putchar,
        NULL,
        _FDEV_SETUP_WRITE
    );

    stdout = &serial_stdout;

    printf("\n========================\n");
    printf("Laborator 6.1 - FSM\n");
    printf("Control Buton - LED\n");
    printf("========================\n");

    FSM_Init();
}

void loop() {

    // Actualizare continua a automatului
    FSM_Update();
}