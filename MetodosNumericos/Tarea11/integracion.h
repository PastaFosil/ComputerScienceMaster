/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * LIBRERIA DE INTEGRACION NUMERICA
 * 
 * 08/11/2024
************************************************************/

#ifndef INTEGRACION_H
#define INTEGRACION_H

double closed_newton_cotes(int n, double a, double b, double (*f)(double));
double open_newton_cotes(int n, double a, double b, double (*f)(double));

#endif // INTEGRACION_H