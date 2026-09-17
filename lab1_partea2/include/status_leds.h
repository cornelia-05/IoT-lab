#ifndef STATUS_LEDS_H
#define STATUS_LEDS_H

namespace StatusLeds
{
// Configureaza ambii pini ca iesiri si stinge ambele LED-uri.
void initialize();
// Stinge ambele LED-uri la resetarea introducerii.
void reset();
// Aprinde LED-ul verde si stinge LED-ul rosu.
void showSuccess();
// Stinge LED-ul verde si aprinde LED-ul rosu.
void showFailure();
}

#endif
