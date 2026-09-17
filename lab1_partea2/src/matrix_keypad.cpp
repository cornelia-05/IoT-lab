#include "matrix_keypad.h"

#include <Arduino.h>
#include <Keypad.h>

namespace
{
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {34, 36, 38, 40};
byte colPins[COLS] = {42, 44, 46, 48};

// Asocierea dintre randuri, coloane si caractere ramane neschimbata.
// Obiectul si tablourile au durata de viata a intregului program deoarece
// biblioteca pastreaza referinte la configuratia tastaturii.
Keypad keypad = Keypad(
    makeKeymap(keys),
    rowPins,
    colPins,
    ROWS,
    COLS
);
}

namespace MatrixKeypad
{
char readKey()
{
    // Delegam detectarea tastelor bibliotecii, fara temporizari sau filtrari
    // suplimentare care ar modifica modul de citire al programului initial.
    return keypad.getKey();
}
}
