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

double x2ln(double x){
    return x*x*log(x);
}

double x2exp_x(double x){
    return x*x*exp(-x);
}

int main(){
    double a = 0.0, b = PI/4;

    double errc, erro, closed, open, real = 0.1606027941427884;
    for (int n = 0;n<5;n++){
        closed = closed_newton_cotes(n,a,b,sin);
        errc = fabs(real-closed)/real;
        open = open_newton_cotes(n,a,b,sin);
        erro = fabs(real-open)/real;

        cout << n << "\t" << closed << "\t" << errc << "\t" << open << "\t" << erro  << endl;
    }
}