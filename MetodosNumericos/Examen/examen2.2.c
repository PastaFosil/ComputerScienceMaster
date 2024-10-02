#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

int main(){
    int n = 10; // discretizacion
    double k = 1.0;
    double c = 1.0;
    double L = 1.0; // longitud del sistema
    double T = 0.2; // tiempo final (tiempo inicial = 0)
    double delta_x = L/(double)n; // discretizacion de longitud
    double t = T/(double)n; // discretizacion de tiempo
    double lambda = k*c/(delta_x*delta_x);
    double **A = genMatriz_double(n,n);
    double *u0 = genVector_double(n);

    for (int i=0;i<n;i++){
        u0[i] = 4*(delta_x*i-delta_x*delta_x*i*i);
        if (i-1>=0)
            A[i][i-1] = -lambda;
        A[i][i] = 1+2.0*lambda;
        if (i+1<n)
            A[i][i+1] = -lambda;
    }

    free_matrix(A);

    

    return 0;
}