#ifndef SERIAL_CONSOLE_H
#define SERIAL_CONSOLE_H

namespace SerialConsole
{
// Porneste Serial la 9600 baud si afiseaza mesajul initial si comenzile.
// Se apeleaza o singura data din setup(), dupa initializarea LED-ului.
void initialize();

// Citeste cel mult un caracter disponibil, il retransmite prin Serial (echo)
// si il adauga la comanda. La CR sau LF elimina spatiile de la capete,
// transmite comanda catre procesor si goleste sirul acumulat.
// Daca nu exista date disponibile, revine fara a modifica starea.
void update();
}

#endif
