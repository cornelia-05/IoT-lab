#include "hardware.h"

void initializeHardware()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}
