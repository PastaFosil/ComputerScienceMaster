/* 
JUAN CARLOS PEREZ RAMIREZ
METODOS NUMERICOS
TAREA 4

Resuelve un sistema de ecuaciones mediante factorizacion LU.
*/

#include <stdio.h>
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
    double **matriz = leerMatriz_txt(argv[1], &m,&n); // matriz original (posteriormente almacenara los valores de L)
    double *vector = leerVector_txt(argv[2], &p); // vector de constantes (posteriormente guardara el vector solucion del sistema)
    double **U = genMatriz_double(m,n); // matriz triangular superior
    double **res = genMatriz_double(m,n); // matriz que guardara el producto LU
    double *y = genVector_double(m); // vector que guardara Ly=b

    if (matriz==NULL||U==NULL||vector==NULL||res==NULL||y==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    printf("Matriz A:\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++)
            printf("%g ",matriz[i][j]);
        
        printf("\n");
    }
    printf("\n");

    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for (int j = 0; j<n; j++){ // recorre las columnas de inicio a final
        for (int i=0;i<=j;i++){ // recorre los renglones de la matriz triangular superior (los de la inferior se recorren por reflejo)
            // primer termino de cada elemento de U (dado que matriz guardara L, este paso esta implicito para L)
            U[i][j] = matriz[i][j];
            
            // recorre los elementos involucrados en la sumatoria
            for (int k=0;k<i;k++)
                U[i][j] -= matriz[i][k]*U[k][j];
            for (int k=0;k<i;k++)
                matriz[j][i] -= matriz[j][k]*U[k][i];
            matriz[j][i] /= U[i][i];
            if (i!=j)
                matriz[i][j] = 0.0; // anula los elementos de matriz arriba de la diagonal y divide elementos de matriz entre los correspondientes elementos de U
        }
    }
    /*
    printf("L\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++)
            printf("%g ",matriz[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("U\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++)
            printf("%g ",U[i][j]);
        printf("\n");
    }
    printf("\n");
    */
    // guarda matriz y U en archivos txt
    escribirMatriz_txt("sistA2_L.txt",matriz,m,n);
    escribirMatriz_txt("sistA2_U.txt",U,m,n);

    res = MxM(matriz,U,n);
    printf("Producto LU:\n");
    for (int i=0;i<m;i++){
        for (int j=0;j<m;j++)
            printf("%g ",res[i][j]);
        
        printf("\n");
    }
    printf("\n");

    y = x_inferior(matriz,vector,n);

    vector = x_superior(U,y,n);
    printf("Solucion al sistema:\n");
    for (int i=0;i<n;i++){
        printf("%g ", vector[i]);
    }
    printf("\n");
    escribirVector_txt("sistA2_sol.txt",vector,n);

    // liberando la memoria dinamica
    free_matrix(matriz);
    free_matrix(U);
    free_matrix(res);
    free(vector);
    free(y);

    return 0;
}
