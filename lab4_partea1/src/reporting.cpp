#include <stdio.h>
#include "lcd_display.h"
#include "relay.h"
#include "reporting.h"
void showMenu() { printf("\n================================\n"); printf(" LABORATOR 4.1 - CONTROL RELEU\n"); printf("================================\n"); printf("SERIAL:\n"); printf(" relay on  - Pornire\n relay off - Oprire\n status    - Stare curenta\n help      - Meniu\n"); printf("\nKEYPAD:\n"); printf(" A - Pornire releu\n B - Oprire releu\n C - Stare curenta\n D - Afisare meniu\n"); printf("================================\n"); }
void activateRelay() { setRelay(true); displayRelayState(); printf("[OK] LED APRINS\n"); }
void deactivateRelay() { setRelay(false); displayRelayState(); printf("[OK] LED STINS\n"); }
void showStatus() { displayRelayState(); printf("[STATUS] Releu: %s\n", getRelayState() ? "ON" : "OFF"); }
