#include "system_report.h"

#include <stdio.h>

namespace {
void printDistance(const char *label, uint16_t distanceMm) {
    printf("%s: %u.%u cm\n", label,
           static_cast<unsigned int>(distanceMm / 10),
           static_cast<unsigned int>(distanceMm % 10));
}

void printValidUltrasonic(const UltrasonicSignals &ultrasonic) {
    printf("ECHO us: brut=%lu | sat=%d | med=%d | avg=%d\n",
           ultrasonic.rawUs, ultrasonic.saturatedUs,
           ultrasonic.medianUs, ultrasonic.averageUs);
    printDistance("Distanta bruta", ultrasonic.rawDistanceMm);
    printDistance("Distanta filtrata", ultrasonic.filteredDistanceMm);
    printf("Alerta: %s | LED: %s\n",
           ultrasonic.alert ? "ACTIVA" : "INACTIVA",
           ultrasonic.alert ? "ON" : "OFF");
    if (ultrasonic.alert) {
        printf("ATENTIE: obiect sub 10 cm!\n");
    }
}

void printUltrasonic(const UltrasonicSignals &ultrasonic) {
    printf("[ULTRASONIC] Citire: %lu\n", ultrasonic.count);
    if (ultrasonic.count == 0) {
        printf("Stare: asteptare\n");
    } else if (!ultrasonic.valid) {
        printf("Stare: fara ecou / timeout\n");
        printf("Distanta: indisponibila\n");
        printf("LED: OFF\n");
    } else {
        printValidUltrasonic(ultrasonic);
    }
}

void printPotentiometer(const PotentiometerSignals &potentiometer) {
    printf("\n[POTENTIOMETRU] Citire: %lu\n", potentiometer.count);
    if (potentiometer.count == 0) {
        printf("Stare: asteptare\n");
        return;
    }
    printf("ADC brut: %d\n", potentiometer.rawAdc);
    printf("Tensiune mV: brut=%d | sat=%d | med=%d | avg=%d\n",
           potentiometer.voltageMv, potentiometer.saturatedMv,
           potentiometer.medianMv, potentiometer.averageMv);
    printf("Unghi: brut=%d deg | filtrat=%d deg\n",
           potentiometer.rawAngleDeg, potentiometer.filteredAngleDeg);
}
}

namespace SystemReport {
void printStartup() {
    printf("\nLAB 3.2 - CONDITIONARE SEMNAL\n");
    printf("HC-SR04: TRIG D6, ECHO D7\n");
    printf("Potentiometru: A0 | LED: D9\n");
    printf("Filtre: mediana 3, mediere ponderata 4\n");
    printf("Ponderi: 50, 25, 15, 10\n");
}

void print(const SystemSignals &snapshot) {
    printf("\n========== LAB 3.2 ==========\n");
    printUltrasonic(snapshot.ultrasonic);
    printPotentiometer(snapshot.potentiometer);
    printf("============================\n");
}
}
