#include "serial_console.h"

#include <Arduino.h>

#include "command_processor.h"

namespace
{
// Sirul pastreaza caracterele intre iteratiile loop(), pana la CR sau LF.
String command = "";
}

namespace SerialConsole
{
void initialize()
{
    Serial.begin(9600);

    Serial.println("Arduino ready.");
    Serial.println("Commands:");
    Serial.println("led on");
    Serial.println("led off");
}

void update()
{
    // Pastram citirea unui singur caracter pe iteratie, fara o bucla while
    // pentru golirea bufferului serial si fara intarzieri suplimentare.
    if (Serial.available() > 0)
    {
        char c = Serial.read();

        // Ecoul preceda orice prelucrare si include inclusiv CR si LF.
        Serial.print(c);

        if (c == '\n' || c == '\r')
        {
            // Fiecare terminator finalizeaza separat comanda. La CRLF,
            // CR executa comanda, apoi LF finalizeaza un sir gol, ignorat
            // de procesor. trim() ramane inaintea compararii comenzilor.
            command.trim();
            CommandProcessor::execute(command);
            command = "";
        }
        else
        {
            // Pastram acumularea originala, fara o limita noua de lungime
            // si fara tratare speciala pentru alte caractere (ex. backspace).
            command += c;
        }
    }
}
}
