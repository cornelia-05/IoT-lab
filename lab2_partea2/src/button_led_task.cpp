#include "rtos_resources.h"
#include "button_led_task.h"
#include "hardware.h"

// Task 1: citeste butonul la 10 ms, filtreaza contactele si aprinde LED1 timp de 1 s.
void TaskButtonLed(void *pvParameters)
{
    (void)pvParameters;

    TickType_t lastWakeTime = xTaskGetTickCount();

    bool lastRawPressed = false;
    bool stablePressed = false;
    TickType_t lastChangeTime = lastWakeTime;

    bool led1Active = false;
    TickType_t led1StartTime = 0;

    for (;;)
    {
        // INPUT_PULLUP:
        // LOW  = apasat
        // HIGH = liber

        const TickType_t now = xTaskGetTickCount();
        const bool pressed = (digitalRead(BUTTON_PIN) == LOW);
        if (pressed != lastRawPressed)
        {
            lastRawPressed = pressed;
            lastChangeTime = now;
        }

        // Acceptam schimbarea doar dupa 30 ms stabili, inclusiv la eliberare.
        if (pressed != stablePressed &&
            (TickType_t)(now - lastChangeTime) >= pdMS_TO_TICKS(30))
        {
            stablePressed = pressed;
            if (stablePressed)
            {
                digitalWrite(LED1_PIN, HIGH);
                led1Active = true;
                led1StartTime = now; // O noua apasare reporneste intervalul de 1 s.

                taskENTER_CRITICAL();
                if (pendingPresses < MAX_PENDING_PRESSES)
                    ++pendingPresses;
                else
                    pressOverflow = true;
                taskEXIT_CRITICAL();

                // Daca e deja dat, notificarea exista; contorul retine evenimentul.
                xSemaphoreGive(buttonSemaphore);
            }
        }

        // LED1 ramane aprins 1 secunda
        // fara blocarea Task 1
        if (led1Active)
        {
            TickType_t elapsed =
                now - led1StartTime;

            if (elapsed >= pdMS_TO_TICKS(1000))
            {
                digitalWrite(LED1_PIN, LOW);
                led1Active = false;

            }
        }

        // Fara Serial/mutex/blocare de 1 s in task-ul cu prioritatea maxima.
        // Perioada nominala de 10 ms, cu rezolutie de 1 ms.
        vTaskDelayUntil(
            &lastWakeTime,
            pdMS_TO_TICKS(10)
        );
    }
}
