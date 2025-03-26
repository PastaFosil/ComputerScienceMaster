/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 9
 * 
 * Devuelve el valor del polinomio de diferencias divididas
 * de grado n, evaluado en el punto z.
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

double divided_difference(double z, double *data, int n){
    double **F = genMatriz_double(n+1,n+1);
    if (F==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }
    for (int i=0;i<=n;i++){
        F[i][0] = data[i+n];
        for (int j=1;j<=i;j++)
            F[i][j] = (F[i][j-1]-F[i-1][j-1])/(data[i]-data[i-j]);
    }

    double P = 0;
    for (int i=0;i<=n;i++){
        double temp = F[i][i];
        for (int j=0;j<i;j++)
            temp *= (z - data[j]);
        P += temp;
    }

    free_matrix(F);

    return P;
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
    /* PARA PROBLEMA 5
    double z[] = {0.0, 0.5, 1.0, 1.5, 2.0}; // coordenada x de las observaciones
    double inter[] = {0.4, 0.8, 1.2, 1.6, 1.9}; // puntos a interpolar
    int n = 4; // orden de interpolacion
    double *data = (double *)malloc(2*n*sizeof(double));
    if (data==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }
    
    for (int i=0;i<=n;i++){
        data[i] = z[i];
        data[n+i] = exp(data[i]);
    }

    double res;
    double err;

    for (int i=0;i<=n;i++){
        res = divided_difference(inter[i],data,n);
        err = fabs(res-exp(inter[i]));
        printf("%g %g\n", res, err);
    }*/

    int n = 6; // orden de interpolacion
    double *data = (double *)malloc(2*n*sizeof(double));
    if (data==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    double I[] = {0.001, 0.005, 0.010, 0.015, 0.019, 0.022};
    double V[] = {0.7252, 0.62625, 0.57969, 0.53438, 0.43906, 0.28125};
    for (int i=0;i<n;i++){
        data[i] = I[i];
        data[n+i] = V[i];
    }
    double inter[] = {0.0075, 0.0125, 0.0175}; // coordenadas a interpolar
    double res; // almacenara la aproximacion

    for (int i=0;i<=2;i++){
        res = divided_difference(inter[i],data,n);
        printf("%g %g\n", inter[i], res);
    }

    free(data);

    return 0;
}