/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * PARCIAL 2

 * 02/10/2024
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

int main(){
    int n = 20;
    double **mtx = genMatriz_double(n,n);
    double **A = genMatriz_double(n,n);
    double **eigenstuff;
    double *b = genVector_double(n);
    double *x1, *x2; 
    if (mtx==NULL||A==NULL||b==NULL){
        free_matrix(mtx);
        free_matrix(A);
        free(b);
        return -1;
    }

    for (int i=0;i<n;i++){
        if (i-2>=0)
            mtx[i][i-2] = -4.0;
        if (i-1>=0)
            mtx[i][i-1] = -8.0;
        mtx[i][i] = 40.0;
        if (i+1<n)
            mtx[i][i+1] = -8.0;
        if (i+2<n)
            mtx[i][i+2] = -4.0;
        
        if (i==0||i==n-1)
            b[i] = 20.0;
        else if (i==1||i==n-2)
            b[i] = 50.0;
        else
            b[i] = 100.0;
    }

    copy_matrix(A,mtx,n,n);
    // se resuelve el sistema por el metodo de Gauss-Seidel
    x1 = solve_gauss_seidel(A,b,n);
    if (x1==NULL){
        free_matrix(mtx);
        free_matrix(A);
        free(b);
        return -1;
    }
    printf("\nGauss Seidel\n");
    print_vector(x1,n);
    printf("\n");

    escribirVector_txt("resolucion_gsb.txt",x1,n);

    free(x1);
    
    // Se resuelve el sistema por factorizacion LLT
    double **LT = genMatriz_double(n,n);
    if (LT==NULL){
        free_matrix(mtx);
        free_matrix(A);
        free(b);
        return -1;
    }

    //Factoriza la matriz
    copy_matrix(A,mtx,n,n);
    factor_LLT(A,LT,n);
    printf("LT an examen\n");
    print_matrix(A,n,n);
    // Resuelve el sistema
    x2 = solve_upper_lower(LT,A,b,n);
    if (x2==NULL){
        free_matrix(mtx);
        free_matrix(A);
        free_matrix(LT);
        free(b);
        return -1;
    }

    printf("LLT\n");
    print_vector(x2,n);
    printf("\n");
    escribirVector_txt("resolucion_LLTb.txt",x2,n);

    free_matrix(LT);
    free(x2);
    
    // Encuentra los 10 mayores valores propios y sus vectores asociados
    copy_matrix(A,mtx,n,n);
    eigenstuff = power_iteration_generalized(A,n,10);
    if (eigenstuff==NULL){
        printf("10 MAYORES. ALGO SALIO MAL\n");
        free_matrix(mtx);
        free_matrix(A);
        free(b);
        return -1;
    }

    printf("\n10 mayores valores propios:\n");
    for (int i=0;i<10;i++)
        printf("%g ",eigenstuff[i][n]);
    printf("\n\n");

    escribirMatriz_txt("mayores_vectores_propiosb.txt",eigenstuff,10,n+1);
    
    free_matrix(eigenstuff);
    free_matrix(A);
    
    // Encuentra los 10 menores valores propios y sus vectores asociados
    eigenstuff = inverse_power_iteration_generalized(mtx,n,10);
    if (eigenstuff==NULL){
        printf("10 MENORES. ALGO SALIO MAL\n");
        free_matrix(mtx);
        free(b);
        return -1;
    }

    printf("\n10 menores valores propios:\n");
    for (int i=0;i<10;i++)
        printf("%g ",eigenstuff[i][n]);
    printf("\n");

    escribirMatriz_txt("menores_vectores_propiosb.txt",eigenstuff,10,n+1);
    
    free_matrix(eigenstuff);
    
    free_matrix(mtx);
    free(b);
    return 0;
}
