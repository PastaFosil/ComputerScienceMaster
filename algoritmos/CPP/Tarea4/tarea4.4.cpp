/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4 C++
 * 
 * Intercambia bits pares e impares.
************************************************************/

#include <iostream>
using namespace std;

void show_bits(unsigned int a){
    int size = 8*sizeof(unsigned int);
    unsigned int mask = 1;
    char bits[size+1];
    bits[size] = '\0';

    for (int i=1;i<size;i++){
        if (a & mask)
            bits[size-i-1] = '1';
        else 
            bits[size-i-1] = '0';

        mask = mask<<1;
    }

    cout << a << " = " << bits << endl;
}

int main(){
    unsigned int a = 141;
    unsigned int even = a & 0x55555555;
    unsigned int odd = a & 0xAAAAAAAA;

    cout << "a: " << a << endl;
    show_bits(a);

    even = even << 1;
    odd = odd >> 1;
    a = 0;
    a = even | odd;

    cout << "a: " << a << endl;
    show_bits(a);
    
    return 0;
}