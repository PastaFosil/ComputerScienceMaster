/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 9
 * 
 * Devuelve el valor del polinomio de Ma de grado n, 
 * centrado en x0 y evaluado en el punto z.
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

double li(double x, int k, double *data, int n){
    double res = 1;
    for (int i=0;i<n;i++){
        if (i!=k)
            res *= (x-data[i])/(data[k]-data[i]);
    }

    return res;
}

double lagrange_interpolation(double x, double *data, int n){
    double res = 0;
    for (int i=0;i<n;i++)
        res += data[n+i]*li(x,i,data,n);
    
    return res;
}

/*int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: ./%s z n\n", argv[0]);
        return -1;
    }

    double z = atof(argv[1]);
    int n = atoi(argv[2]);
*/
int main(){
    double z[] = {0.0, 0.5, 1.0, 1.5, 2.0};
    double inter[] = {0.4, 0.8, 1.2, 1.6, 1.9};
    int n = 4;
    double *data = (double *)malloc(2*n*sizeof(double));
    if (data==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }
    double begin = 0.0, end = 2.0;
    double delta = (end-begin)/n;
    for (int i=0;i<=n;i++){
        data[i] = i*delta;
        data[n+i] = exp(data[i]);
    }
    double res;
    double err;

    for (int i=0;i<=n;i++){
        res = lagrange_interpolation(inter[i],data,n+1);
        err = res-exp(inter[i]);
        //printf("%g %g %g\n", inter[i], res, err);
        printf("%g\n",exp(inter[i]));
    }

    free(data);

    return 0;
}