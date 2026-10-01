#include "ultrasonic_sensor.h"

#include <Arduino.h>
#include "config.h"

namespace UltrasonicSensor {
void begin() {
    pinMode(Config::TRIG_PIN, OUTPUT);
    pinMode(Config::ECHO_PIN, INPUT);
    digitalWrite(Config::TRIG_PIN, LOW);
}

unsigned long readEchoDuration() {
    // Impulsul de 10 microsecunde pornește măsurarea.
    digitalWrite(Config::TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(Config::TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(Config::TRIG_PIN, LOW);
    // Valoarea 0 indică lipsa ecoului în timpul permis.
    return pulseInLong(Config::ECHO_PIN, HIGH, Config::ECHO_TIMEOUT_US);
}

uint16_t convertToMillimeters(unsigned long durationUs) {
    // Păstrăm milimetri pentru afișarea unei zecimale fără float.
    return static_cast<uint16_t>((durationUs * 10UL) / 58UL);
}
}
