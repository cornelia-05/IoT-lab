#include "led_control.h"

#include <Arduino.h>

namespace
{
const int LED = 13;
}

namespace LedControl
{
void initialize()
{
    // Programul initial configureaza numai directia pinului. Nu adaugam
    // digitalWrite() aici, pentru a pastra aceeasi secventa de initializare.
    pinMode(LED, OUTPUT);
}

void turnOn()
{
    digitalWrite(LED, HIGH);
}

void turnOff()
{
    digitalWrite(LED, LOW);
}
}
