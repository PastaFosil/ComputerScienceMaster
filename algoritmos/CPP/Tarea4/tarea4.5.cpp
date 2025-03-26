/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4 C++
 * 
 * Reacomoda la secuencia de bits dada en la tarea.
************************************************************/

#include <iostream>
using namespace std;

void show_bits(unsigned int a){
    char mask = 1;
    char bits[9];
    bits[9] = '\0';

    for (int i=1;i<9;i++){
        if (a & mask)
            bits[8-i] = '1';
        else 
            bits[8-i] = '0';

        mask = mask<<1;
    }

    cout << bits << endl;
}

int main(){
    char a = 170;
    char even = a & 0x5;
    char odd = a & 0xA;
    char mask_8 = 254;
    char mask_7 = 253;
    char mask_6 = 251;
    char mask_5 = 247;
    char mask_4 = 239;
    char mask_3 = 223;
    char mask_2 = 191;
    char mask_1 = 127; 
    char mask_erase = 63;
    char temp = 0;

    cout << "Orden original y numeracion de los bits\n";
    cout << "12345678" << endl;
    show_bits(a);
    cout << "   ^\n";
    
    // se mueven implicitamente los bits 6 a 4 y 8 a 6
    show_bits(a);
    cout << "     ^\n";
    show_bits(a);
    cout << "       ^\n";

    // bit 7 se mueve a bit 8
    temp = ~mask_7 & a;
    temp = temp >> 1;
    a = mask_7 & a;
    a = a | temp;
    show_bits(a);
    cout << "      ^\n";

    // bits 1, 3 y 5 se mueven 2 espacios a la derecha
    temp = (~mask_1 & a) | (~mask_3 & a) | (~mask_5 & a);
    temp = (temp >> 2) & mask_erase;
    a = (mask_1 & a) & (mask_3 & a) & (mask_5 & a);
    a = a | temp;
    show_bits(a);
    cout << "^\n";

    // bit 2 se mueve a bit 1
    show_bits(a);
    cout << " ^\n";
    // bit 4 y bit 6 se mueven 2 espacios a la izquierda
    show_bits(a);
    cout << "   ^\n";
    show_bits(a);
    cout << "     ^\n";

    // bit 5 se mueve a bit 6
    temp = ~mask_5 & a;
    temp = temp >> 1;
    a = mask_5 & a;
    a = a | temp;
    show_bits(a);
    cout << "    ^\n";

    // bit 3 se mueve a bit 5
    temp = ~mask_3 & a;
    temp = temp >> 2;
    a = mask_3 & a;
    a = a | temp;
    show_bits(a);
    cout << "  ^\n";

    // bit 4 se mueve a bit 3
    show_bits(a);
    cout << "   ^\n";

    //cout << "AQUI\n";
    //show_bits(temp);
    //even = even << 1;
    //odd = odd >> 1;
    //a = 0;
    //a = even | odd;

    //cout << "a: " << a << endl;
    //show_bits(a);
    
    return 0;
}