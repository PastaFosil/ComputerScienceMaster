/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5

 * Resuelve la ecuacion de calor a traves del sistema
 * lineal derivado mediante de diferencias finitas.
************************************************************/

#include <stdio.h>
#include "tools.c"
#define TOL 0.0001
#define N 50000

double *solve_gauss_seidel(double **matrix, double *vector, int n){
    double *x0 = (double *)calloc(n,sizeof(double)); // vector solucion propuesto
    double *x_new = (double *)malloc(n*sizeof(double)); // vector propuesto de la siguiente iteracion
    double *temp = (double *)malloc(n*sizeof(double)); // vector temporal que almacenara x_new-n0

    if (x0==NULL||x_new==NULL||temp==NULL){
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        free(x0);
        free(x_new);
        free(temp);
        return NULL;
    }

    for (int i=0;i<N;i++){
        for (int j=0;j<n;j++){
            // calculo de componentes de n_new
            x_new[j] = vector[j];
            for (int k=0;k<n;k++){
                if (k<j)
                    x_new[j] -= matrix[j][k]*x_new[k];
                if (k>j)
                    x_new[j] -= matrix[j][k]*x0[k];
            }
            if (matrix[j][j]==0){
                printf("JACOBI. NO SE PUEDE EMPLEAR ESTE METODO (DIV. 0)\n");
                return NULL;
            }
                
            x_new[j] /= matrix[j][j];
            
            // calculo de componentes de vector temporal (se usara para comprobar precision del resultado)
            temp[j] = x_new[j]-x0[j];
        }
        
        if (norm(temp,n)<TOL){
            printf("%d ITERACIONES\n",i);
            return x_new;
        }
        for (int j=0;j<n;j++)
            x0[j] = x_new[j];
    }

    printf("NUMERO MAXIMO DE ITERACIONES EXCEDIDO\n");
    return NULL;
}

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: ./%s matriz.txt vector.txt\n", argv[0]);
        return -1;
    }
    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double *vector = leerVector_txt(argv[2],&n);
    double *sol;
    
    sol = solve_jacobi(mtx,vector,n);
    escribirVector_txt("gs_big_sol.txt",sol,n);

    printf("\nSolucion:\n");
    for (int i=0;i<n;i++)
        printf("%g ",sol[i]);
    printf("\n");

    free_matrix(mtx);
    free(vector);
    free(sol);
}
