/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 2

Calcula aproximaciones del valor de exp(1), y calcula 
los errores relativo y absoluto respecto al real
*/

#include <stdio.h>
#define exp1 2.718281828459045

int main(){
    double e, n, factor, er, ea;

    n = 1;
    printf("k\te\te.a.\te.r.\n");
    for (int k=1;k<=20;k++){ // se recorren todos los valores de k
        e = 1; // inicializacion del valor de e
        n *= 10; // calculo del valor de n para la k actual
        factor = 1+1/n; // factor a exponencial
        for (int i=0;i<n;i++) // calculo de e para la n dada
            e *= factor;
        ea = fabs(e-exp1); // calculo de error absoluto
        er = ea/fabs(e); // calculo de error relativo

        printf("%d\t%.3lf\t%.3lf\t%.3lf\n",k,e,ea,er); // impresion de los valores
    }
}