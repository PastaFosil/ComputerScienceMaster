/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 7
 * 
 * Resuelve un sistema de ecuaciones usando el metodo de 
 * factorizacion QR
************************************************************/

#include <stdio.h>
#include <string.h>
#include "tools.c"

void factor_qr(double **A, int rows, int cols, double ***Q, double ***R){
    if (A==NULL){
        printf("QR. MATRIZ VACIA\n");
        return;
    }

    // se asigna espacio para Q y R
    *Q = genMatriz_double(rows,cols);
    *R = genMatriz_double(rows,cols);
    double **U = genMatriz_double(rows,cols);
    if (*Q==NULL || *R==NULL || U==NULL){
        printf("QR. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(*Q);
        free_matrix(*R);
        free_matrix(U);
        return;
    }

    // llenado de las matrices U y Q
    double a[rows];
    double *u = genVector_double(rows);
    double *proy = genVector_double(rows);
    double *temp = genVector_double(rows);
    if (u==NULL || proy==NULL || temp==NULL){
        printf("QR. ERROR ASIGNANDO MEMORIA\n");
        free(u);
        free(proy);
        free(temp);
        return;
    }
    for (int i=0;i<cols;i++){ // calcula cada vector de U y Q
        for (int j=0;j<rows;j++){
            a[j] = A[j][i];
            u[j] = a[j];
        }
        for (int j=0;j<i;j++){// resta las contribuciones de las proyecciones de a en las u ya calculadas
            for (int k=0;k<rows;k++)
                temp[k] = U[k][j];
            proyection(a,temp,&proy,rows);
            for (int k=0;k<rows;k++)
                u[k] -= proy[k];
        }
        for (int j=0;j<rows;j++)
            U[j][i] = u[j];
        unit_vector(&u,rows);
        for (int j=0;j<rows;j++)
            (*Q)[j][i] = u[j];
    }
    free(u);
    free(proy);
    free(temp);
    free_matrix(U);

    //traspose(*Q,rows);
    double q[rows];
    for (int i=0;i<rows;i++){
        for (int j=0;j<=i;j++){
            for (int k=0;k<rows;k++)
                q[k] = (*Q)[k][j];
            for (int k=0;k<rows;k++)
                a[k] = A[k][i];
            (*R)[j][i] = dot_product(q,a,rows);
        }
    }
}

double *solve_qr(double **A, double *b, int n){
    double **Q, **R;

    factor_qr(A,n,n,&Q,&R);
    if (Q==NULL || R==NULL){
        printf("ALGO SALIO MAL\n");
        free_matrix(Q);
        free_matrix(R);
        return NULL;
    }
    traspose(Q,n);
    double *new_b = MxV(Q,b,n);
    if (new_b==NULL){
        printf("ALGO SALIO MAL\n");
        free_matrix(Q);
        free_matrix(R);
        return NULL;
    }
    double *x = solve_upper(R,new_b,n);
    if (x==NULL){
        printf("ALGO SALIO MAL\n");
        free_matrix(Q);
        free_matrix(R);
        free(new_b);
        return NULL;
    }

    free_matrix(Q);
    free_matrix(R);
    free(new_b);

    return x;
}

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: ./%s matriz.txt vector.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double *vec = leerVector_txt(argv[2],&n);
    if(mtx==NULL || vec==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        free_matrix(mtx);
        free(vec);
        return -1;
    }

    char out_file[30];
    strcpy(out_file,"sol_");
    strcat(out_file, argv[1]);

    double *x = solve_qr(mtx,vec,n);

    escribirVector_txt(out_file,x,n);

    free_matrix(mtx);
    free(vec);
    free(x);

    return 0;
}
