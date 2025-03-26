/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 4

Resuelve un sistema de ecuaciones mediante factorizacion de Cholesky 
de la forma LL^T.
*/

#include <stdio.h>
#include <math.h>
#include "tools.c"

// producto matriz por matriz
double **MxM(double **mtz1, double **mtz2, int n){
    /*
        Multiplica dos matrices cuadradas de igual dimension 
        y devuelve el resultado
    */

    double **res = genMatriz_double(n,n);

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            for (int k=0;k<n;k++){
                res[i][j] += mtz1[i][k]*mtz2[k][j];
            }
        }
    }
    
    return res;
}

// resolucion de sistema lineal descrito por matriz triangular inferior
double *x_inferior(double **matriz, double *vector, int n){

    double *x = (double *)calloc(n, sizeof(double));
    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for(int i = 0; i<n; i++){ // recorre las filas de inicio a final
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
            return NULL;
        }
    }

    return x;
}

// resolucion de sistema lineal descrito por matriz triangular inferior
double *x_superior(double **matriz, double *vector, int n){
    double *x = (double *)calloc(n, sizeof(double));
    
    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for(int i = n-1; i>=0; i--){ // recorre las filas de la ultima hacia la primera
        if (matriz[i][i]!=0){
            if (i==n){ // caso particular del elemento final
                x[n] = vector[i]/matriz[i][i];
            }
            else { // calculo del resto de los elementos
                x[i] = vector[i];
                for (int j=i+1;j<n;j++){
                    x[i] -= matriz[i][j]*x[j];
                }
                x[i] /= matriz[i][i];
            }
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return NULL;
        }
    }

    return x;
}

// liberacion de la memoria asignada
void free_matrix(double **mtx){
    if (mtx!=NULL){
        free(mtx[0]);
        free(mtx);
    }
}

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: %s matriz.txt vector.txt\n",argv[0]);
        return 1;
    }

    // Dimensiones de matriz y vector
    int m,n,p;

    // Llamada a valores para leer los archivos
    double **matriz = leerMatriz_txt(argv[1], &m,&n); // matriz original
    double **LT = genMatriz_double(n,n); // matriz L traspuesta
    double *vector = leerVector_txt(argv[2], &p); // vector de constantes (posteriormente guardara el vector solucion del sistema)
    double *y = genVector_double(m); // vector que guardara Ly=b

    if (matriz==NULL||vector==NULL||y==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    int positiva = 1;
    printf("Matriz A:\n");
    for (int i=0;i<m;i++){
        if (matriz[i][i]<=0)
            positiva = 0;
        for (int j=0;j<m;j++){
            if (matriz[i][j]!=matriz[j][i])
                positiva = 0;
            printf("%g ",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    if (positiva==0){
        printf("\nNO PUEDE REALIZARSE LA FACTORIZACION DE LA MATRIZ\n");
        return -1;
    }

    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for (int i=0; i<n; i++){ // recorre los renglones de inicio a final
        for (int j=0;j<=i;j++){ // recorre las columnas de la matriz triangular inferior
            if (i==j){ // elementos de la diagonal
                for (int k=0;k<j;k++)
                    matriz[i][j] -= matriz[i][k]*matriz[i][k];
                matriz[i][j] = sqrt(matriz[i][j]);
            } else { // elementos fuera de la diagonal
                for (int k=0;k<j;k++)
                    matriz[i][j] -= matriz[i][k]*matriz[j][k];
                matriz[i][j] /= matriz[j][j];
                matriz[j][i] = 0.0; // se anulan elementos arriba de la diagonal
            }
            LT[j][i] = matriz[i][j]; // se asignan valores de la matriz traspuesta
        }
    }
    /*
    printf("L\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++){
            printf("%g ",matriz[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("LT\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++)
            printf("%g ",LT[i][j]);
        printf("\n");
    }
    printf("\n");
    */

    // guarda matriz L en un archivo txt
    escribirMatriz_txt("choleskyLLT_A1_L.txt",matriz,m,n);

    y = x_inferior(matriz,vector,n);
    
    // calculo y almacenamiento del vector solucion
    vector = x_superior(LT,y,n);
    escribirVector_txt("choleskyLLT_A1_sol.txt",vector,n);
    
    printf("Solucion al sistema:\n");
    for (int i=0;i<n;i++){
        printf("%g ", vector[i]);
    }
    printf("\n");
    
    
    // liberando la memoria dinamica
    free_matrix(matriz);
    free_matrix(LT);
    free(vector);
    free(y);

    return 0;
}
