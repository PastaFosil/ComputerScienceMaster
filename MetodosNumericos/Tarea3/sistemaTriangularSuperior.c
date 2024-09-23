/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 3

Resuelve un sistema de ecuaciones de la forma Ux=b,
con U una matriz triangular superior.
*/

#include <stdio.h>
#include "tools.c"

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: %s matriz.txt vector.txt\n",argv[0]);
        return 1;
    }

    // Dimensiones de matriz y vector
    int j,k,m;

    // Llamada a valores para leer los archivos
    double **matriz = leerMatriz_txt(argv[1], &j,&k);
    double *vector = leerVector_txt(argv[2], &m);
    double *x = (double *)calloc(m, sizeof(double));
    
    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for(int i = k-1; i>=0; i--){ // recorre las filas de la ultima hacia la primera
        if (matriz[i][i]!=0){
            if (i==k){ // caso particular del elemento final
                x[k] = vector[i]/matriz[i][i];
            }
            else { // calculo del resto de los elementos
                x[i] = vector[i];
                for (int j=i+1;j<k;j++){
                    x[i] -= matriz[i][j]*x[j];
                }
                x[i] /= matriz[i][i];
            }
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return -1;
        }
    }
    escribirVector_txt("vectorX_superior.txt",x,m);

    //Liberando la memoria dinamica
    free(matriz[0]);
    free(matriz);
    free(vector);
    free(x);

    return 0;
}
