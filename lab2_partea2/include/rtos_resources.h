#pragma once

#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

// Limitele cozii si ale contorului de apasari.
constexpr uint8_t QUEUE_LENGTH = 32;
constexpr uint8_t MAX_N = QUEUE_LENGTH - 1; // 1..N si markerul 0
constexpr uint8_t MAX_PENDING_PRESSES = 32;
static_assert(MAX_N < 255, "N trebuie sa incapa intr-un byte fara overflow");

// Resurse comune pentru sincronizare, comunicare si diagnosticul task-urilor.
extern SemaphoreHandle_t buttonSemaphore;
extern SemaphoreHandle_t serialMutex;
extern QueueHandle_t dataQueue;
extern volatile uint8_t pendingPresses;
extern volatile bool pressOverflow;
extern volatile bool seriesReady;
extern TaskHandle_t buttonTaskHandle;
extern TaskHandle_t syncTaskHandle;
extern TaskHandle_t asyncTaskHandle;

// Creeaza semaforul, mutexul si coada; opreste executia la eroare.
void initializeRtosResources();
