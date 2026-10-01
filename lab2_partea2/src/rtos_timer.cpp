#include <Arduino_FreeRTOS.h>
#include <Arduino.h>
#include <task.h>

// Configureaza tick-ul FreeRTOS de 1 ms pe Timer1 si hook-ul de delay.
// Configuratia globala din platformio.ini se aplica si bibliotecii FreeRTOS.
// Fara ea, watchdog-ul implicit da pdMS_TO_TICKS(10) == 0.
static_assert(configTICK_RATE_HZ == 1000, "Este necesar tick-ul de 1 ms");
static_assert(pdMS_TO_TICKS(10) == 10, "Configuratie tick incorecta");
static_assert(F_CPU == 16000000UL, "Timer1 este configurat pentru Mega 16 MHz");

extern "C" void prvSetupTimerInterrupt(void)
{
    // Apelat de port.c cu intreruperile dezactivate, la pornirea schedulerului.
    // 16 MHz / 64 / (249 + 1) = 1000 Hz; ISR-ul FreeRTOS salveaza contextul.
    // Timer0 (millis/micros) si pinii D2/D9/D8 nu sunt afectati.
    // Timer1 este rezervat schedulerului: fara Servo/PWM pe D11/D12.
    TIMSK1 = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 249;
    TIFR1 = _BV(OCF1A) | _BV(OCF1B) | _BV(OCF1C) | _BV(TOV1);
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
    TIMSK1 = _BV(OCIE1A);
}

// Hook cerut de portul bibliotecii cand watchdog-ul nu este folosit.
// Task-urile de mai jos folosesc explicit serviciile FreeRTOS.
extern "C" void vPortDelay(const uint32_t ms)
{
    // TickType_t are 16 biti; impartim eventualele intervale foarte lungi.
    uint32_t remaining = ms;
    while (remaining != 0)
    {
        const TickType_t part = remaining > 60000UL ? 60000 : remaining;
        vTaskDelay(part);
        remaining -= part;
    }
}
