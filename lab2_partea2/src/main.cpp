#include "rtos_resources.h"
#include "hardware.h"
#include "serial_output.h"
#include "button_led_task.h"
#include "sync_task.h"
#include "async_task.h"

// Initializeaza componentele si creeaza task-urile in ordinea initiala.
void setup()
{
    initializeHardware();
    Serial.begin(9600);
    initializeRtosResources();
    printStartupMessages();

    // =================================================
    // CREARE TASK-URI
    // =================================================

    BaseType_t result1;
    BaseType_t result2;
    BaseType_t result3;

    // 3 > 2 > 1: butonul are deadline-ul cel mai scurt; consumerul e periodic.
    // StackType_t este uint8_t pe AVR: dimensiunile sunt in bytes.
    // T1 pastreaza 192 B; T2/T3 au 256 B pentru Serial si diagnostice.
    result1 = xTaskCreate(
        TaskButtonLed,
        "Button",
        192,
        NULL,
        3,
        &buttonTaskHandle
    );

    result2 = xTaskCreate(
        TaskSync,
        "Sync",
        256,
        NULL,
        2,
        &syncTaskHandle
    );

    result3 = xTaskCreate(
        TaskAsync,
        "Async",
        256,
        NULL,
        1,
        &asyncTaskHandle
    );

    // Verificam daca task-urile au fost create
    if (result1 != pdPASS ||
        result2 != pdPASS ||
        result3 != pdPASS)
    {
        Serial.println(
            F("EROARE: unul dintre task-uri nu a fost creat!")
        );

        while (1)
        {
        }
    }

    Serial.println(
        F("Toate task-urile au fost create.")
    );

    Serial.println(
        F("Sistem pornit. Apasa butonul.")
    );

    Serial.println(
        F("====================================")
    );
    Serial.flush(); // Terminam mesajele initiale inainte sa porneasca schedulerul.
}

void loop()
{
    // FreeRTOS ruleaza task-urile.
}
