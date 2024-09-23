/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 3

Resuelve un sistema de ecuaciones de la forma Dx=b,
con D una matriz diagonal.
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

    // Operacion sobre los elementos de la diagonal para obtener el valor de las variables
    for(int i = 0; i<k; i++){
        if (vector[i]!=0){
            vector[i] /= matriz[i][i];
            printf("%g\n",vector[i]);
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return -1;
        }
    }
    escribirVector_txt("vectorX_diagonal.txt",vector,m);

    //Liberando la memoria dinamica
    free(matriz[0]);
    free(matriz);
    free(vector);

    return 0;
}
