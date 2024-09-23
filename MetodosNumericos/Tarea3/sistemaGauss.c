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
    double factor;

    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for(int m = 0; m<k; m++){ // recorre las columnas para hacer ceros los elementos bajo la diagonal
        if (matriz[m][m]!=0){
            for (int n=m+1;n<k;n++){ // recorre los renglones debajo de la diagonal
                factor = matriz[n][m]/matriz[m][m];
                vector[n] -= factor*vector[m];
                for (int o=m;o<k;o++){ // recorre las columnas a la derecha de m para actualizar los valores del renglon completo
                    matriz[n][o] -= factor*matriz[m][o];
                }
            }
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return -1;
        }
    }
    for(int i=k-1; i>=0; i--){ // recorre las filas de la ultima hacia la primera
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
    }
    escribirVector_txt("vectorX_gauss.txt",x,m);

    //Liberando la memoria dinamica
    free(matriz[0]);
    free(matriz);
    free(vector);
    free(x);

    return 0;
}
