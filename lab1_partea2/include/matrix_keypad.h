#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H

namespace MatrixKeypad
{
// Citeste o tasta prin biblioteca Keypad; intoarce 0 daca nu exista o tasta
// noua. Pastreaza caracterele 0-9, A-D, * si # din matricea originala.
char readKey();
}

#endif
