/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 2

Calcula una aproximacion del la raiz de la ecuacion ingresada
a traves del metodo de punto fijo
*/

#include <stdio.h>
#include <math.h>

#define epsilon 0.0001 // umbral de tolerancia
#define N 20 // maximo numero de iteraciones

double f(double x){
    return x*x*x*x+3*x*x-2;
}

double g(double x, int op){ // diferentes formulaciones de la funcion g
    switch (op){
        case 0:
            return sqrt((2-x*x*x*x)/3.0);
        case 1:
            return pow(2-3*x*x,1.0/4.0);
        case 2:
            return (2-x*x*x*x)/(3*x);
        case 3:
            return cbrt((2-3*x*x)/x);
    }
}

int main(){
    double p0=1, p, diff;
    char incisos[] = {'a','b','c','d'};
    printf("Inciso\tRaiz encontrada\n");
    for (int j=0;j<4;j++){ // se emplea cada formulacion de g
        printf("j=%d\n",j);
        p0 = 1;
        for (int i=0;i<N;i++){ // iteraciones del metodo
            //printf("\tp0(x)=%lf\n",p0);
            p = g(p0,j);
            //printf("\tp(x)=%lf\n",p);
            diff = p-p0;
            if (fabs(diff)<epsilon)
                break;
            p0 = p;
        }
        printf("%d\t%lf\n",*(incisos+j),p);
        //printf("La raiz encontrada esta en x = %lf\n",p);
    }

    return 0;
}