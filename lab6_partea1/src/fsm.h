#ifndef FSM_H
#define FSM_H

#include <Arduino.h>

// Starile automatului finit
enum FSMState {
    STATE_OFF,
    STATE_ON
};

// Initializarea automatului
void FSM_Init();

// Actualizarea automatului
void FSM_Update();

// Obtinerea starii curente
FSMState FSM_GetState();

#endif