#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <Arduino.h>
#include <stdio.h>
#include "traffic_fsm.h"

// Timpii semaforului (milisecunde)
#define YELLOW_TIME     2000
#define ALL_RED_TIME    1000
#define NORD_GREEN_TIME 5000
#define EST_MIN_TIME    2000

// Debounce pentru buton
#define DEBOUNCE_TIME   50

// Semafor binar FreeRTOS
SemaphoreHandle_t requestSemaphore;

// Redirectare printf catre Serial
FILE serial_stdout;

int serial_putchar(char c, FILE *stream) {

    (void)stream;

    if (c == '\n') {
        Serial.write('\r');
    }

    Serial.write(c);

    return 0;
}

// =======================================
// TASK 1 - Citirea butonului
// =======================================

void TaskButton(void *pvParameters) {

    (void)pvParameters;

    int lastReading = HIGH;
    int stableState = HIGH;

    unsigned long lastChange = millis();

    for (;;) {

        int reading = digitalRead(BUTTON_PIN);

        // Detectarea schimbarii semnalului
        if (reading != lastReading) {

            lastChange = millis();
            lastReading = reading;
        }

        // Verificarea debounce
        if (millis() - lastChange >= DEBOUNCE_TIME) {

            if (reading != stableState) {

                stableState = reading;

                // Apasare valida
                if (stableState == LOW) {

                    // Trimitem cererea catre semafor
                    xSemaphoreGive(requestSemaphore);
                }
            }
        }

        // Task-ul cedeaza procesorul
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

// =======================================
// TASK 2 - Controlul semaforului FSM
// =======================================

void TaskTraffic(void *pvParameters) {

    (void)pvParameters;

    for (;;) {

        switch (FSM_GetState()) {

            // ========================
            // EST VERDE
            // ========================

            case EST_VERDE:

                // Timp minim pentru directia EST
                vTaskDelay(pdMS_TO_TICKS(EST_MIN_TIME));

                // Asteptam cererea din NORD
                if (xSemaphoreTake(
                        requestSemaphore,
                        portMAX_DELAY
                    ) == pdTRUE) {

                    printf("\nCerere detectata din NORD!\n");

                    FSM_SetState(EST_GALBEN);
                }

                break;

            // ========================
            // EST GALBEN
            // ========================

            case EST_GALBEN:

                vTaskDelay(pdMS_TO_TICKS(YELLOW_TIME));

                FSM_SetState(ROSU_SPRE_NORD);

                break;

            // ========================
            // AMBELE ROSU
            // ========================

            case ROSU_SPRE_NORD:

                vTaskDelay(pdMS_TO_TICKS(ALL_RED_TIME));

                FSM_SetState(NORD_VERDE);

                break;

            // ========================
            // NORD VERDE
            // ========================

            case NORD_VERDE:

                vTaskDelay(pdMS_TO_TICKS(NORD_GREEN_TIME));

                FSM_SetState(NORD_GALBEN);

                break;

            // ========================
            // NORD GALBEN
            // ========================

            case NORD_GALBEN:

                vTaskDelay(pdMS_TO_TICKS(YELLOW_TIME));

                FSM_SetState(ROSU_SPRE_EST);

                break;

            // ========================
            // AMBELE ROSU
            // ========================

            case ROSU_SPRE_EST:

                vTaskDelay(pdMS_TO_TICKS(ALL_RED_TIME));

                printf("Revenire la directia prioritara EST\n\n");

                FSM_SetState(EST_VERDE);

                break;
        }
    }
}

// =======================================
// SETUP
// =======================================

void setup() {

    Serial.begin(9600);

    // Initializare STDIO
    fdev_setup_stream(
        &serial_stdout,
        serial_putchar,
        NULL,
        _FDEV_SETUP_WRITE
    );

    stdout = &serial_stdout;

    printf("\n================================\n");
    printf("Laborator 6.2 - Semafor Inteligent\n");
    printf("FSM + FreeRTOS\n");
    printf("================================\n\n");

    // Initializare automat finit
    FSM_Init();

    // Crearea semaforului binar
    requestSemaphore = xSemaphoreCreateBinary();

    if (requestSemaphore == NULL) {

        printf("Eroare creare semafor FreeRTOS!\n");

        FSM_SetState(ROSU_SPRE_NORD);

        while (1) {}
    }

    // Crearea taskului pentru buton
    BaseType_t buttonResult = xTaskCreate(
        TaskButton,
        "Button",
        256,
        NULL,
        2,
        NULL
    );

    // Crearea taskului pentru semafor
    BaseType_t trafficResult = xTaskCreate(
        TaskTraffic,
        "Traffic",
        384,
        NULL,
        1,
        NULL
    );

    // Verificarea taskurilor
    if (buttonResult != pdPASS ||
        trafficResult != pdPASS) {

        printf("Eroare creare taskuri!\n");

        FSM_SetState(ROSU_SPRE_NORD);

        while (1) {}
    }

    printf("Taskuri FreeRTOS create cu succes!\n");
    printf("Asteptare cerere NORD...\n");
}

// =======================================
// LOOP
// =======================================

void loop() {

    // FreeRTOS gestioneaza taskurile.
    // Nu avem nevoie de cod aici.
}