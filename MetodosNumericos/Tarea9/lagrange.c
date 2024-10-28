/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 9
 * 
 * Devuelve el valor del polinomio de Lagrange de grado n, 
 * evaluado en el punto z.
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

double li(double z, int k, double *data, int n){
    double res = 1;
    for (int i=0;i<n;i++){
        if (i!=k)
            res *= (z-data[i])/(data[k]-data[i]);
    }

    return res;
}

double lagrange_interpolation(double z, double *data, int n){
    double res = 0;
    for (int i=0;i<n;i++)
        res += data[n+i]*li(z,i,data,n);
    
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

    int n = 6;
    double *data = (double *)malloc(2*n*sizeof(double));
    if (data==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    double z[] = {0.0, 0.5, 1.0, 1.5, 2.0}; // coordenada x de las observaciones
    double inter[] = {0.4, 0.8, 1.2, 1.6, 1.9}; // puntos a interpolar
    
    for (int i=0;i<=n;i++){
        data[i] = z[i];
        data[n+i] = exp(data[i]);
    }
    
    double res; // almacenara la aproximacion
    double err; // almacenara el error absoluto

    for (int i=0;i<=n;i++){
        res = lagrange_interpolation(inter[i],data,n+1);
        err = res-exp(inter[i]);
        printf("%g %g %g\n", inter[i], res, err);
    }

    free(data);

    return 0;
}