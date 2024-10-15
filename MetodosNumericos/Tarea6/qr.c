/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5
 * 
 * Encuentra los valores propios de mayor magnitud de una 
 * matriz junto con sus vectores propios asociados
************************************************************/

#include <stdio.h>
#include <string.h>
#include "../Tools/tools.c"

void factor_qr(double **A, int rows, int cols, double ***Q, double ***R){
    if (A==NULL){
        printf("QR. MATRIZ VACIA\n");
        return;
    }

    // se asigna espacio para Q y R
    *Q = genMatriz_double(rows,cols);
    *R = genMatriz_double(rows,cols);
    if (Q==NULL || R==NULL){
        printf("QR. ERROR ASIGNANDO MEMORIA\n");
        return;
    }

    // llenado de la matriz Q
    double *temp = genVector_double(rows);
    for (int i=1;i<cols;i++){
        for (int j=0;j<rows;j++)
            temp[j] = A[j][i];
        unit_vector(&temp, rows);
        for (int j=0;j<rows;j++)
            (*Q)[j][i] = temp[j];
    }
    free(temp);

    double q[rows], a[rows];
    for (int i=0;i<rows;i++)
        for (int j=0;j<cols;j++){
            for (int k=0;k<rows;k++)
                q[k] = (*Q)[k][j];
            for (int k=0;k<rows;k++)
                a[k] = A[k][i];
            (*R)[j][i] = point_product(q,a,rows);
        }
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s matriz.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    if(mtx==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        free_matrix(mtx);
        return -1;
    }

    double **Q, **R, *x;
    char *out_file = "sol_";
    strcat(out_file, argv[1]);

    factor_qr(mtx,m,n,&Q,&R);
    if (Q==NULL || R==NULL){
        printf("ALGO SALIO MAL\n");
        return -1;
    }

    printf("Q\n");
    print_matrix(Q,m,n);
    printf("R\n");
    print_matrix(R,m,n);

    return 0;
}