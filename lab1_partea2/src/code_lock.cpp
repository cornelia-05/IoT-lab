#include "code_lock.h"
#include <Arduino.h>
#include "lcd_display.h"
#include "matrix_keypad.h"
#include "status_leds.h"

namespace
{
const char correctCode[] = "1234";
String enteredCode = "";
}

namespace CodeLock
{
void initialize()
{
    // Pastram ordinea initializarii: LED-uri, Serial, apoi LCD si mesaj.
    StatusLeds::initialize();
    Serial.begin(9600);
    LcdDisplay::initialize();
    LcdDisplay::showPrompt();
}

void update()
{
    char key = MatrixKeypad::readKey();

    if (key)
    {
        Serial.print("Tasta apasata: ");
        Serial.println(key);

        if (key == '*')
        {
            // Resetarea elimina codul, stinge LED-urile si reface invitatia.
            enteredCode = "";
            StatusLeds::reset();
            LcdDisplay::showPrompt();
        }
        else if (key == '#')
        {
            // Stergem LCD-ul inainte de comparatie, ca in versiunea initiala.
            // Comparatia se face cu intregul sir, inclusiv daca este gol.
            LcdDisplay::clear();

            if (enteredCode == correctCode)
            {
                StatusLeds::showSuccess();
                LcdDisplay::showCorrectCode();
                Serial.println("Cod corect!");
            }
            else
            {
                StatusLeds::showFailure();
                LcdDisplay::showIncorrectCode();
                Serial.println("Cod incorect!");
            }

            // Golim numai sirul. Rezultatul afisat si LED-urile raman active
            // pana la o noua verificare sau la resetarea cu tasta '*'.
            enteredCode = "";
        }
        else
        {
            // Acceptam toate celelalte taste, inclusiv A-D, fara limita noua
            // de lungime. Afisarea mascata nu modifica LED-urile sau mesajul.
            enteredCode += key;
            LcdDisplay::showMaskedCode(enteredCode.length());
        }
    }
}
}
