#include "rtos_resources.h"
#include "sync_task.h"
#include "hardware.h"
#include "serial_output.h"

// Task 2: proceseaza apasarile, trimite seria in coada si clipeste LED2 de N ori.
void TaskSync(void *pvParameters)
{
    (void)pvParameters;

    uint8_t N = 0;

    for (;;)
    {
        // Asteptam semaforul de la Task 1
        if (xSemaphoreTake(
                buttonSemaphore,
                portMAX_DELAY) == pdTRUE)
        {
            // Golim contorul de evenimente; semaforul ramane BINAR.
            for (;;)
            {
                taskENTER_CRITICAL();
                const bool hasPendingPress = pendingPresses != 0;
                if (hasPendingPress)
                    --pendingPresses;
                taskEXIT_CRITICAL();
                if (!hasPendingPress)
                    break;

                // Nu suprascriem o serie care inca este citita de Task 3.
                while (seriesReady)
                    vTaskDelay(pdMS_TO_TICKS(10));

                // Saturare explicita: dupa 31, fiecare apasare produce tot seria 1..31.
                if (N < MAX_N)
                    ++N;
                else
                    printMessage(F("[TASK 2] Limita N=31; repet seria 1..31."));

                printTask2N(N);

                // ==========================================
                // TRIMITERE DATE IN QUEUE
                //
                // Cerinta cere:
                // xQueueSendToFront()
                //
                // Dorim ca Task 3 sa citeasca:
                //
                // 1 2 3 ... N 0
                //
                // Deoarece folosim SendToFront,
                // introducem invers, cu consumerul oprit pana la publicarea seriei:
                //
                // 0 N ... 3 2 1
                // ==========================================

                uint8_t endMarker = 0;

                BaseType_t sent = xQueueSendToFront(
                    dataQueue,
                    &endMarker,
                    0
                );

                TickType_t lastSendTime = xTaskGetTickCount();
                for (uint8_t i = N; i >= 1 && sent == pdPASS; --i)
                {
                    // Inclusiv intre markerul 0 si N exista un interval de 50 ms.
                    vTaskDelayUntil(&lastSendTime, pdMS_TO_TICKS(50));
                    uint8_t value = i;

                    sent = xQueueSendToFront(
                        dataQueue,
                        &value,
                        0
                    );
                }

                // N+1 <= 32, coada initial goala si un singur producator:
                // nu poate deveni plina inaintea ultimei inserari.
                if (sent != pdPASS)
                {
                    xQueueReset(dataQueue); // Nu publicam niciodata o serie incompleta.
                    printMessage(F("[TASK 2] EROARE Queue; serie anulata."));
                    continue;
                }
                seriesReady = true;

                // ==========================================
                // LED2 CLIPESTE N ORI
                // ON  = 300 ms
                // OFF = 500 ms
                // ==========================================

                for (uint8_t i = 0; i < N; i++)
                {
                    digitalWrite(LED2_PIN, HIGH);

                    vTaskDelay(
                        pdMS_TO_TICKS(300)
                    );

                    digitalWrite(LED2_PIN, LOW);

                    vTaskDelay(
                        pdMS_TO_TICKS(500)
                    );
                }

                printMessage(F("[TASK 2] Secventa LED2 terminata"));
            }
        }
    }
}
