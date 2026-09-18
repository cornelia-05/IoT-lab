#include <Arduino.h>

#include "led_control.h"
#include "serial_console.h"

// Pastram ordinea initiala: configurarea LED-ului, apoi comunicatia seriala.
void setup()
{
    LedControl::initialize();
    SerialConsole::initialize();
}

// Fiecare iteratie deleaga citirea si prelucrarea unui caracter serial.
void loop()
{
    SerialConsole::update();
}
