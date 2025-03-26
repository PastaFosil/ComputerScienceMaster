/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5

 * Resuelve la ecuacion de calor a traves del sistema
 * lineal derivado mediante de diferencias finitas.
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.c"

double *solve_heat_eq(double Q, double K, double phi_0, double phi_n, double L, int n){
    // Discretizacion de longitud
    double delta_x = L/(double)n;
    n--;
    // Calculo de termino comun de las ecuaciones de calor
    double heat_term = Q*delta_x*delta_x/K;
    // Generacion de espacio para matriz de factores y vector solucion y de resultados
    double **mtx = (double **)malloc(n*sizeof(double *));
    double *temp = (double *)calloc(n*n,sizeof(double));
    double *vector = (double *)malloc(n*sizeof(double));

    // Llenado de la matriz con los terminos de las n-2 ecuaciones de calor y el vector de resultados
    for (int i=0;i<n;i++){
        vector[i] = heat_term;
        mtx[i] = temp+i*n;
        mtx[i][i] = 2.0;
        if (i>0)
            mtx[i][i-1] = -1.0;
        else
            vector[i] += phi_0;
        if (i<n-1)
            mtx[i][i+1] = -1.0;
        else
            vector[i] += phi_n;
    }

    // Calculo y devolucion de vector solucion por Cholesky LL^T
    return solve_choleskyLLT(mtx,vector,n);;
}

int main(int argc, char **argv){
    if (argc<7){
        printf("Uso: Q K phi_0 phi_n L n\n");
        return -1;
    }

    double *sol;

    sol = solve_heat_eq(atof(argv[1]),atof(argv[2]),atof(argv[3]),atof(argv[4]),atof(argv[5]),atoi(argv[6]));
    if (sol!=NULL){
        char file_name[200];
        // Generacion de nombre de archivo de vector solucion
        file_name[0] = 'h';
        strcat(file_name,"eat_solution_n_");
        /*
        strcat(file_name,"eat_solution_Q_");
        strcat(file_name,argv[1]);
        strcat(file_name,"_K_");
        strcat(file_name,argv[2]);
        strcat(file_name,"_phi0_");
        strcat(file_name,argv[3]);
        strcat(file_name,"_phin_");
        strcat(file_name,argv[4]);
        strcat(file_name,"_L_");
        strcat(file_name,argv[5]);
        strcat(file_name,"_n_");
        */
        strcat(file_name,argv[6]);
        strcat(file_name,".txt");

        escribirVector_txt(file_name,sol,atoi(argv[6])-1);

        //printf("\nSolucion:\n");
        double partition = atof(argv[5])/(double)atoi(argv[6]);

        printf("%d %g\n",0,atof(argv[3]));
        for (int i=0;i<atoi(argv[6])-1;i++){
            //printf("%g ",sol[i]);
            printf("%g %g\n",(i+1)*partition,sol[i]);
        }
        printf("%g %g\n",atof(argv[5]),atof(argv[4]));
        
        
        free(sol);

        return 0;
    } else
        return -1;
}
