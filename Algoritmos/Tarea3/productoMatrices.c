/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 3

El programa toma un numero arbitrario de archivos de
matrices cuadradas y devuelve el producto de todas ellas

Para ejecutar:
./productoMatrices mat1.txt mat2.txt ...

Estructura de archivo de matriz:
dim
a11 a12 ... a1[dim]
a21 a22 ... a2[dim]
... ...     ...
a[dim]1 a[dim]2 ... a[dim][dim]
*/

#include <stdio.h>
#include <stdlib.h>

double **create_matrix(int n){
    double **mtx, *elements;
    mtx = (double **)malloc(n*sizeof(double *));
    if (mtx==NULL){
        puts("NO SE PUDO ASIGNAR MEMORIA :(\n)");

        return NULL;
    }

    elements = (double *)calloc(n*n,sizeof(double));
    if (elements==NULL){
        puts("NO SE PUDO ASIGNAR MEMORIA :(\n)");
        free(mtx);
        return NULL;
    }

    for (int i=0;i<n;i++){
        mtx[i] = elements + i*n;
    }

    return mtx;
}

double **read_matrix(char *name, int *n){
    // abre archivo en modo lectura
    FILE *file = fopen(name, "r"); 
    if (file==NULL){
        puts("NO SE PUDO ABRIR EL ARCHIVO :(\n");
        return NULL;
    }

    // lee las dimensiones de la matriz
    fscanf(file, "%d",n);

    // asigna espacio para la matriz
    double **mtx = create_matrix(*n);
    if (mtx==NULL){
        fclose(file);
        return NULL;
    }

    // llena matriz
    for (int i=0;i<*n;i++){
        for (int j=0;j<*n;j++){
            fscanf(file, "%lf", &mtx[i][j]);
        }
    }

    // cierra archivo
    fclose(file);

    return mtx;
    
}

double **MxM(double **mtz1, double **mtz2, int n){
    /*
        Multiplica dos matrices y devuelve el resultado
    */

    double **res = create_matrix(n);

    for (int i=0;i<n;i++){
        for (int j=0;j<n;j++){
            for (int k=0;k<n;k++){
                res[i][j] += mtz1[i][k]*mtz2[k][j];
            }
        }
    }

    return res;
}

void free_matrix(double **mtx){
    if (mtx!=NULL){
        free(mtx[0]);
        free(mtx);
    }
}

int main(int argc, char **argv){
    int n, m; // dimensiones de las matrices leidas
    double **res; // apuntador a matriz de resultado

    // lee primera matriz
    double **mtz1 = read_matrix(argv[1],&n);

    // multiplica dos matrices a la vez hasta multiplicar todas las ingresadas
    for (int i=2;i<argc;i++){
        double **mtz2 = read_matrix(argv[i], &m); // lee siguiente matriz
        if (n!=m){ // si las matrices no tienen las mismas dimensiones, devuelve error
                puts("LAS MATRICES NO SE PUEDEN MULTIPLICAR :(\n");
                return -1;
            }

        if (i>2){ // si ya se multiplicaron las primeras dos matrices, se guarda el resultado en el arreglo de mtz1
            free(mtz1); // libera la informacion de mtz1
            mtz1 = res; // y guarda el resultado anterior
        } 
        res = MxM(mtz1,mtz2,n); // realiza el producto de las dos matrices almacenadas
        free_matrix(mtz2); // libera la memoria de mtz2
    }
    
    for (int i=0;i<n;i++){ // imprime el resultado
        for (int j=0;j<n;j++){
            printf("%g ", res[i][j]);
        }
        printf("\n");
    }

    free_matrix(mtz1);
    free_matrix(res);


    return 0;
}
