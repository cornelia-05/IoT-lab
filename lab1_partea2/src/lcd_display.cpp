#include "lcd_display.h"
#include <LiquidCrystal.h>

namespace
{
// Ordinea pinilor este RS, E, D4, D5, D6, D7, identica versiunii initiale.
LiquidCrystal lcd(22, 24, 26, 28, 30, 32);
}

namespace LcdDisplay
{
void initialize()
{
    lcd.begin(16, 2);
}

void clear()
{
    lcd.clear();
}

void showPrompt()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Introdu codul:");
}

void showCorrectCode()
{
    lcd.setCursor(0, 0);
    lcd.print("Cod corect!");
}

void showIncorrectCode()
{
    lcd.setCursor(0, 0);
    lcd.print("Cod incorect!");
}

void showMaskedCode(unsigned int count)
{
    lcd.setCursor(0, 1);

    // Rescriem cate un asterisc pentru fiecare caracter introdus. Nu stergem
    // LCD-ul si nu limitam lungimea: pastram inclusiv comportamentul original
    // pentru introduceri mai lungi decat latimea fizica a afisajului.
    for (unsigned int i = 0; i < count; i++)
    {
        lcd.print("*");
    }
}
}
