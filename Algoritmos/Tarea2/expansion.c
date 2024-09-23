/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 2

El programa acepta un entero n ingresado por el usuario y
calcula los primeros n terminos de la expansion de exp(-x^2)
*/

#include <stdio.h>

int main(){
    double n, fac, x, exp, pow;
    printf("x = ");
    scanf("%lf", &x);
    printf("n = ");
    scanf("%lf",&n);


    exp = 1;
    fac = 1;
    pow = 1;
    for (int i=1;i<n;i++){
        fac *= i;
        pow *= -x*x;
        exp += pow/fac;
    }

    printf("exp(-(%.2lf)^2) = %.2lf", x,exp);

    return 0;
}