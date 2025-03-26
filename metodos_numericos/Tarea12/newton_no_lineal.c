/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 12
 * 
 * Resuelve sistemas de ecuaciones no lineales por el 
 * metodo de Newton
************************************************************/

#include <stdio.h>
#include <math.h>
#include "../Tools/tools.c"

#define PI 3.14159265358979323846

double five_midpoint(double *x, int i, double (*f)(double*), int n, double h){
    double result = 0.0;
    double x0[n];
    for (int j=0; j<n; j++)
        x0[j] = x[j];
    x0[i] -= 2*h;
    result += f(x0);
    x0[i] += h;
    result -= 8*f(x0);
    x0[i] += 2*h;
    result += 8*f(x0);
    x0[i] += h;
    result -= f(x0);

    return result/(12*h);
}

double **jacobian(double *x, double (*f[])(double*), int n, double h){
    double **J = genMatriz_double(n,n);
    if (J==NULL){
        printf("J. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            J[i][j] = five_midpoint(x, j, f[i], n, h);
    
    return J;
}

double *non_linear_newton(double *x0, double (*f[])(double*), int n, double h){
    double **J, **U = genMatriz_double(n,n);
    double *x = genVector_double(n);
    double *F = genVector_double(n);
    if (U==NULL || x==NULL || F==NULL){
        printf("J. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(U);
        free(x);
        free(F);
        return NULL;
    }
    copy_vector(x,x0,n);
    for (int i=0;i<N;i++){
        J = jacobian(x,f,n,h);
        if (J==NULL){
            printf("NL Newton. ALGO SALIO MAL\n");
            free(x);
            free(F);
            free_matrix(U);
            return NULL;
        }
        for (int j=0;j<n;j++)
            F[j] = -f[j](x);
        
        factor_crout(J,U,n);
        double *y = solve_upper_lower(U,J,F,n);
        if (y==NULL){
            printf("NL Newton. ALGO SALIO MAL\n");
            free(x);
            free_matrix(J);
            free_matrix(U);
            free(F);
            return NULL;
        }
        for (int j=0;j<n;j++)
            x[j] += y[j];
        if (norm(y,n)<TOL){
            free_matrix(U);
            free(y);
            free_matrix(J);
            free(F);
            return x;
        }
        free(y);
        free_matrix(J);
    }
    printf("NL Newton. NO SE ALCANZO UNA SOLUCION SATISFACTORIA\n");
    free_matrix(U);
    free(x);
    free(F);
    free_matrix(J);
    return NULL;
}

double f1(double *x){
    return 3.0*x[0]-cos(x[1]*x[2])-0.5;
}
double f2(double *x){
    return x[0]*x[0]-81.0*(x[1]+0.1)*(x[1]+0.1)+sin(x[2])+1.06;
}
double f3(double *x){
    return exp(-x[0]*x[1])+20.0*x[2]+(10.0*PI-3.0)/3.0;
}

double f1_e2a(double *x){
    return x[0]+x[1]-3.0;
}
double f2_e2a(double *x){
    return x[0]*x[0]+x[1]*x[1]-9.0;
}

int main(){
    //double x0[] = {0.1,0.1,-0.1};
    double x0[] = {1.0,5.0};
    int n = sizeof(x0)/sizeof(x0[0]);
    //double (*f[])(double*) = {f1,f2,f3};
    double (*f[])(double*) = {f1_e2a,f2_e2a};
    double *x = non_linear_newton(x0,f,n,1.0e-3);
    if (x==NULL){
        printf("ERROR ENCONTRANDO LA SOLUCION\n");
        return 1;
    }

    printf("SOLUCION\n");
    for (int i=0;i<n;i++)
        printf("%g ",x[i]);
    printf("\n");
    free(x);

    return 0;
}
