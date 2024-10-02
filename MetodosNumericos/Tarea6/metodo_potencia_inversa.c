/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5
 * 
 * Encuentra los valores propios de menor magnitud de una 
 * matriz junto con sus vectores propios asociados
************************************************************/

#include <stdio.h>
#include <string.h>
#include "../Tools/tools.c"

#define TOL 0.00001
#define MAX_ITER 10000

// Encuentra el mayor elemento del vector ingresado
double max(double *vec, int n){
    double max = vec[0];
    for (int i=1;i<n;i++){
        if (vec[i]>max)
            max = vec[i];
    }
    return max;
}

// Elimina la contribucion de los k vectores ortonormales de vec
void substract_contribution(double *vec, int n, double **eigenvectors, int k){
    double a;
    // elimina la contribucion de cada vector propio
    for (int i=0;i<k;i++){
        a = point_product(eigenvectors[i],vec,n); // factor de la contribucion del vector propio i
        for (int j=0;j<n;j++) // resta contribucion de vector propio i
            vec[j] -= a*eigenvectors[i][j];
    }
}

// Encuentra el k-esimo menor valor propio y su vector asociado de la matriz descompuesta en A=LU dados los k-1 anteriores
int inverse_power_iteration(double **L, double **U, int n, double **eigenvectors, int k){
    // revisa si ya se han encontrado otros valores propios
    int found = 0;
    if (k>0) // lista de vectores propios encontrados hasta el momento
        found = 1;
    
    double x0[n], deno;
    double *x1;
    double lambda0 = 10000.0, lambda;
    
    // inicialiacion x0 normalizado
    for (int i=0;i<n;i++)
        x0[i] = 1/sqrtf(n);
    
    // iteraciones para encontrar el resultado
    for (int i=0;i<MAX_ITER;i++){
        if (found==1) // si se han encontrado otros vectores propios, se resta su contribucion a la aproximacion
            substract_contribution(x0,n,eigenvectors,k);
        x1 = solve_upper_lower(U,L,x0,n); // calculo de la siguiente aproximacion del vector propio
        deno = point_product(x1,x1,n);
        if (deno!=0)
            lambda = point_product(x1,x0,n)/deno; // calculo de la siguiente aproximacion del valor propio
        else {
            printf("POWER. ERROR DIV. 0\n");
            return -1;
        }
        unit_vector(&x1,n); // normalizacion del vector aproximacion
        for (int j=0;j<n;j++) // actualizacion de los vectores aproximacion
            x0[j] = x1[j];
        free(x1);
        if (fabs(lambda-lambda0)/fabs(lambda)<TOL){ // almacenamiento y devolucion de resultados satisfactorios
            for (int j=0;j<n;j++)
                eigenvectors[k][j] = x0[j];
            eigenvectors[k][n] = lambda;
            return 0;
        }
        lambda0 = lambda; // actualizacion de valores aproximacion
    }

    printf("INVERSE POWER. NO SE ENCONTRO UNA SOLUCION SATISFACTORIA\n");
    return -1;
}

// Encuentra los k menores valores propios y sus vectores asociados de la matriz A
double **inverse_power_iteration_generalized(double **mtx, int n, int k){
    double **eigenstuff = genMatriz_double(n,n+1); // arreglo que guardara valores y vectores propios
    double **U = genMatriz_double(n,n);
    int result;

    if(eigenstuff==NULL||U==NULL){
        printf("POWER. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(eigenstuff);
        free_matrix(U);
        return NULL;
    }

    // factoriza la matriz ingresada en L y U
    factor_crout(mtx,U,n);
    //factor_choleskyLLT(mtx,L,n);

    // iteraciones para encontrar los f valores y vectores propios
    for (int i=0;i<k;i++)
        result = inverse_power_iteration(mtx,U,n,eigenstuff,i);

    free_matrix(U);

    if (result==0)
        return eigenstuff;
    else
        return NULL;
}

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: ./%s matriz.txt num_resultados\n", argv[0]);
        return -1;
    }

    int m,n, f = atoi(argv[2]);
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double *x;
    double lambda;
    char *out_file = argv[1];

    if(mtx==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        free_matrix(mtx);
        return -1;
    }

    // calculo de los f menores valores y vectores propios asociados
    double **eigenstuff = inverse_power_iteration_generalized(mtx,n,f);
    for (int i=0;i<f;i++)
        printf("%g\n",eigenstuff[i][n]);
        //print_vector(eigenstuff[i],n+1);

    // generacion de nombre de archivo txt de salida
    out_file[strlen(out_file)-4] = '_';
    out_file[strlen(out_file)-3] = '\0';
    strcat(out_file, argv[2]);
    strcat(out_file,".txt");

    // escritura de archivo txt de salida
    // almacena los f valores propios calculados con sus vectores propios asociados
    escribirMatriz_txt(out_file,eigenstuff,f,n+1);

    free_matrix(eigenstuff);
    free_matrix(mtx);
    
    return 0;
}