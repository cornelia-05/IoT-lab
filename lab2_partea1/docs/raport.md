# Material pentru raport — Lucrarea de laborator nr. 2.1

**Universitatea Tehnică a Moldovei**  
Facultatea: [completează]  
Departamentul: [completează]  
Disciplina: [completează]  
Tema: Sisteme de operare secvențiale  
Student: [nume, prenume] · Grupa: [grupa]  
Verificat: [cadrul didactic]  
Chișinău, [an]

> Acest fișier este baza de conținut pentru raport. Transferă-l în șablonul
> UTM cerut de cadrul didactic; șablonul și normele exacte de formatare nu au
> fost furnizate. Fotografiile, măsurătorile și rezultatele fizice trebuie
> completate după realizarea montajului.

## 1. Scopul lucrării

Realizarea unei aplicații modulare pentru Arduino Mega 2560 care execută
secvențial trei task-uri periodice, comunică prin semnale globale conform
modelului provider/consumer și raportează starea în IDLE prin STDIO.

## 2. Noțiuni teoretice

Într-un sistem cooperativ, planificatorul apelează task-urile eligibile și
primește controlul înapoi numai când acestea se termină. Task-urile aplicației
nu se întrerup reciproc. Activitățile sunt intercalate în timp, ceea ce permite
servirea mai multor funcții pe același procesor.

Recurența stabilește intervalul nominal între activări. Offset-ul stabilește
prima activare față de momentul de pornire. Alegerea unor perioade suficient
de mari reduce numărul apelurilor inutile, iar offset-urile separă activările
și permit publicarea datelor înainte de consumarea lor.

Un provider actualizează o variabilă-semnal; un consumer citește ultima valoare
publicată. Nu există aici transmisie între procesoare sau comunicare prin rețea.

## 3. Componente și interfață hardware–software

Arduino Mega 2560, breadboard, 2 LED-uri, **2 rezistoare de 220 Ω**, 3 butoane,
cabluri jumper și cablu USB. Pinii D2/D3/D4 sunt intrări cu pull-up intern;
pinii D8/D9 sunt ieșiri pentru LED-uri. GND este comun tuturor ramurilor.
HIGH aprinde un LED; LOW la intrarea unui buton înseamnă apăsare.

Inserează [schema electrică](schema-electrica.svg), tabelul de conexiuni din
[README](../README.md) și o fotografie clară a montajului propriu.

## 4. Arhitectura aplicației

Driverele Button și Led sunt separate de logica aplicației. Scheduler gestionează
activările, AppTasks implementează cerințele funcționale, iar AppState păstrează
semnalele globale. SerialStdio conectează stdout la UART; IdleTask raportează.
Inserează diagramele arhitecturale și de execuție din README.

| Task | Perioadă | Offset | Provider / consumer |
|---|---:|---:|---|
| T1 | 10 ms | 0 ms | Provider `led1On`; citește B1 și comandă LED1 |
| T3 | 10 ms | 2 ms | Provider `holdTicks`; citește B+ și B− |
| T2 | 20 ms | 4 ms | Consumer `led1On`, `holdTicks`; provider stare LED2 |
| IDLE | raport la cel puțin 250 ms | — | Consumer al stării globale |

La activarea LED1, T1 stinge mai întâi LED2, ca interblocare imediată. În rest,
starea LED2 și numărul de recurențe sunt actualizate de T2. Toate aceste scrieri
se execută în aceeași buclă; nu există acces concurent din întreruperi.

Butoanele necesită 30 ms de nivel stabil pentru validarea unei schimbări.
Se emite un eveniment numai la tranziția în starea apăsat. Două evenimente
opuse validate simultan în T3 se anulează.

## 5. Parametrizarea clipirii

Variabila `holdTicks = N` este cuprinsă între 5 și 100, cu valoarea inițială 25.
Durata nominală a unei stări este `T_stare = N × 20 ms`; perioada completă
este `T_ciclu = 2 × N × 20 ms`. Rezultă:

| N | Aprins | Stins | Perioadă completă |
|---:|---:|---:|---:|
| 5 | 100 ms | 100 ms | 200 ms |
| 25 | 500 ms | 500 ms | 1000 ms |
| 100 | 2000 ms | 2000 ms | 4000 ms |

La schimbarea lui N, numărătoarea stării curente se reia. La reluarea clipirii
după stingerea LED1, LED2 începe în starea aprins. Dacă se ratează activări,
timpul real poate crește: implementarea numără execuții, iar `skip` semnalează
activările omise. Nu s-au măsurat încă timpii de execuție pe placa fizică.

## 6. STDIO și IDLE

`fdev_setup_stream()` configurează stdout. Funcția `printf()` din IDLE formatează
timpul, stările LED-urilor, N, contorul k și activările ratate. Caracterele sunt
transmise prin `Serial.write()`. Monitorul Serial folosește 115200 baud.
Înaintea raportului se verifică disponibilitatea bufferului TX. `delay(1)`
este apelat în bucla principală, după dispatch și IDLE.

## 7. Verificare experimentală

Compilarea pentru Mega 2560 a reușit: 5752 octeți Flash și 328 octeți RAM
statică raportată de PlatformIO; stiva de execuție nu este inclusă în RAM statică.
Testele pe calculator au trecut 194 de verificări cu GPIO și timp simulate:
anti-rebond, apăsare lungă, interblocare, clipire, limite N, evenimente simultane,
offset-uri, activări omise și overflow al ceasului. Aceste teste nu verifică
montajul, USB/UART fizic sau temporizarea reală.

Completează tabelul **după testarea pe placă**:

| Acțiune | Rezultat așteptat | Rezultat observat |
|---|---|---|
| Alimentare / reset | LED1 stins, LED2 clipește; N=25 | [completează] |
| Apăsare B1 | LED1 aprins, LED2 stins | [completează] |
| Ține B1 apăsat 2 s | O singură comutare | [completează] |
| Eliberează și apasă B1 | LED1 stins, clipirea reîncepe | [completează] |
| Apăsare B+ | N crește cu 1, clipirea încetinește | [completează] |
| Apăsare B− | N scade cu 1, clipirea accelerează | [completează] |
| Scade la limita inferioară | N rămâne 5 la apăsări suplimentare | [completează] |
| Crește la limita superioară | N rămâne 100 la apăsări suplimentare | [completează] |
| Modifică N cu LED1 aprins | LED2 rămâne stins; noua valoare este memorată | [completează] |
| Monitor Serial | Linii STDIO coerente, `skip=0` în funcționare normală | [completează] |

Anexează o captură proprie din terminal și fotografii pentru ambele moduri.
Pentru verificarea precisă a duratelor folosește un analizor logic/osciloscop;
o observație vizuală sau eșantioanele Serial la 250 ms nu măsoară fiecare front.

## 8. Avantaje, limitări și concluzii

Soluția folosește puțină memorie și are un flux de control ușor de urmărit.
Driverele sunt reutilizabile, iar comunicația între task-uri este explicită.
Limita principală este lipsa preempției: un task lent întârzie toate celelalte.
Offset-urile reduc suprapunerea activărilor, dar nu compensează blocările lungi.
Un LCD cu operații blocante ar trebui integrat în IDLE și evaluat pentru efectul
asupra planificării. Versiunea realizată utilizează raportarea Serial.

[Completează concluzia personală pe baza rezultatelor fizice obținute.]

## 9. Bibliografie

1. Enunțul lucrării de laborator nr. 2.1, furnizat de cadrul didactic.
2. Arduino, [Mega 2560](https://docs.arduino.cc/hardware/mega-2560).
3. Arduino, [InputPullupSerial](https://docs.arduino.cc/built-in-examples/digital/InputPullupSerial/).
4. PlatformIO, [Mega 2560](https://docs.platformio.org/en/latest/boards/atmelavr/megaatmega2560.html).
5. AVR-LibC, [Standard IO](https://avrdudes.github.io/avr-libc/avr-libc-user-manual/group__avr__stdio.html).
