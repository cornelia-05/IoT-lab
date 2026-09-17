#ifndef CODE_LOCK_H
#define CODE_LOCK_H

namespace CodeLock
{
// Initializeaza LED-urile, comunicarea seriala la 9600 baud si LCD-ul.
// Afiseaza mesajul initial. Se apeleaza o singura data din setup().
void initialize();
// Executa o iteratie: citeste o tasta, o raporteaza prin Serial si trateaza
// resetarea (*), verificarea (#) sau adaugarea unui caracter la cod.
// Daca nu exista o tasta noua, nu modifica starea aplicatiei.
void update();
}

#endif
