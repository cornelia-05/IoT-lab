#include "ultrasonic.h"

// Initializarea pinilor
void Ultrasonic_Init() {

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(TRIG_PIN, LOW);
}

// Citirea distantei
Measurement Ultrasonic_Read() {

    Measurement result = {0, 0};

    // Generarea impulsului TRIG
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    // Asteptarea impulsului ECHO
    // Timeout: 30 milisecunde
    unsigned long duration = pulseIn(
        ECHO_PIN,
        HIGH,
        30000UL
    );

    // Senzorul nu a primit ecou
    if (duration == 0) {
        return result;
    }

    // Viteza sunetului: aproximativ 343 m/s
    // Distanta in mm = durata * 343 / 2000
    unsigned long distanceMm =
        (duration * 343UL + 1000UL)
        / 2000UL;

    // Domeniul aproximativ HC-SR04
    // 2 cm - 400 cm
    if (distanceMm < 20 ||
        distanceMm > 4000) {

        return result;
    }

    result.distanceMm = (uint16_t)distanceMm;
    result.valid = 1;

    return result;
}