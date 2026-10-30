#include <Arduino.h>
#include <stdio.h>

#include "config.h"
#include "motor_control.h"
#include "motor_driver.h"
#include "system_state.h"

int clampPower(int power) {
    return constrain(power, MIN_POWER, MAX_POWER);
}

void setMotorPower(int power) {
    power = clampPower(power);
    motorPower = power;

    if (power > 0) {
        motorDirection = 1;
    }
    else if (power < 0) {
        motorDirection = -1;
    }

    writeMotorHardware(motorPower);
}

void stopMotor() {
    setMotorPower(0);
}

void maxMotor() {
    setMotorPower(motorDirection * MAX_POWER);
}

void increaseMotorPower() {
    int currentMagnitude = abs(motorPower);

    if (currentMagnitude >= MAX_POWER) {
        printf("[LIMITA] Puterea maxima a fost atinsa!\n");
        return;
    }

    int newMagnitude = min(currentMagnitude + POWER_STEP, MAX_POWER);
    setMotorPower(motorDirection * newMagnitude);
}

void decreaseMotorPower() {
    int currentMagnitude = abs(motorPower);

    if (currentMagnitude == 0) {
        printf("[LIMITA] Motorul este deja oprit!\n");
        return;
    }

    int newMagnitude = max(currentMagnitude - POWER_STEP, 0);
    setMotorPower(motorDirection * newMagnitude);
}

void reverseMotorDirection() {
    motorDirection *= -1;

    if (motorPower != 0) {
        setMotorPower(motorDirection * abs(motorPower));
    }
}
