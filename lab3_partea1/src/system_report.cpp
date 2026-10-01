#include "system_report.h"

#include <stdio.h>
#include "config.h"

namespace {
void printValidMeasurement(const SensorSignals &snapshot) {
    printf("Stare: masurare valida\n");
    printf("Semnal ECHO: %lu us\n", snapshot.echoDurationUs);
    // Separăm partea întreagă și zecimala distanței în centimetri.
    printf("Distanta: %u.%u cm\n",
           static_cast<unsigned int>(snapshot.distanceMm / 10),
           static_cast<unsigned int>(snapshot.distanceMm % 10));
    printf("Alerta: %s\n", snapshot.alert ? "ACTIVA" : "INACTIVA");
    printf("LED: %s\n", snapshot.alert ? "ON" : "OFF");
    if (snapshot.alert) {
        printf("ATENTIE: obiect la mai putin de 10 cm!\n");
    }
}

void printMeasurementState(const SensorSignals &snapshot) {
    // Lipsa primei măsurări diferă de un ecou care nu a sosit.
    if (snapshot.measurementCount == 0) {
        printf("Stare: asteptare prima masurare\n");
    } else if (!snapshot.valid) {
        printf("Stare: fara ecou / timeout\n");
        printf("Distanta: indisponibila\n");
        printf("LED: OFF\n");
    } else {
        printValidMeasurement(snapshot);
    }
}
}

namespace SystemReport {
void printStartup() {
    printf("\nLAB 3.1 - ACHIZITIE SEMNAL\n");
    printf("Senzor: HC-SR04\n");
    printf("TRIG: D%u | ECHO: D%u | LED: D%u\n",
           static_cast<unsigned int>(Config::TRIG_PIN),
           static_cast<unsigned int>(Config::ECHO_PIN),
           static_cast<unsigned int>(Config::LED_PIN));
    printf("Achizitie: %u ms | Raport: %u ms\n",
           static_cast<unsigned int>(Config::ACQUISITION_PERIOD_MS),
           static_cast<unsigned int>(Config::REPORT_PERIOD_MS));
}

void print(const SensorSignals &snapshot) {
    printf("\n========== RAPORT SISTEM ==========\n");
    printf("Masurare: %lu\n", snapshot.measurementCount);
    printMeasurementState(snapshot);
    printf("===================================\n");
}
}
