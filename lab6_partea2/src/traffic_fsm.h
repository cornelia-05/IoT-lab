#ifndef TRAFFIC_FSM_H
#define TRAFFIC_FSM_H

#include <Arduino.h>

// Pinii semaforului EST
#define EST_RED       22
#define EST_YELLOW    23
#define EST_GREEN     24

// Pinii semaforului NORD
#define NORD_RED      25
#define NORD_YELLOW   26
#define NORD_GREEN    27

// Buton cerere NORD
#define BUTTON_PIN     2

// Starile automatului finit
enum TrafficState {
    EST_VERDE,
    EST_GALBEN,
    ROSU_SPRE_NORD,
    NORD_VERDE,
    NORD_GALBEN,
    ROSU_SPRE_EST
};

// Initializarea semaforului
void FSM_Init();

// Schimbarea starii
void FSM_SetState(TrafficState state);

// Obtinerea starii curente
TrafficState FSM_GetState();

#endif