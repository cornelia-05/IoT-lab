#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>

namespace CommandProcessor
{
// Executa o comanda din care apelantul a eliminat spatiile de la capete.
// Recunoaste exact "led on" si "led off" si raporteaza rezultatul prin Serial.
// Pentru un sir nevid necunoscut scrie "Unknown command"; ignora sirul gol.
// Nu modifica sirul primit. Comunicatia seriala trebuie sa fie initializata.
void execute(const String &command);
}

#endif
