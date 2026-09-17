#include "status_leds.h"

#include <Arduino.h>

namespace
{
const int GREEN_LED = 50;
const int RED_LED = 52;
}

namespace StatusLeds
{
void initialize()
{
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    reset();
}

void reset()
{
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
}

// Fiecare rezultat seteaza explicit ambele iesiri. Ordinea scrierilor
// (verde, apoi rosu) si nivelurile logice sunt cele din codul initial.
void showSuccess()
{
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
}

void showFailure()
{
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
}
}
