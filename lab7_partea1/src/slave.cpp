#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <Arduino.h>
#include <Wire.h>
#include <util/atomic.h>

#include "protocol.h"
#include "ultrasonic.h"
#include "serial_stdio.h"

// Perioada de achizitie
constexpr uint16_t SENSOR_PERIOD_MS = 200;

// Coada FreeRTOS
static QueueHandle_t measurementQueue;

// Bufferul de raspuns I2C
static uint8_t responseBuffer[PACKET_SIZE];

// Comanda primita de la Master
static volatile uint8_t commandReceived = 0;

// =======================================
// CALLBACK I2C - Primire comanda
// =======================================

void receiveEvent(int count) {

    commandReceived = 0;

    if (count == 1 && Wire.available()) {

        uint8_t command = Wire.read();

        if (command == CMD_READ_DISTANCE) {
            commandReceived = 1;
        }
    }

    // Golim eventualii octeti suplimentari
    while (Wire.available()) {
        Wire.read();
    }
}

// =======================================
// CALLBACK I2C - Raspuns catre Master
// =======================================

void requestEvent() {

    if (commandReceived == 1) {

        // Trimitem ultimul pachet disponibil
        Wire.write(
            responseBuffer,
            PACKET_SIZE
        );

        commandReceived = 0;

    } else {

        // Cerere fara comanda valida
        uint8_t errorPacket[PACKET_SIZE] = {0};

        Wire.write(
            errorPacket,
            PACKET_SIZE
        );
    }
}

// =======================================
// TASK 1 - Achizitie senzor
// =======================================

void TaskSensor(void *pvParameters) {

    (void)pvParameters;

    TickType_t lastWakeTime =
        xTaskGetTickCount();

    for (;;) {

        // Masurarea distantei
        Measurement measurement =
            Ultrasonic_Read();

        // Pastram cea mai recenta valoare
        xQueueOverwrite(
            measurementQueue,
            &measurement
        );

        // Perioada de achizitie stabila
        vTaskDelayUntil(
            &lastWakeTime,
            pdMS_TO_TICKS(SENSOR_PERIOD_MS)
        );
    }
}

// =======================================
// TASK 2 - Actualizare buffer I2C
// =======================================

void TaskBuffer(void *pvParameters) {

    (void)pvParameters;

    Measurement measurement;

    uint8_t localPacket[PACKET_SIZE];

    uint8_t logCounter = 0;

    for (;;) {

        // Asteptam date noi din coada
        if (xQueueReceive(
                measurementQueue,
                &measurement,
                portMAX_DELAY
            ) == pdTRUE) {

            // Construim pachetul local
            buildPacket(
                measurement,
                localPacket
            );

            // Actualizare atomica a bufferului
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

                for (uint8_t i = 0;
                     i < PACKET_SIZE;
                     i++) {

                    responseBuffer[i] =
                        localPacket[i];
                }
            }

            // Afisare debug aproximativ
            // o data pe secunda
            logCounter++;

            if (logCounter >= 5) {

                logCounter = 0;

                if (measurement.valid) {

                    printf(
                        "SLAVE | Distanta: "
                        "%u.%u cm\n",
                        (unsigned int)
                            (measurement.distanceMm / 10),
                        (unsigned int)
                            (measurement.distanceMm % 10)
                    );

                } else {

                    printf(
                        "SLAVE | Fara ecou "
                        "sau distanta invalida\n"
                    );
                }
            }
        }
    }
}

// =======================================
// SETUP
// =======================================

void setup() {

    STDIO_Init(9600);

    printf("\n============================\n");
    printf("LAB 7.1 - I2C SLAVE\n");
    printf("HC-SR04 + FreeRTOS\n");
    printf("============================\n");

    Ultrasonic_Init();

    // Pachet initial: date invalide
    Measurement initial = {0, 0};

    buildPacket(
        initial,
        responseBuffer
    );

    // Coada cu o singura masuratoare
    measurementQueue = xQueueCreate(
        1,
        sizeof(Measurement)
    );

    if (measurementQueue == NULL) {

        printf("Eroare creare queue!\n");

        while (1) {}
    }

    // Configurarea I2C Slave
    Wire.begin(SLAVE_ADDRESS);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    // Crearea taskului pentru senzor
    BaseType_t sensorResult = xTaskCreate(
        TaskSensor,
        "Sensor",
        256,
        NULL,
        1,
        NULL
    );

    // Crearea taskului pentru buffer
    BaseType_t bufferResult = xTaskCreate(
        TaskBuffer,
        "Buffer",
        384,
        NULL,
        2,
        NULL
    );

    if (sensorResult != pdPASS ||
        bufferResult != pdPASS) {

        printf("Eroare creare taskuri!\n");

        while (1) {}
    }

    printf("Adresa I2C: 0x%02X\n",
           (unsigned int)SLAVE_ADDRESS);

    printf("Slave initializat!\n");
    printf("Asteptare cereri Master...\n");
}

void loop() {
    // FreeRTOS gestioneaza executia
}