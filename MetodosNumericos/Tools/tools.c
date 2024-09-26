/*****************************************
        Metodos Numericos
        2024/09/22
        Juan Carlos Perez Ramirez
        Funciones de metodos numericos
*****************************************/

#include "tools.h"

/********************************************************************************************/
// LECTURA, ESCRITURA Y GENERACION DE MATRICES Y VECTORES

// Generacion de matriz de doble precision
double **genMatriz_double(int nRows, int nCols) {
    // Se solicita espacio para la matriz
    double **matriz, *temp;
    matriz = (double **)malloc(nRows * sizeof(double *)); // Espacio para los apuntadores

    // Se verifica que exista espacio
    if(matriz == NULL) {
        printf("Error de asignacion de memoria.\n");
        return NULL;
    }

    // Solicitamos la memoria contigua
    temp = (double *)calloc(nRows * nCols, sizeof(double));
    // Se verifica que exista espacio
    if(temp == NULL) {
        printf("Error de asignacion de memoria.\n");
        free(matriz); // Liberacion de memoria en caso de error
        return NULL;
    }

    // Asignacion de la memoria contigua a la matriz
    for(int i = 0; i < nRows; i++) {
        matriz[i] = &temp[i * nCols];
    }

    // Retornando la matriz
    return matriz;
}

// Lectura de matriz desde archivo de texto
double **leerMatriz_txt(char *fileName, int *nRows, int *nCols) {
    // Se accede al archivo en modo de lectura
    FILE *file = fopen(fileName, "r");

    // Verificar si el archivo se abrio correctamente
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    // Se leen las dimensiones de la matriz
    fscanf(file, "%d %d", nRows, nCols);

    // Se solicita espacio para la matriz
    double **matriz = genMatriz_double(*nRows, *nCols); // Se usan los valores desreferenciados

    // Verificar si la matriz se asigno correctamente
    if (matriz == NULL) {
        fclose(file);
        return NULL;
    }

    // Rellenando la matriz
    for(int i = 0; i < *nRows; i++) {
        for(int j = 0; j < *nCols; j++) {
            fscanf(file, "%lf", &matriz[i][j]);
        }
    }

    fclose(file);
    
    return matriz;
}

// Generacion de vector de doble precision
double *genVector_double(int nElements) {
    // Se solicita espacio para el vector
    double *vector;
    vector = (double *)calloc(nElements, sizeof(double)); // Uso de calloc para inicializar a 0

    // Se verifica que exista espacio
    if (vector == NULL) {
        printf("Error de asignacion de memoria.\n");
        return NULL;
    }

    // Retornando el vector
    return vector;
}

// Lectura de vector desde archivo de texto
double *leerVector_txt(char *fileName, int *nElements) {
    // Se accede al archivo en modo de lectura
    FILE *file = fopen(fileName, "r");

    // Verificar si el archivo se abrio correctamente
    if (file == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return NULL;
    }

    // Se lee la cantidad de elementos del vector
    fscanf(file, "%d", nElements);

    // Se solicita espacio para el vector
    double *vector = genVector_double(*nElements);

    // Verificar si el vector se asigno correctamente
    if (vector == NULL) {
        fclose(file);
        return NULL;
    }

    // Rellenando el vector
    for (int i = 0; i < *nElements; i++) {
        fscanf(file, "%lf", &vector[i]);
    }

    fclose(file);
    
    return vector;
}

// Escritura de matriz en archivo de texto
void escribirMatriz_txt(char *fileName, double **matriz, int nRows, int nCols){
    // Se abre el archivo en modo de escritura
    FILE *file = fopen(fileName, "w");

    // Verificar si el archivo se abrio correctamente
    if(file == NULL){
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }

    // Escribimos las dimensiones de la matriz en el archivo
    fprintf(file, "%d %d\n", nRows, nCols);

    // Escribimos los elementos de la matriz
    for(int i = 0; i < nRows; i++){
        for(int j = 0; j < nCols; j++){
            fprintf(file, "%lf ", matriz[i][j]);
        }
        fprintf(file, "\n"); // Nueva linea al final de cada fila
    }

    // Cerramos el archivo
    fclose(file);
}

// Escritura de vector en archivo de texto
void escribirVector_txt(char *fileName, double *vector, int nElements){
    // Se abre el archivo en modo de escritura
    FILE *file = fopen(fileName, "w");

    // Verificar si el archivo se abrio correctamente
    if(file == NULL){
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }

    // Escribimos el numero de elementos en el archivo
    fprintf(file, "%d\n", nElements);

    // Escribimos los elementos del vector
    for(int i = 0; i < nElements; i++){
        fprintf(file, "%lf\n", vector[i]);
    }

    // Cerramos el archivo
    fclose(file);
}

// Liberacion de la memoria asignada a matriz
void free_matrix(double **mtx){
    if (mtx!=NULL){
        free(mtx[0]);
        free(mtx);
    }
}

/********************************************************************************************/
// OPERACIONES BASICAS ALGEBRA LINEAL

double point_product(double *u, double *v, int n){
    double res = 0;
    for (int i=0;i<n;i++)
        res += u[i]*v[i];
    return res;
}

double norm(double *v, int n){
    double res;
    res = point_product(v,v,n);
    return sqrt(res);
}

/********************************************************************************************/
// RESOLUCION DE SISTEMAS DE ECUACIONES LINEALES


// resolucion de sistema lineal descrito por matriz triangular inferior
double *solve_lower(double **matriz, double *vector, int n){

    double *x = (double *)calloc(n, sizeof(double));
    if (x==NULL){
        printf("LOWER. NO SE PUDO ASIGNAR MEMORIA\n");
        free(x);
        
        return NULL;
    }

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
            printf("LOWER. NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return NULL;
        }
    }

    return x;
}

// resolucion de sistema lineal descrito por matriz triangular inferior
double *solve_upper(double **matriz, double *vector, int n){
    double *x = (double *)calloc(n, sizeof(double));
    if (x==NULL){
        printf("UPPER. NO SE PUDO ASIGNAR MEMORIA\n");
        free(x);
        
        return NULL;
    }

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
            printf("UPPER. NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return NULL;
        }
    }

    return x;
}

/********************************************************************************************/
// METODOS DE FACTORIZACION

double *solve_choleskyLLT(double **matriz, double *vector, int n){
    // Llamada a valores para leer los archivos
    double **LT = genMatriz_double(n,n); // matriz L traspuesta
    double *y = genVector_double(n); // vector que guardara Ly=b

    if (matriz==NULL||vector==NULL||y==NULL){
        printf("LL^T. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(LT);
        free(y);
        return NULL;
    }

    int positiva = 1;
    for (int i=0;i<n;i++){
        if (matriz[i][i]<=0)
            positiva = 0;
        for (int j=0;j<n;j++){
            if (matriz[i][j]!=matriz[j][i])
                positiva = 0;
        }
    }

    if (positiva==0){
        printf("\nLL^T. NO PUEDE REALIZARSE LA FACTORIZACION DE LA MATRIZ\n");
        free_matrix(LT);
        free(y);
        return NULL;
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

    // calculo de los vectores de transicion y solucion
    y = solve_lower(matriz,vector,n);
    vector = solve_upper(LT,y,n);

    return vector;
}