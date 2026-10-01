#include "rtos_resources.h"
#include "serial_output.h"

void printMessage(const __FlashStringHelper *message)
{
    if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE)
    {
        Serial.println(message);
        xSemaphoreGive(serialMutex);
    }
}

void printTask2N(uint8_t n)
{
    if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE)
    {
        Serial.print(F("[TASK 2] Apasare procesata. N = "));
        Serial.println(n);
        xSemaphoreGive(serialMutex);
    }
}

void printStartupMessages()
{
    Serial.println();
    Serial.println(
        F("====================================")
    );

    Serial.println(
        F(" LAB 2.2 - FreeRTOS")
    );

    Serial.println(
        F("====================================")
    );

    Serial.println(
        F("D2 -> Buton (INPUT_PULLUP)")
    );

    Serial.println(
        F("D9 -> LED1")
    );

    Serial.println(
        F("D8 -> LED2")
    );

    Serial.println(
        F("------------------------------------")
    );

    Serial.println(
        F("Task 1: perioada 10 ms")
    );

    Serial.println(
        F("Task 2: Semaphore + Queue")
    );

    Serial.println(
        F("Task 3: perioada 200 ms")
    );

    Serial.println(
        F("Mutex: protectie Serial")
    );

    Serial.println(F("Timer1: tick 1 ms; debounce: 30 ms"));
    Serial.println(F("Queue: 32 bytes; N saturat la 31"));
    Serial.println(F("Cel mult 32 apasari in asteptare"));

    Serial.println(
        F("====================================")
    );
}
