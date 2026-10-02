#pragma once

// Inițializează printf pe portul serial și gestionează erorile fatale.
namespace SerialConsole {
void begin();
void stopWithError(const char *message);
}
