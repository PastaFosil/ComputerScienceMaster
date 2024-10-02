#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

#define TOL 0.0001
#define N 50000

int main(){
    int n = 2000;
    double **mtx = genMatriz_double(n,n);
    double **U = genMatriz_double(n,n);
    double *b = genVector_double(n);
    double *x1, *x2; 
    if (mtx==NULL||U==NULL||b==NULL){
    //if (mtx==NULL||b==NULL){
        free_matrix(mtx);
        free_matrix(U);
        free(b);
        return -1;
    }
    
    for (int i=0;i<n;i++){
        if (i-2>=0)
            mtx[i][i-2] = -4.0;
        if (i-1>=0)
            mtx[i][i-1] = -8.0;
        mtx[i][i] = 40.0;
        if (i+1<n)
            mtx[i][i+1] = -8.0;
        if (i+2<n)
            mtx[i][i+2] = -4.0;
        
        if (i==0||i==1999)
            b[i] = 20.0;
        else if (i==1||i==1998)
            b[i] = 50.0;
        else
            b[i] = 100.0;
    }

    /*
    x1 = solve_gauss_seidel(mtx,b,n,N,TOL);
    if (x1==NULL){
        free_matrix(mtx);
        free_matrix(U);
        free(b);
        return -1;
    }
    printf("Gauss Seidel\n");
    print_vector(x1,n);
    
    factor_crout(mtx,U,n);
    x2 = solve_upper_lower(U,mtx,b,n);
    if (x1==NULL){
        free_matrix(mtx);
        free_matrix(U);
        free(b);
        free(x2);
        return -1;
    }print_matrix(mtx,n,n);
    printf("LU\n");
    print_vector(x2,n);
    */

    

    free_matrix(mtx);
    free_matrix(U);
    free(b);
    free(x1);
    free(x2);
    
}