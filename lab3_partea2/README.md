# Lab 3.2 — Condiționare semnal cu FreeRTOS

Proiectul citește HC-SR04 și potențiometrul la fiecare 100 ms. Semnalele trec
prin saturare, filtru median de 3 valori și mediere ponderată de 4 valori,
cu ponderile 50, 25, 15, 10, de la cea mai nouă la cea mai veche valoare.
LED-ul se aprinde când distanța filtrată validă este mai mică de 10 cm.

## Organizarea codului

Declarațiile modulelor sunt în `include/`, iar implementările în `src/`.

| Fișier / modul | Rol |
| --- | --- |
| `main.cpp` | Inițializează componentele și pornește cele trei task-uri. |
| `config.h` | Pini, perioade, limite, ponderi, stive și priorități. |
| `sensor_signals.h` | Datele senzorilor și structura raportului comun. |
| `ultrasonic_sensor` | Citește impulsul ECHO și convertește durata în milimetri. |
| `potentiometer_sensor` | Citește ADC și convertește în milivolți și unghi. |
| `signal_filter` | Saturare, mediană și mediere ponderată, cu istoric per instanță. |
| `alert_led` | Inițializează și comandă LED-ul. |
| `signal_store` | Publică măsurările și copiază ambele semnale sub același mutex. |
| `serial_console` | Conectează `printf` la Serial și gestionează erorile fatale. |
| `system_report` | Formatează mesajele de pornire și rapoartele. |
| `ultrasonic_task` | Achiziționează și filtrează ECHO, actualizează alerta și publică datele. |
| `potentiometer_task` | Achiziționează și filtrează tensiunea, calculează unghiul și publică datele. |
| `report_task` | Preia o copie coerentă a datelor și o afișează periodic. |

Fiecare task de achiziție are propria instanță `SignalFilter`. Lipsa ecoului
resetează numai filtrul ultrasonic și stinge LED-ul; timeout-ul nu intră în
istoricul filtrului. Prima valoare validă umple din nou istoricul.
Potențiometrul folosește saturarea 1000–4000 mV și conversia unghiului
din intervalul 0–5000 mV în −135…+135°.

Task-ul ultrasonic are prioritatea 3 și stiva 320, cel al potențiometrului
prioritatea 2 și stiva 320, iar raportarea prioritatea 1 și stiva 512.
Potențiometrul are un decalaj inițial de 30 ms, iar raportarea de 60 ms.
Raportul se afișează la fiecare 500 ms, după eliberarea mutexului.
Perioadele și decalajele sunt convertite în tick-uri prin `pdMS_TO_TICKS`,
ca în versiunea inițială.

Funcțiile au cel mult 20 de rânduri, incluzând semnătura, acoladele,
comentariile și rândurile goale din interior.

## Compilare și simulare

Compilare: `pio run -e megaatmega2560`.

Schema este în `diagram.json`, iar `wokwi.toml` folosește firmware-ul
generat în `.pio/build/megaatmega2560/`. Monitorul serial folosește 115200 baud.
