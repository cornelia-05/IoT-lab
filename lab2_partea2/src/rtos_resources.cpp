#include "rtos_resources.h"

SemaphoreHandle_t buttonSemaphore;
SemaphoreHandle_t serialMutex;
QueueHandle_t dataQueue;

// Semaforul binar trezeste Task 2; contorul pastreaza apasarile cat timp e ocupat.
// Contorul si avertizarea sunt accesate numai in sectiuni critice scurte.
volatile uint8_t pendingPresses = 0;
volatile bool pressOverflow = false;

// Protocol: false = producatorul poate construi seria, true = consumerul o citeste.
// Un bool este atomic pe AVR. Devine false numai DUPA consumarea markerului 0.
volatile bool seriesReady = false;
TaskHandle_t buttonTaskHandle;
TaskHandle_t syncTaskHandle;
TaskHandle_t asyncTaskHandle;

void initializeRtosResources()
{
    buttonSemaphore = xSemaphoreCreateBinary();

    serialMutex = xSemaphoreCreateMutex();

    dataQueue = xQueueCreate(
        QUEUE_LENGTH,
        sizeof(uint8_t)
    );

    // Verificam daca resursele au fost create
    if (buttonSemaphore == NULL ||
        serialMutex == NULL ||
        dataQueue == NULL)
    {
        Serial.println(
            F("EROARE: resursele FreeRTOS nu au fost create!")
        );

        while (1)
        {
        }
    }
}
