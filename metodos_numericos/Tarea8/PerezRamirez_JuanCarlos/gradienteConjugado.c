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

double *solve_conjugate_gradient(double **A, double *b, double **r, int n){
    double *x = genVector_double(n);
    *r = genVector_double(n);
    double *w;
    double *v;
    double **invC = genMatriz_double(n,n);
    if (x==NULL || r==NULL || invC==NULL){
        printf("CONJUGATE GRAD. ERROR ASIGNANDO MEMORIA\n");
        free(x);
        free(*r);
        free_matrix(invC);
        return NULL;
    }
    double alpha;
    for (int i=0;i<n;i++)
        invC[i][i] = 1.0/A[i][i];
    copy_vector(*r,b,n);
    w = MxV(invC,*r,n);
    v = MxV(invC,w,n);
    if (w==NULL || v==NULL){
        free(x);
        free(*r);
        free_matrix(invC);
        return NULL;
    }
    alpha = norm(w,n);
    alpha *= alpha;
    int s = 0;
    for (int i=0;i<N;i++){
        s++;
        if (norm(v,n)<TOL){
            free(w);
            free(v);
            free_matrix(invC);
            return x;
        }

        double *u = MxV(A,v,n);
        if (u==NULL){
            free(x);
            free(w);
            free(v);
            free(*r);
            free_matrix(invC);
            return NULL;
        }
        double t = alpha/dot_product(u,v,n);
        for (int j=0;j<n;j++){
            x[j] += t*v[j];
            (*r)[j] -= t*u[j];
        }
        free(w);
        free(u);
        w = MxV(invC,*r,n);
        double beta = norm(w,n);
        beta *= beta;

        if (fabs(beta)<TOL && norm(*r,n)<TOL){
            free(w);
            free(v);
            free_matrix(invC);
            printf("%d ITERACIONES\n",s);
            return x;
        }

        double s = beta/alpha;
        double *temp = MxV(invC,w,n);
        if (temp==NULL){
            free(x);
            free(w);
            free(v);
            free(*r);
            free_matrix(invC);
            return NULL;
        }
        for (int j=0;j<n;j++)
            v[j] = s*v[j] + temp[j];
        free(temp);
        alpha = beta;
    }

    free(x);
    free(w);
    free(v);
    free_matrix(invC);
    return NULL;
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
    double *r;
    char out_file[30];
    strcpy(out_file,"sol_");
    strcat(out_file, argv[1]);

    double *x = solve_conjugate_gradient(mtx,vec,&r,n);
    if (x==NULL){
        printf("OCURRIO UN ERROR\n");
        free_matrix(mtx);
        free(vec);
        return -1;
    }
    char out_file2[32];
    escribirVector_txt(out_file,x,n);
    strcpy(out_file2,"r_");
    strcat(out_file2,out_file);
    escribirVector_txt(out_file2,r,n);


    free_matrix(mtx);
    free(vec);
    free(r);
    free(x);
    return 0;
}
