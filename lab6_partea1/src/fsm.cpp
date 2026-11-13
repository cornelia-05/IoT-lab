#include "fsm.h"
#include <stdio.h>

// Configurare pini
#define LED_PIN 8
#define BUTTON_PIN 2

// Timp debounce (ms)
#define DEBOUNCE_DELAY 50

// Starea initiala
static FSMState currentState = STATE_OFF;

// Variabile pentru debounce
static int lastButtonReading = HIGH;
static int stableButtonState = HIGH;
static unsigned long lastDebounceTime = 0;

// Initializarea FSM
void FSM_Init() {

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(LED_PIN, LOW);

    currentState = STATE_OFF;

    lastButtonReading = digitalRead(BUTTON_PIN);
    stableButtonState = lastButtonReading;
    lastDebounceTime = millis();

    printf("FSM initializat\n");
    printf("Stare curenta: OFF\n");
}

// Actualizarea FSM
void FSM_Update() {

    int reading = digitalRead(BUTTON_PIN);

    // Detectarea schimbarii semnalului
    if (reading != lastButtonReading) {
        lastDebounceTime = millis();
    }

    // Verificarea debounce
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {

        if (reading != stableButtonState) {

            stableButtonState = reading;

            // Buton apasat
            if (stableButtonState == LOW) {

                switch (currentState) {

                    case STATE_OFF:

                        currentState = STATE_ON;
                        digitalWrite(LED_PIN, HIGH);

                        printf("Tranzitie: OFF -> ON\n");
                        printf("LED APRINS\n");

                        break;

                    case STATE_ON:

                        currentState = STATE_OFF;
                        digitalWrite(LED_PIN, LOW);

                        printf("Tranzitie: ON -> OFF\n");
                        printf("LED STINS\n");

                        break;
                }
            }
        }
    }

    lastButtonReading = reading;
}

// Returnarea starii
FSMState FSM_GetState() {
    return currentState;
}