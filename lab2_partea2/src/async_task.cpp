#include "rtos_resources.h"
#include "async_task.h"
#include "serial_output.h"

// Task 3: citeste coada la 200 ms si afiseaza datele si diagnosticele prin Serial.
void TaskAsync(void *pvParameters)
{
    (void)pvParameters;

    TickType_t lastWakeTime = xTaskGetTickCount();

    uint8_t value;

    for (;;)
    {
        // Citim un singur byte la fiecare activare
        if (seriesReady && xQueueReceive(
                dataQueue,
                &value,
                0) == pdTRUE)
        {
            if (xSemaphoreTake(
                    serialMutex,
                    portMAX_DELAY) == pdTRUE)
            {
                if (value == 0)
                {
                    Serial.println();
                    Serial.println(
                        F("[TASK 3] Sfarsit serie")
                    );

                    // Minimul de stack ramas (bytes pe AVR), pentru testele pe placa.
                    Serial.print(F("[STACK liber B] T1/T2/T3: "));
                    Serial.print(uxTaskGetStackHighWaterMark(buttonTaskHandle));
                    Serial.print('/');
                    Serial.print(uxTaskGetStackHighWaterMark(syncTaskHandle));
                    Serial.print('/');
                    Serial.println(uxTaskGetStackHighWaterMark(asyncTaskHandle));
                }
                else
                {
                    Serial.print(
                        F("[TASK 3] Byte citit: ")
                    );

                    Serial.println(value);
                }

                xSemaphoreGive(serialMutex);
            }
            if (value == 0)
                seriesReady = false;
        }

        taskENTER_CRITICAL();
        const bool overflow = pressOverflow;
        pressOverflow = false;
        taskEXIT_CRITICAL();
        if (overflow)
            printMessage(F("[AVERTIZARE] 32 apasari in asteptare; exces ignorat."));

        // Perioada 200 ms
        vTaskDelayUntil(
            &lastWakeTime,
            pdMS_TO_TICKS(200)
        );
    }
}
