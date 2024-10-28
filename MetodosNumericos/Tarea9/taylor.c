/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 9
 * 
 * Devuelve el valor del polinomio de Taylor de grado n, 
 * centrado en x0 y evaluado en el punto z.
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

double exp_taylor_n(double z, double x0, int n){
    double fac = 1.0;
    double power = 1.0;
    double res = 1.0;

    //res += exp(x0); // se evalua el termino 0 del polinomio
    for (int i=1;i<=n;i++){
        fac *= (double)i;
        power *= z - x0;
        res += power/fac;
    }

    return res*exp(x0);
}

int main(int argc, char **argv){
    if (argc<4){
        printf("Uso: ./%s z x0 n\n", argv[0]);
        return -1;
    }

    double res = exp_taylor_n(atof(argv[1]),atof(argv[2]), atoi(argv[3]));
    double err = res-exp(atof(argv[1]));
    printf("%s %g %g\n", argv[1], res, err);

    return 0;
}