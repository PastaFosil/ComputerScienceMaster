/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5
 * 
 * Diagonaliza la matriz ingresada y calcula la matriz
 * de transformacion necesaria para lograrlo
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

// Eleva la matriz ingresada al cuadrado mediante descomposicion espectral
double **matrix2(double **mtx, int n){
    double **A = genMatriz_double(n,n);
    if (A==NULL){
        printf("MATRIZ CUADRADA. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    copy_matrix(A,mtx,n,n);

    double **T = jacobi_eigen_method(A,n);
    if (T==NULL){
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        return NULL;
    }
    
    for (int i=0;i<n;i++){
        A[i][i] = A[i][i]*A[i][i];
    }

    double **fA = MxM(T,A,n);
    if (fA==NULL){
        free_matrix(T);
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        return NULL;
    }

    copy_matrix(A,fA,n,n);
    free_matrix(fA);
    traspose(T,n);
    fA = MxM(A,T,n);
    if (fA==NULL){
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        free_matrix(T);
        return NULL;
    }

    free_matrix(A);
    free_matrix(T);
    return fA;
}

// Calcula la raiz cuadrada de la matriz ingresada mediante descomposicion espectral
double **sqrt_matrix(double **mtx, int n){
    double **A = genMatriz_double(n,n);
    if (A==NULL){
        printf("MATRIZ CUADRADA. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    copy_matrix(A,mtx,n,n);
    double **T = jacobi_eigen_method(A,n);
    if (T==NULL){
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        return NULL;
    }

    for (int i=0;i<n;i++){
        A[i][i] = sqrt(A[i][i]);
    }

    double **fA = MxM(A,T,n);
    if (fA==NULL){
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        return NULL;
    }

    copy_matrix(A,fA,n,n);
    traspose(T,n);
    fA = MxM(T,A,n);
    if (fA==NULL){
        printf("MATRIZ CUADRADA. ALGO SALIO MAL\n");
        return NULL;
    }

    free_matrix(A);
    free_matrix(T);
    return fA;
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s matriz.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double **A = genMatriz_double(n,n);
    if (mtx==NULL||A==NULL){
        free_matrix(mtx);
        free_matrix(A);
        printf("NO SE ENCONTRO UN RESULTADO SATISFACTORIO\n");
        return -1;
    }
    copy_matrix(A,mtx,n,n);

    double **new_mtx = sqrt_matrix(mtx,n);
    if (new_mtx==NULL){
        free_matrix(mtx);
        printf("NO SE ENCONTRO UN RESULTADO SATISFACTORIO\n");
        return -1;
    }

    printf("jacobi\n");
    print_matrix(new_mtx,n,n);

    //double **new_mtx2 = MxM(mtx,mtx,n);
    //printf("directo\n");
    //print_matrix(new_mtx2,n,n);
    

    free_matrix(mtx);
    free_matrix(A);
    free_matrix(new_mtx);
    //free_matrix(new_mtx2);
}