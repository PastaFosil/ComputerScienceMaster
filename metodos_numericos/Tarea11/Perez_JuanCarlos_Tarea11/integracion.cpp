/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * LIBRERIA DE INTEGRACION NUMERICA
 * 
 * 08/11/2024
************************************************************/

#include <iostream>
#include <cmath>
#include "integracion.h"

using namespace std;

double closed_newton_cotes(int n, double a, double b, double (*f)(double)){
    double h = (b-a)/n;

    switch (n){
        case 1:
            return h*(f(a)+f(b))/2.0;
        case 2:
            return h*(f(a)+4.0*f(a+h)+f(b))/3.0;
        case 3:
            return 3.0*h*(f(a)+3.0*f(a+h)+3*f(b-h)+f(b))/8.0;
        case 4:
            return 2.0*h*(7.0*f(a)+32.0*f(a+h)+12.0*f(a+2*h)+32.0*f(b-h)+7.0*f(b))/45.0;
        default:
            //cout << "ERROR. n INVALIDA" << endl;
            return NAN;
    }
}

double open_newton_cotes(int n, double a, double b, double (*f)(double)){
    double h = (b-a)/(n+2);

    switch (n){
    case 0:
        return 2.0*h*f(a+h);
    case 1:
        return 3.0*h*(f(a+h)+f(a+2*h));
    case 2:
        return 4.0*h*(2.0*f(a+h)-f(a+2*h)+2.0*f(a+3*h));
    case 3:
        return 5.0*h*(11.0*f(a+h)+f(a+2*h)+f(a+3*h)+11.0*f(a+4*h))/24.0;
    default:
        //cout << "ERROR. n INVALIDA" << endl;
        return NAN;
    }
}