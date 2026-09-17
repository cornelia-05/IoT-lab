#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

// Interfata LCD-ului; conexiunile si obiectul hardware sunt private modulului.
namespace LcdDisplay
{
// Configureaza LCD-ul cu 16 coloane si 2 randuri.
void initialize();
// Sterge intregul continut al afisajului.
void clear();
// Sterge afisajul si scrie invitatia de introducere pe primul rand.
void showPrompt();
// Scriu rezultatul pe primul rand, fara a sterge in prealabil afisajul.
void showCorrectCode();
void showIncorrectCode();
// Scrie count asteriscuri de la inceputul randului al doilea, fara stergere.
void showMaskedCode(unsigned int count);
}

#endif
