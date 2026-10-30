#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include "lcd_display.h"
#include "relay.h"
LiquidCrystal_I2C lcd27(0x27, 16, 2);
LiquidCrystal_I2C lcd3F(0x3F, 16, 2);
LiquidCrystal_I2C* lcd = nullptr;
bool i2cDeviceFound(byte address) { Wire.beginTransmission(address); return Wire.endTransmission() == 0; }
void initLcd() {
    Wire.begin(); delay(100); printf("\nScanare I2C...\n"); bool foundAny = false;
    for (byte address = 1; address < 127; address++) if (i2cDeviceFound(address)) { printf("Dispozitiv I2C: 0x%02X\n", address); foundAny = true; }
    if (!foundAny) printf("[EROARE] Niciun dispozitiv I2C!\n");
    if (i2cDeviceFound(0x27)) { lcd = &lcd27; printf("LCD selectat: 0x27\n"); }
    else if (i2cDeviceFound(0x3F)) { lcd = &lcd3F; printf("LCD selectat: 0x3F\n"); }
    else { printf("[EROARE] LCD nedetectat!\n"); return; }
    lcd->init(); lcd->backlight(); lcd->clear(); lcd->setCursor(0, 0); lcd->print("LABORATOR 4.1"); lcd->setCursor(0, 1); lcd->print("Sistem pornit"); delay(1500);
}
void showLcd(const char* line1, const char* line2) { if (lcd == nullptr) return; lcd->clear(); lcd->setCursor(0, 0); lcd->print(line1); lcd->setCursor(0, 1); lcd->print(line2); }
void displayRelayState() { if (getRelayState()) showLcd("RELAY CONTROL", "STATUS: ON"); else showLcd("RELAY CONTROL", "STATUS: OFF"); }
void displayError() { showLcd("EROARE COMANDA", "Comanda invalida"); }
