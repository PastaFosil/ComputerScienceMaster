/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 11
 * 
 * Usa la libreria con implementacion de los metodos de 
 * Newton-Cotes abierto y cerrado para la integracion 
 * numerica, usando las formulas para las diferentes n
************************************************************/

#include <iostream>
#include "integracion.h"
#include <cmath>
const double PI = 3.14159265358979323846;

using namespace std;

int main(){
    double a = 0.0, b = PI/4.0;

    for (int n = 0;n<5;n++)
        cout << n << "\t" << closed_newton_cotes(n,a,b,sin) << "\t" << open_newton_cotes(n,a,b,sin) << endl;
}