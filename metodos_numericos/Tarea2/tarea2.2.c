/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 2

Calcula una aproximacion del la raiz de la ecuacion ingresada
a traves del metodo de biseccion
*/

#include <stdio.h>

#define epsilon 0.0001 // umbral de tolerancia
#define N 10000 // maximo numero de iteraciones
#define root 1.634365320 // raiz calculada analiticamente

double f(double x){ // funcion objetivo
    return x*x*x + x - 6;
}

int main(){
    double a, b, c, fa, fb, fc, prev_c, diff;
    int iterations;

    printf("Ingrese los limites del intervalo [a,b]:\n");
    printf("a = ");
    scanf("%lf",&a);
    printf("b = ");
    scanf("%lf",&b);
    
    fa = f(a); // evaluacion de la funcion en los extremos
    fb = f(b);

    if (fa*fb<0){ // se revisa que haya una raiz en el intervalo
        for (int i=0;i<N;i++){
            iterations = i;
            c = (a+b)/2.0; // calcula el punto medio del intervalo
            fc = f(c); // evalua la funcion en c

            if (fa*fc<0){ // se analiza en cual de los subintervalos esta la raiz
                b = c;
                fb = fc;
            } else if (fb*fc<0){
                a = c;
                fa = fc;
            }
            if (i>0){ // calculo de la diferencia relativa entre puntos medios consecutivos
                diff = fabs(c-prev_c);
                diff /= fabs(c);
                if ((diff<epsilon)||(fc==0)){
                    break;
                }
            }
            prev_c = c;
        }

        diff = c-root; // calculo del error relativo entre soluciones numerica y analitica
        if (diff<0){
            diff *= -1.0;
        }
        diff /= c;
        if (diff<0){
            diff *= -1.0;
        }
        printf("La raiz se encuentra en x = %lf\n",c);
        printf("Se tiene un error de %lf\%\n", diff*100);
        printf("Se necesitaron %d iteraciones\n",iterations);
    }
    else
        printf("\n\nNO PUEDE USARSE EL INTERVALO DEFINIDO D:\n\n");
    
    return 0;
}
