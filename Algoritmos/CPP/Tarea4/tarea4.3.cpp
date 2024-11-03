/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4 C++
 * 
 * Muestra la posicion de todos los bits activos.
************************************************************/

#include <iostream>
using namespace std;

int main(){
    unsigned int a = 4294967295;
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
    
    return 0;
}