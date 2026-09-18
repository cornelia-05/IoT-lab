#ifndef LED_CONTROL_H
#define LED_CONTROL_H

namespace LedControl
{
// Configureaza pinul LED-ului ca iesire, fara a scrie un nivel logic initial.
void initialize();

// Aprinde LED-ul prin scrierea nivelului HIGH pe pinul 13.
void turnOn();

// Stinge LED-ul prin scrierea nivelului LOW pe pinul 13.
void turnOff();
}

#endif
