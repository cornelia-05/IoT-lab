#include "traffic_fsm.h"
#include <stdio.h>

// Starea initiala
static TrafficState currentState = EST_VERDE;

// Stingerea tuturor LED-urilor
static void AllOff() {
    digitalWrite(EST_RED, LOW);
    digitalWrite(EST_YELLOW, LOW);
    digitalWrite(EST_GREEN, LOW);

    digitalWrite(NORD_RED, LOW);
    digitalWrite(NORD_YELLOW, LOW);
    digitalWrite(NORD_GREEN, LOW);
}

// Initializare
void FSM_Init() {

    pinMode(EST_RED, OUTPUT);
    pinMode(EST_YELLOW, OUTPUT);
    pinMode(EST_GREEN, OUTPUT);

    pinMode(NORD_RED, OUTPUT);
    pinMode(NORD_YELLOW, OUTPUT);
    pinMode(NORD_GREEN, OUTPUT);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    FSM_SetState(EST_VERDE);
}

// Schimbarea starii FSM
void FSM_SetState(TrafficState state) {

    currentState = state;

    AllOff();

    switch (currentState) {

        case EST_VERDE:

            digitalWrite(EST_GREEN, HIGH);
            digitalWrite(NORD_RED, HIGH);

            printf("EST: VERDE | NORD: ROSU\n");

            break;

        case EST_GALBEN:

            digitalWrite(EST_YELLOW, HIGH);
            digitalWrite(NORD_RED, HIGH);

            printf("EST: GALBEN | NORD: ROSU\n");

            break;

        case ROSU_SPRE_NORD:

            digitalWrite(EST_RED, HIGH);
            digitalWrite(NORD_RED, HIGH);

            printf("EST: ROSU | NORD: ROSU\n");

            break;

        case NORD_VERDE:

            digitalWrite(EST_RED, HIGH);
            digitalWrite(NORD_GREEN, HIGH);

            printf("EST: ROSU | NORD: VERDE\n");

            break;

        case NORD_GALBEN:

            digitalWrite(EST_RED, HIGH);
            digitalWrite(NORD_YELLOW, HIGH);

            printf("EST: ROSU | NORD: GALBEN\n");

            break;

        case ROSU_SPRE_EST:

            digitalWrite(EST_RED, HIGH);
            digitalWrite(NORD_RED, HIGH);

            printf("EST: ROSU | NORD: ROSU\n");

            break;
    }
}

// Returnarea starii curente
TrafficState FSM_GetState() {
    return currentState;
}