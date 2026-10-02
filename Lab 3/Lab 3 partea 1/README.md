# Lab 3.1 — Achiziție semnal cu FreeRTOS

Proiectul măsoară distanța cu HC-SR04 la fiecare 100 ms și afișează un
raport la fiecare 500 ms, cu un decalaj inițial de 50 ms. LED-ul se aprinde
pentru o distanță validă mai mică de 10 cm.

LCD-ul I2C 16×2 afișează distanța în centimetri, cu o zecimală, la fiecare
500 ms. Dacă senzorul nu primește ecou, afișează `Fara ecou`. LCD-ul folosește
adresa `0x27` și conexiunile existente: SDA la pinul 20, SCL la pinul 21.

## Organizarea codului

Declarațiile modulelor sunt în `include/`, iar implementările în `src/`.

| Fișier / modul | Rol |
| --- | --- |
| `main.cpp` | Inițializează componentele și pornește task-urile. |
| `config.h` | Conține pinii, perioadele și parametrii task-urilor. |
| `sensor_signals.h` | Definește datele unei măsurări. |
| `ultrasonic_sensor` | Trimite impulsul TRIG, citește ECHO și convertește durata în milimetri. |
| `alert_led` | Inițializează și comandă LED-ul. |
| `distance_display` | Inițializează LCD-ul I2C și afișează distanța sau lipsa ecoului. |
| `signal_store` | Păstrează ultima măsurare și protejează accesul prin mutex. |
| `serial_console` | Conectează `printf` la Serial și afișează erorile fatale. |
| `system_report` | Formatează mesajele de pornire și rapoartele. |
| `acquisition_task` | Citește senzorul, stabilește alerta și publică măsurarea. |
| `report_task` | Preia o copie a datelor și afișează raportul periodic. |

Task-ul de achiziție actualizează LED-ul și publică măsurarea prin
`SignalStore`. Task-ul de raportare copiază datele sub mutex, apoi îl
eliberează înainte de afișare. Astfel, afișarea nu ține mutexul ocupat.

Funcțiile au cel mult 20 de rânduri, incluzând semnătura, acoladele,
comentariile și rândurile goale din interior.

## Compilare și simulare

Compilare: `pio run -e megaatmega2560`.

Schema este în `diagram.json`, iar `wokwi.toml` folosește firmware-ul
generat în `.pio/build/megaatmega2560/`. Monitorul serial folosește 115200 baud.

## Placă fizică și diagnosticare

Închide monitorul serial înainte de încărcare. Pentru placa de pe COM5:
`pio run -e megaatmega2560 -t upload --upload-port COM5`, apoi
`pio device monitor --port COM5 --baud 115200`.

La pornire, programul afișează raportul inițial și scanează magistrala I2C.
Adresele dispozitivelor detectate apar în monitorul serial. Dacă adresa
LCD-ului diferă de `0x27`, actualizează `LCD_I2C_ADDRESS` din `include/config.h`
și încarcă din nou programul. Un timeout I2C sau lipsa LCD-ului nu împiedică
raportarea distanței prin Serial. După corectarea conexiunilor, apasă RESET.
