/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 2

Calcula una aproximacion del la raiz de la ecuacion ingresada
a traves de los metodos de biseccion, Newton, secante y falsa posicion
*/

#include <stdio.h>
#include <math.h>

#define epsilon 0.0001 // umbral de tolerancia
#define N 100 // maximo numero de iteraciones

double f(double x){ // funcion objetivo
    return 1000000.0*exp(x)+435000.0*(exp(x)-1)/x-1564000;
    //return x*x-1;
}
double f_df(double x){ // Calcula f/f'
    return x*(1000000.0*x*exp(x)+435000.0*(exp(x)-1))/(exp(x)*(1000000.0*x*x+435000.0*x-435000)+435000.0);
    //return (x*x-1.0)/(2.0*x);
}

double biseccion(double a, double b){
    double c, fa, fb, fc, prev_c, diff;
    int iter;

    fa = f(a); // evaluacion de la funcion en los extremos
    fb = f(b);

    if (fa*fb<0){ // se revisa que haya una raiz en el intervalo
        for (int i=0;i<N;i++){
            iter = i;
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
                diff = fabs(c-prev_c)/fabs(c);
                if ((diff<epsilon)||(fc==0)){
                    printf("%d iteraciones\n",iter); // se imprime cuantas iteraciones se requirierob
                    return c;
                }
            }
            prev_c = c;
        }
        printf("BISECCION. NO SE PUDO ENCONTRAR LA RAIZ :(\n"); // si no se encuentra un resultado satisfactorio, se devuelve un mensaje de error
    }
    else
        printf("BISECCION. NO PUEDE USARSE EL INTERVALO DEFINIDO D:\n");
    
    return 0;
}

double newton(double x){
    double p0=x, p;
    int iter;
    for (int i=0;i<N;i++){
        iter = i;
        p = p0-f_df(p0); // calculo de la siguiente aproximacion
        if (fabs(p-p0)<epsilon){ // si se satisface la condici\'on de parada, la ejecucion se detiene y se devuelve el resultado encontrado
            printf("%d iteraciones\n",iter); // se imprime cuantas iteraciones se requirieron
            return p;
        }
        p0 = p; // actualizacion de p0
    }
    printf("NEWTON. NO SE PUDO ENCONTRAR LA RAIZ :(\n"); // si no se encuentra un resultado satisfactorio, se devuelve un mensaje de error
}

double secante(double x){
    double p, q0, q1, p0, p1;
    int iter;

    p0 = x;
    p1 = 2*x+0.1; // definicion de p1 cercano a p0
    q0 = f(p0); // calculo de q0 y q1
    q1 = f(p1);

    for (int i=0;i<N;i++){
        iter = i;
        p = p1-q1*(p1-p0)/(q1-q0); // calculo de la siguiente aproximacion
        if (fabs(p-p1)<epsilon){ // si se satisface la condici\'on de parada, la ejecucion se detiene y se devuelve el resultado encontrado
            printf("%d iteraciones\n",iter); // se imprime cuantas iteraciones se requirieron
            return p;
        }
        p0 = p1; // actualizacion de los parametros
        p1 = p;
        q0 = q1;
        q1 = f(p);
    }
    printf("SECANTE. NO SE PUDO ENCONTRAR LA RAIZ :(\n");
}

double falsa_posicion(double x, double p0, double p1){
    double p, q, q0, q1;
    int iter;

    q0 = f(p0); // calculo de q0 y q1
    q1 = f(p1);

    if (q0*q1<0){ // se revisa que el intervalo definido sea valido
        for (int i=0;i<N;i++){
            iter = i;
            p = p1-q1*(p1-p0)/(q1-q0); // calculo de la siguiente aproximacion
            if (fabs(p-p1)<epsilon){ // si se satisface la condici\'on de parada, la ejecucion se detiene y se devuelve el resultado encontrado
                printf("%d iteraciones\n",iter); // se imprime cuantas iteraciones se requirieron
                return p;
            }
            q = f(p); // calculo del resultado de la aproximacion
            if (q*q1<0){ // analiza en cual de los intervalos se encuentra la solucion y actualiza los extremos
                p1 = p;
                q1 = q;
            } else {
                p0 = p;
                q0 = q;
            }
        }
        printf("FALSA POSICION. NO SE PUDO ENCONTRAR LA RAIZ :(\n"); // si no se encuentra una solucin satisfactoria, se devuelve un mensaje de error
    } else
        printf("FALSA POSICION. NO PUEDEN USARSE LOS PUNTOS INGRESADOS :(\n"); // si el intervalo no es valido, se devuelve un mensaje de error
}

int main(){
    double lambda0=0.01;
    
    // Implementacion de los cuatro metodos
    printf("Biseccion: %lf\n", biseccion(-0.2, 3.0));
    printf("Newton: %lf\n", newton(lambda0));
    printf("Secante: %lf\n", secante(lambda0));
    printf("Falsa posicion: %lf\n",falsa_posicion(lambda0,-0.2,3.0));
}