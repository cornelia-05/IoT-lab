#include "command_processor.h"

#include "led_control.h"

namespace CommandProcessor
{
void execute(const String &command)
{
    // Comparatiile raman exacte si sensibile la litere mari/mici. LED-ul este
    // modificat inaintea mesajului de confirmare, ca in programul initial.
    if (command == "led on")
    {
        LedControl::turnOn();
        Serial.println("LED is ON");
    }
    else if (command == "led off")
    {
        LedControl::turnOff();
        Serial.println("LED is OFF");
    }
    else if (command.length() > 0)
    {
        // O comanda necunoscuta nu modifica LED-ul. Sirul gol este ignorat:
        // al doilea caracter din CRLF nu produce un mesaj de eroare.
        Serial.println("Unknown command");
    }
}
}
