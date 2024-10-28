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

double neville(double z, double *data, int n){
    double **Q = genMatriz_double(n+1,n+1);
    if (Q==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }
    for (int i=0;i<=n;i++){
        Q[i][0] = data[i+n];
        for (int j=1;j<=i;j++){
            Q[i][j] = ((z-data[i-j])*Q[i][j-1]-(z-data[i])*Q[i-1][j-1])/(data[i]-data[i-j]);
        }
    }

    double res = Q[n][n];
    free_matrix(Q);

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
        res = neville(inter[i],data,n);
        err = fabs(res-exp(data[i]));
        printf("%g %g\n", res, err);
    }

    free(data);

    return 0;
}