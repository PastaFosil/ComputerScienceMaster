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
    for(int i = 0; i<k; i++){ // recorre las filas de inicio a final
        if (matriz[i][i]!=0){
            if (i==0){ // caso especial del primer elemento
                x[i] = vector[i]/matriz[i][i];
            }
            else { // calculo del resto de los elementos
                x[i] = vector[i];
                for (int j=i-1;j>=0;j--){
                    x[i] -= matriz[i][j]*x[j];
                }
                x[i] /= matriz[i][i];
            }
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return -1;
        }
    }
    escribirVector_txt("vectorX_inferior.txt",x,m);

    //Liberando la memoria dinamica
    free(matriz[0]);
    free(matriz);
    free(vector);
    free(x);

    return 0;
}
