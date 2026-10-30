#include <Arduino.h>

#include "config.h"
#include "motor_driver.h"

void initMotorDriver() {
    digitalWrite(MOTOR_ENA, LOW);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);

    pinMode(MOTOR_ENA, OUTPUT);
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);

    analogWrite(MOTOR_ENA, 0);
}

void writeMotorHardware(int power) {
    int pwmValue = map(abs(power), 0, 100, 0, 255);

    analogWrite(MOTOR_ENA, 0);

    if (power > 0) {
        digitalWrite(MOTOR_IN1, HIGH);
        digitalWrite(MOTOR_IN2, LOW);
    }
    else if (power < 0) {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, HIGH);
    }
    else {
        digitalWrite(MOTOR_IN1, LOW);
        digitalWrite(MOTOR_IN2, LOW);
    }

    analogWrite(MOTOR_ENA, pwmValue);
}
