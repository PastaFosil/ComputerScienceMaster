/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 10
 * 
 * Devuelve el valor del polinomio de Hermite de grado n, 
 * evaluado en el punto z.
************************************************************/

#include <stdio.h>
#include "../Tools/tools.c"

double hermite_interpolation(double z, double *data, int n){
    double res = 0.0;
    double **Q = genMatriz_double(2*n+2,2*n+2);
    double *zi = genVector_double(2*n+2);
    if (Q==NULL || zi==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    for (int i=0;i<n;i++){
        zi[2*i] = data[i];
        zi[2*i+1] = data[i];
        Q[2*i][0] = data[i+n];
        Q[2*i+1][0] = data[i+n];
        Q[2*i+1][1] = data[i+2*n];
        if (i!=0)
            Q[2*i][1] = (Q[2*i][0]-Q[2*i-1][0])/(zi[2*i]-zi[2*i-1]);
    }
    for (int i=2;i<=2*n+1;i++){
        for (int j=2;j<=i;j++)
            Q[i][j] = (Q[i][j-1]-Q[i-1][j-1])/(zi[i]-zi[i-j]);
    }

    double factor = 1.0;
    int counter = 0;
    int j = 0;
    for (int i=0;i<=2*n+1;i++){
        res += Q[i][i]*factor;
        factor *= z-zi[j];
        counter++;
        if (counter==2){
            counter = 0;
            j++;
        }
    }

    free_matrix(Q);
    free(zi);

    return res;
}

int main(){
    double z[] = {0.3, 0.32, 0.35}; // coordenada x de las observaciones
    int n = sizeof(z)/sizeof(double); // numero de datos
    double inter[] = {0.34}; // puntos a interpolar
    int ni = sizeof(inter)/sizeof(double); // numero de puntos a interpolar

    double *data = (double *)malloc(3*n*sizeof(double));
    if (data==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    for (int i=0;i<n;i++){
        data[i] = z[i];
        data[n+i] = sin(data[i]);
        data[2*n+i] = cos(data[i]);
    }

    double res;
    double err;
    for (int i=0;i<ni;i++){
        res = hermite_interpolation(inter[i],data,n);
        err = fabs(res-sin(inter[i]));
        printf("%g %g %g\n", inter[i], res, err);
    }

    free(data);

    return 0;
}