#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <Wire.h>

#include "protocol.h"
#include "serial_stdio.h"

// Perioada de interogare
constexpr uint16_t MASTER_PERIOD_MS = 1000;

// Prag alerta: 150 mm = 15 cm
constexpr uint16_t ALERT_DISTANCE_MM = 150;

// LED integrat Arduino
constexpr uint8_t ALERT_LED = LED_BUILTIN;

// =======================================
// Citirea datelor prin I2C
// =======================================

bool readSlavePacket(uint8_t *packet) {

    // Trimitem comanda catre Slave
    Wire.beginTransmission(SLAVE_ADDRESS);

    Wire.write(CMD_READ_DISTANCE);

    uint8_t error = Wire.endTransmission();

    if (error != 0) {

        printf(
            "Eroare I2C: cod %u\n",
            (unsigned int)error
        );

        return false;
    }

    // Solicitam pachetul de 6 octeti
    uint8_t received = Wire.requestFrom(
        (uint8_t)SLAVE_ADDRESS,
        (uint8_t)PACKET_SIZE
    );

    // Verificam numarul de octeti
    if (received != PACKET_SIZE) {

        printf(
            "Pachet incomplet: %u/%u octeti\n",
            (unsigned int)received,
            (unsigned int)PACKET_SIZE
        );

        while (Wire.available()) {
            Wire.read();
        }

        return false;
    }

    // Citirea pachetului
    for (uint8_t i = 0;
         i < PACKET_SIZE;
         i++) {

        if (!Wire.available()) {
            return false;
        }

        packet[i] = Wire.read();
    }

    // Validarea HEAD, LENGTH si CHECKSUM
    if (!validatePacket(packet)) {

        printf("Pachet invalid!\n");

        return false;
    }

    return true;
}

// =======================================
// Afisarea pachetului in format HEX
// =======================================

void printPacket(const uint8_t *packet) {

    printf("RX: ");

    for (uint8_t i = 0;
         i < PACKET_SIZE;
         i++) {

        printf(
            "%02X ",
            (unsigned int)packet[i]
        );
    }

    printf("\n");
}

// =======================================
// TASK MASTER - Interogare periodica
// =======================================

void TaskMaster(void *pvParameters) {

    (void)pvParameters;

    uint8_t packet[PACKET_SIZE];

    TickType_t lastWakeTime =
        xTaskGetTickCount();

    for (;;) {

        printf("\nInterogare Slave 0x08...\n");

        // Citire si verificare pachet
        if (readSlavePacket(packet)) {

            printPacket(packet);

            printf("Checksum: OK\n");

            // Verificam validitatea masuratorii
            if (packet[2] == 1) {

                uint16_t distanceMm =
                    decodeDistance(packet);

                printf(
                    "Distanta: %u.%u cm\n",
                    (unsigned int)
                        (distanceMm / 10),
                    (unsigned int)
                        (distanceMm % 10)
                );

                // Comportament aditional
                // Alerta de apropiere
                if (distanceMm <
                    ALERT_DISTANCE_MM) {

                    digitalWrite(
                        ALERT_LED,
                        HIGH
                    );

                    printf(
                        "ALERTA! Obiect "
                        "prea aproape!\n"
                    );

                } else {

                    digitalWrite(
                        ALERT_LED,
                        LOW
                    );

                    printf("Status: NORMAL\n");
                }

            } else {

                digitalWrite(
                    ALERT_LED,
                    LOW
                );

                printf(
                    "Masuratoare indisponibila "
                    "sau fara ecou.\n"
                );
            }

        } else {

            digitalWrite(
                ALERT_LED,
                LOW
            );

            printf(
                "Comunicatie esuata. "
                "Verifica magistrala I2C.\n"
            );
        }

        // Interogare periodica
        vTaskDelayUntil(
            &lastWakeTime,
            pdMS_TO_TICKS(MASTER_PERIOD_MS)
        );
    }
}

// =======================================
// SETUP
// =======================================

void setup() {

    STDIO_Init(9600);

    printf("\n============================\n");
    printf("LAB 7.1 - I2C MASTER\n");
    printf("Comunicatie + FreeRTOS\n");
    printf("============================\n");

    pinMode(ALERT_LED, OUTPUT);

    digitalWrite(ALERT_LED, LOW);

    // Initializare I2C Master
    Wire.begin();

    // Frecventa magistralei: 100 kHz
    Wire.setClock(100000UL);

    // Timeout pentru evitarea blocarii
    // magistralei in cazul unei erori
    #if defined(WIRE_HAS_TIMEOUT)

        Wire.setWireTimeout(
            25000UL,
            true
        );

    #endif

    // Creare task de comunicatie
    BaseType_t result = xTaskCreate(
        TaskMaster,
        "Master",
        512,
        NULL,
        1,
        NULL
    );

    if (result != pdPASS) {

        printf("Eroare creare TaskMaster!\n");

        while (1) {}
    }

    printf("Master initializat!\n");

    printf(
        "Interogare periodica: 1000 ms\n"
    );

    printf("Prag alerta: 15 cm\n");
}

void loop() {
    // FreeRTOS gestioneaza executia
}