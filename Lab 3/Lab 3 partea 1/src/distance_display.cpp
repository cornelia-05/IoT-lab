#include "distance_display.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <stdio.h>
#include "config.h"

namespace {
LiquidCrystal_I2C lcd(0x27, 16, 2);
bool lcdReady = false;

bool scanI2c() {
    bool lcdFound = false;
    for (uint8_t address = 1; address < 127; ++address) {
        Wire.beginTransmission(address);
        const uint8_t status = Wire.endTransmission();
        if (Wire.getWireTimeoutFlag()) {
            printf("EROARE I2C: timeout. Verifica SDA=20, SCL=21 si alimentarea.\n");
            return false;
        }
        if (status == 0) {
            printf("I2C: dispozitiv la 0x%02X\n", static_cast<unsigned int>(address));
            lcdFound = lcdFound || address == Config::LCD_I2C_ADDRESS;
        }
    }
    return lcdFound;
}

bool checkTimeout() {
    if (!Wire.getWireTimeoutFlag()) {
        return false;
    }
    lcdReady = false;
    printf("EROARE LCD: timeout I2C. Afisarea oprita pana la RESET.\n");
    return true;
}

void printValue(const char *text) {
    lcd.setCursor(0, 1);
    uint8_t column = 0;
    while (*text && column < Config::LCD_COLUMNS) {
        lcd.write(*text++);
        if (checkTimeout()) return;
        ++column;
    }
    // Stergem caracterele ramase de la o valoare mai lunga.
    while (column++ < Config::LCD_COLUMNS) {
        lcd.write(' ');
        if (checkTimeout()) return;
    }
}
}

namespace DistanceDisplay {
void begin() {
    printf("LCD: verificare I2C, adresa configurata 0x%02X\n",
           static_cast<unsigned int>(Config::LCD_I2C_ADDRESS));
    Wire.begin();
    Wire.setWireTimeout(Config::I2C_TIMEOUT_US, true);
    if (!scanI2c()) {
        printf("LCD indisponibil. Raportarea seriala continua.\n");
        return;
    }
    lcd.init();
    if (checkTimeout()) return;
    lcdReady = true;
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Distanta obiect:");
    printValue("Asteptare...");
    if (lcdReady) printf("LCD: initializare terminata.\n");
}

void update(const SensorSignals &snapshot) {
    if (!lcdReady) return;
    if (snapshot.measurementCount == 0) {
        printValue("Asteptare...");
    } else if (!snapshot.valid) {
        printValue("Fara ecou");
    } else {
        char value[Config::LCD_COLUMNS + 1];
        snprintf(value, sizeof(value), "%u.%u cm",
                 static_cast<unsigned int>(snapshot.distanceMm / 10),
                 static_cast<unsigned int>(snapshot.distanceMm % 10));
        printValue(value);
    }
}
}
