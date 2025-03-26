/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4 C++
 * 
 * Verifica si un numero es potencia de 2.
************************************************************/

#include <iostream>
using namespace std;

int main(){
    unsigned int a = 1;
    int size = 8*sizeof(unsigned int);
    unsigned int mask = 1;

    int count = 0;
    for (int i=0;i<size;i++){
        if (a & mask)
            count++;
        if (count>1){
            cout << a << " NO ES POTENCIA DE 2" << endl;
            return 0;
        }
        mask = mask<<1;
    }

    if (count=0)
        cout << a << " NO ES POTENCIA DE 2" << endl;
    else
        cout << a << " ES POTENCIA DE 2" << endl;
    
    return 0;
}