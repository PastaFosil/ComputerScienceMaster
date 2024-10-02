/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 6
 * 
 * Diagonaliza la matriz ingresada y calcula la matriz
 * de transformacion necesaria para lograrlo
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tools.c"

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s matriz.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double **T = jacobi_eigen_method(mtx,n);
    if (mtx==NULL||T==NULL){
        free_matrix(mtx);
        free_matrix(T);
        printf("NO SE ENCONTRO UN RESULTADO SATISFACTORIO\n");
        return -1;
    }

    printf("Valores propios:\n");
    for (int i=0;i<n;i++)
        printf("%g ",mtx[i][i]);
    
    printf("\n\n");
    printf("Vectores propios:\n");
    print_matrix(T,n,n);

    //double **new1 = MxM()

    free_matrix(mtx);
    free_matrix(T);
    return 0;
}
