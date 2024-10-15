/*****************************************
        Metodos Numericos
        2024/09/22
        Juan Carlos Perez Ramirez
        Funciones de metodos numericos
*****************************************/

#include "tools.h"

#define N 1000000
#define TOL 0.000001

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
// IMPRESION Y COPIA DE MATRIZ Y VECTOR

// Imprime la matriz de dimensiones m x n
void print_matrix(double **mtx, int m, int n){
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++)
            printf("%g ", mtx[i][j]);
        printf("\n");
    }
}

// Imprime el vector vec de dimension m
void print_vector(double *vec, int n){
    for (int i=0;i<n;i++)
        printf("%g ", vec[i]);
    printf("\n");
}

// Copia los contenidos de la matriz cuadrada B en A
void copy_matrix(double **A, double **B, int m, int n){
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++)
            A[i][j] = B[i][j];
    }
}

void copy_vector(double *u, double *v, int n){
    for (int i=0;i<n;i++)
        u[i] = v[i];
}

/********************************************************************************************/
// OPERACIONES BASICAS ALGEBRA LINEAL

// producto punto
double dot_product(double *u, double *v, int n){
    double res = 0;
    for (int i=0;i<n;i++)
        res += u[i]*v[i];
    return res;
}

// producto matriz por matriz
double **MxM(double **A, int Arows, int Acols, double **B, int Brows, int Bcols){

    if (Acols!=Brows){
        printf("MxM. NO SE PUEDE REALIZAR EL PRODUCTO\n");
        return NULL;
    }

    double **res = genMatriz_double(Arows,Bcols);
    if (res==NULL){
        printf("MxM. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<Arows;i++){
        for (int j=0;j<Bcols;j++){
            for (int k=0;k<Acols;k++){
                res[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    
    return res;
}

// producto matriz por vector
double *MxV(double **mtz, double *vec, int n){
    /*
        Multiplica dos matrices cuadradas de igual dimension 
        y devuelve el resultado
    */

    double *res = genVector_double(n);
    if (res==NULL){
        printf("MxV. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++)
            res[i] += mtz[i][j]*vec[j];
    }
    
    return res;
}

// Norma de un vector
double norm(double *v, int n){
    double res;
    res = dot_product(v,v,n);
    return sqrt(res);
}

// Convierte al vector ingresado en unitario
void unit_vector(double **v, int n){
    double norm_v = norm(*v,n);
    for (int i=0;i<n;i++)
        (*v)[i] /= norm_v;
}

void *proyection(double *a, double *u, double **v, int n){
    copy_vector(*v,u,n);
    double factor = dot_product(a,u,n)/dot_product(u,u,n);

    for (int i=0;i<n;i++)
        (*v)[i] *= factor;
}

// Trasposicion de matriz
void traspose(double **mtx, int n){
    double aux;
    for (int i=0;i<n;i++){
        for (int j=0;j<i;j++){
            aux = mtx[i][j];
            mtx[i][j] = mtx[j][i];
            mtx[j][i] = aux;
        }
    }
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

// Resolucion de sistema lineal descrito por matriz triangular inferior
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

double *solve_diagonal(double *matriz, double *vector, int n){
    double *sol = genVector_double(n);
    // Operacion sobre los elementos de la diagonal para obtener el valor de las variables
    for(int i = 0; i<n; i++){
        if (vector[i]!=0){
            sol[i] /= matriz[i];
        } else {
            printf("NO SE PUEDE RESOLVER EL SISTEMA (DIV. 0)\n");
            return NULL;
        }
    }

    return sol;
}

double *solve_upper_lower(double **U, double **L, double *vector, int n){
    double *y = solve_lower(L,vector,n);
    double *res = solve_upper(U,y,n);
    free(y);
    return res;
}

double *solve_lower_diagonal_upper(double **L, double *D, double **LT, double *vector, int n){
// calculo de vector transitorio y (Ly=b)
    double *y1 = solve_lower(L,vector,n);
    if (y1==NULL){
        printf("SOLVE LDU. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    // calculo de vector transitorio z (Dz=y)
    double *y2 = solve_diagonal(D,y1,n);
    if (y2==NULL){
        printf("SOLVE LDU. ERROR ASIGNANDO MEMORIA\n");
        free(y1);
        return NULL;
    }
    free(y1);
    // calculo y almacenamiento del vector solucion
    y1 = solve_upper(LT,y2,n);
    free(y2);

    return y1;
}

/********************************************************************************************/
// METODOS DE FACTORIZACION

// Factoriza la matriz ingresada por el metodo de Cholesky LLT
void factor_LLT(double **matriz, double **LT, int n){
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
        printf("\nNO PUEDE REALIZARSE LA FACTORIZACION DE LA MATRIZ\n");
        free_matrix(LT);
        return;
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
}

// Factoriza la matriz ingresada por el metodo de Cholesky LLT
void factor_LDLT(double **matriz, double *D, double **LT, int n){
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
        printf("\nNO PUEDE REALIZARSE LA FACTORIZACION DE LA MATRIZ\n");
        return;
    }

    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for (int i=0; i<n; i++){ // recorre los renglones de inicio a final
        for (int j=0;j<=i;j++){ // recorre las columnas de la matriz triangular inferior
            if (i==j){ // elementos de la diagonal
                D[i] = matriz[i][j];
                matriz[i][j] = 1.0;
                for (int k=0;k<i;k++)
                    D[i] -= matriz[i][k]*matriz[i][k]*D[k];
            } else { // elementos fuera de la diagonal
                matriz[i][j] = matriz[j][i];
                for (int k=0;k<j;k++){
                    matriz[i][j] -= matriz[j][k]*matriz[i][k]*D[k];
                }
                matriz[i][j] /= D[j];
                matriz[j][i] = 0.0; // se anulan elementos arriba de la diagonal
            }
            LT[j][i] = matriz[i][j]; // se asignan valores de la matriz traspuesta
        }
    }
}

void factor_crout(double **mtx, double **U, int n){
    // Operacion sobre los elementos de la matriz para obtener el valor de las variables
    for (int j = 0; j<n; j++){ // recorre las columnas de inicio a final
        for (int i=0;i<=j;i++){ // recorre los renglones de la matriz triangular superior (los de la inferior se recorren por reflejo)
            // primer termino de cada elemento de U (dado que matriz guardara L, este paso esta implicito para L)
            U[i][j] = mtx[i][j];
            
            // recorre los elementos involucrados en la sumatoria
            for (int k=0;k<i;k++)
                U[i][j] -= mtx[i][k]*U[k][j];
            for (int k=0;k<i;k++)
                mtx[j][i] -= mtx[j][k]*U[k][i];
            mtx[j][i] /= U[i][i];
            if (i!=j)
                mtx[i][j] = 0.0; // anula los elementos de matriz arriba de la diagonal y divide elementos de matriz entre los correspondientes elementos de U
        }
    }
}

/********************************************************************************************/
// METODOS ITERATIVOS

// Resuelve un sistema de ecuaciones por el metodo de Gauss-Seidel
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
                free(x0);
                free(x_new);
                free(temp);
                return NULL;
            }
                
            x_new[j] /= matrix[j][j];
            
            // calculo de componentes de vector temporal (se usara para comprobar precision del resultado)
            temp[j] = x_new[j]-x0[j];
        }
        
        if (norm(temp,n)<TOL){
            free(x0);
            free(temp);
            return x_new;
        }
        for (int j=0;j<n;j++)
            x0[j] = x_new[j];
    }

    printf("NUMERO MAXIMO DE ITERACIONES EXCEDIDO\n");
    free(x0);
    free(x_new);
    free(temp);
    return NULL;
}

// Resuelve un sistema de ecuaciones por el metodo de Jacobi
double *solve_jacobi(double **matrix, double *vector, int n){
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
            for (int k=0;k<n;k++)
                if (k!=j)
                    x_new[j] -= matrix[j][k]*x0[k];
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


/********************************************************************************************/
// CALCULO DE VALORES Y VECTORES PROPIOS

// Elimina la contribucion de los k vectores ortonormales de vec
void substract_contribution(double *vec, int n, double **eigenvectors, int k){
    double a;
    // elimina la contribucion de cada vector propio
    for (int i=0;i<k;i++){
        a = dot_product(eigenvectors[i],vec,n); // factor de la contribucion del vector propio i
        for (int j=0;j<n;j++) // resta contribucion de vector propio i
            vec[j] -= a*eigenvectors[i][j];
    }
}

//-------------------------------------------------------------------------------------------
// POTENCIA

// Encuentra el k-esimo mayor valor propio y su vector asociado de la matriz A dados los k-1 anteriores
int power_iteration(double **mtx, int n, double **eigenvectors, int k){
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
    for (int i=0;i<N;i++){
        if (found==1) // si se han encontrado otros vectores propios, se resta su contribucion a la aproximacion
            substract_contribution(x0,n,eigenvectors,k);
        x1 = MxV(mtx,x0,n); // calculo de la siguiente aproximacion del vector propio
        deno = dot_product(x1,x0,n); // calculo del denominador
        if (deno!=0)
            lambda = dot_product(x1,x1,n)/deno; // calculo de la siguiente aproximacion del valor propio
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
            printf("%d ITERACIONES\n",i);
            return 0;
        }
        lambda0 = lambda; // actualizacion de valores aproximacion
    }

    printf("POWER. NO SE ENCONTRO UNA SOLUCION SATISFACTORIA\n");
    return -1;
}

// Encuentra los k mayores valores propios y sus vectores asociados de la matriz A
double **power_iteration_generalized(double **mtx, int n, int k){
    double **eigenstuff = genMatriz_double(n,n+1); // arreglo que guardara valores y vectores propios
    int result;

    if(eigenstuff==NULL){
        printf("POWER GENERALIZED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(eigenstuff);
        return NULL;
    }

    // Iteraciones para encontrar los k valores y vectores propios
    for (int i=0;i<k;i++){
        result = power_iteration(mtx, n, eigenstuff,i);
        if (result==-1){
            printf("POWER GENERALIZED. ALGO SALIO MAL\n");
            return NULL;
        }
    }
    if (result==0)
        return eigenstuff;
    else{
        free_matrix(eigenstuff);
        return NULL;
    }
}

//-------------------------------------------------------------------------------------------
// POTENCIA INVERSA

// Encuentra el k-esimo menor valor propio y su vector asociado de la matriz descompuesta en A=LU dados los k-1 anteriores
//int inverse_power_iteration(double **mtx, int n, double **eigenvectors, int k){
/*int inverse_power_iteration(double **L, double **U, int n, double **eigenvectors, int k){
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
    for (int i=0;i<N;i++){
        if (found==1) // si se han encontrado otros vectores propios, se resta su contribucion a la aproximacion
            substract_contribution(x0,n,eigenvectors,k);
        //x1 = solve_gauss_seidel(mtx,x0,n); // actualiza el vector mediante Gauss-Seidel
        x1 = solve_upper_lower(U,L,x0,n);
        if (x1==NULL){
            printf("INVERSE POWER. ALGO SALIO MAL\n");
            return -1;
        }
        deno = dot_product(x1,x1,n);
        if (deno!=0)
            lambda = dot_product(x1,x0,n)/deno; // calculo de la siguiente aproximacion del valor propio
        else {
            printf("POWER. ERROR DIV. 0\n");
            free(x1);
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
            printf("%d ITERACIONES\n",i);
            print_vector(x0,n);
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
    int result;

    if(eigenstuff==NULL){
        printf("INVERSE POWER. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(eigenstuff);
        return NULL;
    }

    //factoriza la matriz por Cholesky LLT
    double **A = genMatriz_double(n,n);
    double **LT = genMatriz_double(n,n);
    if (result==-1){
            printf("INVERSE GENERALIZED. ALGO SALIO MAL\n");
            free_matrix(eigenstuff);
            free_matrix(LT);
            free_matrix(A);
            return NULL;
        }
    copy_matrix(A,mtx,n,n);
    factor_LLT(A,LT,n);

    printf("L\n");
    print_matrix(A,n,n);

    // iteraciones para encontrar los f valores y vectores propios
    for (int i=0;i<k;i++){
        //result = inverse_power_iteration(mtx,n,eigenstuff,i);
        result = inverse_power_iteration(A,LT,n,eigenstuff,k);
        if (result==-1){
            printf("INVERSE GENERALIZED. ALGO SALIO MAL\n");
            free_matrix(eigenstuff);
            free_matrix(LT);
            free_matrix(A);
            return NULL;
        }
    }

    free_matrix(LT);
    free_matrix(A);

    if (result==0)
        return eigenstuff;
    else{
        free_matrix(eigenstuff);
        return NULL;
    }
}*/

int inverse_power_iteration(double **L, double **U, double **x0, int n, double **eigenvectors, int k, int MAX){
//int inverse_power_iteration(double **L, double **U, int n, double **eigenvectors, int k){
    // revisa si ya se han encontrado otros valores propios
    int found = 0;
    if (k>0) // lista de vectores propios encontrados hasta el momento
        found = 1;
    
    //double x0[n], deno;
    double deno;
    double *x1;
    double lambda0 = 10000.0, lambda;
    
    // inicialiacion x0 normalizado
    //for (int i=0;i<n;i++)
        //x0[i] = 1/sqrtf(n);
    
    // iteraciones para encontrar el resultado
    for (int i=0;i<MAX;i++){
        if (found==1) // si se han encontrado otros vectores propios, se resta su contribucion a la aproximacion
            substract_contribution(*x0,n,eigenvectors,k);
        x1 = solve_upper_lower(U,L,*x0,n); // calculo de la siguiente aproximacion del vector propio
        deno = dot_product(x1,x1,n);
        if (deno!=0)
            lambda = dot_product(x1,*x0,n)/deno; // calculo de la siguiente aproximacion del valor propio
        else {
            printf("POWER. ERROR DIV. 0\n");
            return -1;
        }
        unit_vector(&x1,n); // normalizacion del vector aproximacion
        for (int j=0;j<n;j++) // actualizacion de los vectores aproximacion
            (*x0)[j] = x1[j];
        free(x1);
        if (fabs(lambda-lambda0)/fabs(lambda)<TOL){ // almacenamiento y devolucion de resultados satisfactorios
            for (int j=0;j<n;j++)
                eigenvectors[k][j] = (*x0)[j];
            eigenvectors[k][n] = lambda;
            printf("%d ITERACIONES\n",i);
            return 0;
        }
        lambda0 = lambda; // actualizacion de valores aproximacion
    }

    //printf("INVERSE POWER. NO SE ENCONTRO UNA SOLUCION SATISFACTORIA\n");
    //return -1;
    for (int j=0;j<n;j++)
        eigenvectors[k][j] = (*x0)[j];
    eigenvectors[k][n] = lambda;
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
    //factor_crout(mtx,U,n);
    factor_LLT(mtx,U,n);

    // iteraciones para encontrar los f valores y vectores propios
    for (int i=0;i<k;i++){
        double *x0 = genVector_double(n);
        for (int i=0;i<n;i++)
            x0[i] = 1/sqrt(n);
        result = inverse_power_iteration(mtx,U,&x0,n,eigenstuff,i,N);
    }

    free_matrix(U);

    if (result==0)
        return eigenstuff;
    else
        return NULL;
}

//-------------------------------------------------------------------------------------------
// JACOBI

// Rota la matriz ingresada a un angulo theta
void rotate_mtx(double **A, int n, int *pq, double cosTheta, double sinTheta){
    double Aip,Aiq,App,Aqq,Apq;
    for (int i=0;i<n;i++){
        if (i!=pq[0] && i!=pq[1]){
            Aip = A[i][pq[0]];
            Aiq = A[i][pq[1]];

            A[i][pq[0]] = cosTheta*Aip - sinTheta*Aiq;
            A[i][pq[1]] = cosTheta*Aiq + sinTheta*Aip;
            A[pq[0]][i] = A[i][pq[0]];
            A[pq[1]][i] = A[i][pq[1]];
        }
    }

    App = A[pq[0]][pq[0]];
    Aqq = A[pq[1]][pq[1]];
    Apq = A[pq[0]][pq[1]];

    A[pq[0]][pq[0]] = cosTheta*cosTheta*App + sinTheta*sinTheta*Aqq - 2.0*sinTheta*cosTheta*Apq;
    A[pq[1]][pq[1]] = cosTheta*cosTheta*Aqq + sinTheta*sinTheta*App + 2.0*sinTheta*cosTheta*Apq;
    A[pq[0]][pq[1]] = 0.0;
    A[pq[1]][pq[0]] = 0.0;
}

// Raiz cuadrada de la suma de los cuadrados de los elementos fuera de la diagonal (matriz simetrica)
double off(double **mtx, int n){
    double sum = 0.0;
    for (int i=0;i<n;i++)
        for (int j=0;j<i;j++)
            sum += 2*mtx[i][j]*mtx[i][j];
    return sqrt(sum);
}

// Indices del elemento con el mayor valor absoluto
int *greatest_off_diag(double **mtx, int n){
    int *pq = (int *)calloc(2,sizeof(int));
    double max = fabs(mtx[0][1]);

    if (pq==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    pq[0] = 0;
    pq[1] = 1;

    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){
            if (max<fabs(mtx[i][j])){
                max = fabs(mtx[i][j]);
                pq[0] = i;
                pq[1] = j;
            }
        }
    }
    
    if (max<TOL){
        pq[0] = -1;
        pq[1] = -1;
    }

    return pq;
}

// Generacion de matriz de Jacobi
double **jacobi_mtx(int n, int *pq, double cosTheta, double sinTheta){
    double **J = genMatriz_double(n,n);
    if (J==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<n;i++)
        J[i][i] = 1.0;

    J[pq[0]][pq[0]] = cosTheta;
    J[pq[1]][pq[1]] = cosTheta;
    J[pq[0]][pq[1]] = sinTheta;
    J[pq[1]][pq[0]] = -sinTheta;

    return J;
}

// Encuentra los valores propios ( diag(A) ) y vectores propios (cols T) de A
double **jacobi_eigen_method(double **A, int n, int MAX){
    int *pq; // indices del mayor elemento
    double **J; // matriz de Jacobi para la iteracion actual
    double **T = genMatriz_double(n,n);
    double theta, t, cosTheta, sinTheta; // parametros para calcular angulo de rotacion
    if (T==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    
    for (int i=0;i<MAX;i++){
        pq = greatest_off_diag(A,n);
        if (pq==NULL){
            printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
            return NULL;
        }
        
        if (pq[0]==-1){
            free(pq);
            return T;
        }

        double y = A[pq[0]][pq[0]]-A[pq[1]][pq[1]];
        if(fabs(y)<TOL)
            cosTheta = sinTheta = 1 / sqrt(2);
        else {
            double x = 2*A[pq[0]][pq[1]];
            double z = sqrt(x*x + y*y);
            cosTheta = sqrt((z+y)/(2*z));
            sinTheta = sqrt((z-y)/(2*z));
            sinTheta = (x/y<0)? sinTheta : -sinTheta;
        }
        
        J = jacobi_mtx(n,pq,cosTheta,sinTheta);
        if (J==NULL){
            free(pq);
            free_matrix(T);
            return NULL;
        }

        double **newA = MxM(J,n,n,A,n,n);
        if (newA==NULL){
            free(pq);
            free_matrix(T);
            free_matrix(J);
            return NULL;
        }
        copy_matrix(A,newA,n,n);
        free_matrix(newA);
        traspose(J,n);
        if (i==0)
            copy_matrix(T,J,n,n);
        else {
            double **newT = MxM(T,n,n,J,n,n);
            if (newT==NULL){
                free(pq);
                free_matrix(T);
                free_matrix(J);
                free_matrix(newA);
                return NULL;
            }
            copy_matrix(T,newT,n,n);
            free_matrix(newT);
        }
        //rotate_mtx(A,n,pq,cosTheta,sinTheta);
        newA = MxM(A,n,n,J,n,n);
        if (newA==NULL){
            free(pq);
            free_matrix(T);
            free_matrix(J);
            return NULL;
        }
        copy_matrix(A,newA,n,n);

        free_matrix(newA);
        free_matrix(J);
        
        free(pq);

        if (off(A,n)<TOL)
            return T;
    }

    printf("NO SE ENCONTRO UNA SOLUCION SATISFACTORIA\n");
    free_matrix(T);
    return NULL;
}
