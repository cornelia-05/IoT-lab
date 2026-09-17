#include <Arduino.h>
#include "code_lock.h"

// Punctul de intrare Arduino deleaga initializarea si executia modulului
// aplicatiei. Configuratia componentelor este ascunsa in modulele dedicate.
void setup()
{
    CodeLock::initialize();
}

void loop()
{
    CodeLock::update();
}
