/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 11
 * 
 * Calcula la interpolacion de la viscosidad a partir de la
 * funcion de Sutherland.
************************************************************/

#include <stdio.h>
#include <math.h>
#include "../Tools/tools.c"

#define PI 3.14159265358979323846

// Devuelve el arreglo de coeficientes que ajusta la funcion de interpolacion
double *least_squared_interpolation(double *x, double *y, int n, int m, double (*f)(double, int), double lambda){
    double **Phi = genMatriz_double(n, m);
    double **Phit = genMatriz_double(m, n);
    if (Phi==NULL || Phit==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        return NULL;
    }
    // inicializacion de matrices Phi y Phit
    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            Phi[i][j] = f(x[i], j);
            Phit[j][i] = Phi[i][j];
        }
    }

    // calculo de la matriz Phit*Phi
    double **A = MxM(Phit, m, n, Phi, n, m);
    if (A==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        return NULL;
    }
    for (int i=0;i<m;i++)
        A[i][i] += lambda;
    
    // inicializacion de vector Phit*y
    double *b = MxV(Phit, y, m, n);
    if (b==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        free_matrix(A);
        return NULL;
    }

    // calculo de la solucion del sistema Phit*Phi*w = Phit*y
    double *w = solve_qr(A, b, m);
    
    free_matrix(Phi);
    free_matrix(Phit);
    free_matrix(A);
    free(b);

    return w;
}

// Devuelve el arreglo de coeficientes que ajusta la funcion de interpolacion
double *least_squared_interpolation_extra(double *x, double *y, int n, int m, double (*f)(double, double), double lambda){
    double **Phi = genMatriz_double(n, m);
    double **Phit = genMatriz_double(m, n);
    if (Phi==NULL || Phit==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        return NULL;
    }
    // inicializacion de matrices Phi y Phit
    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            Phi[i][j] = f(x[i], x[j]);
            Phit[j][i] = Phi[i][j];
        }
    }

    // calculo de la matriz Phit*Phi
    double **A = MxM(Phit, m, n, Phi, n, m);
    if (A==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        return NULL;
    }
    for (int i=0;i<m;i++)
        A[i][i] += lambda;
    
    // inicializacion de vector Phit*y
    double *b = MxV(Phit, y, m, n);
    if (b==NULL){
        printf("LEAST SQUARED. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(Phi);
        free_matrix(Phit);
        free_matrix(A);
        return NULL;
    }

    // calculo de la solucion del sistema Phit*Phi*w = Phit*y
    double *w = solve_qr(A, b, m);
    
    free_matrix(Phi);
    free_matrix(Phit);
    free_matrix(A);
    free(b);

    return w;
}

// Genera la curva de la funcion de interpolacion
void least_squares_curve(double x_min, double x_max, double (*f)(double, int), double *w, int m){
    if (x_min!=x_max){
        double x = x_min;
        double h = (x_max-x_min)/100.0;
        for (int i=0;i<100;i++){
            double y = 0.0;
            for (int j=0;j<m;j++)
                y += w[j]*f(x, j);
            printf("%f\t%f\n", x, y);
            x += h;
        }
    } else {
        double y = 0.0;
        for (int j=0;j<m;j++)
            y += w[j]*f(x_min, j);
        printf("%g\t%g\n", x_min, y);
    }
}

// Genera puntos de la curva de interpolacion
void least_squares_curve_extra(double x_min, double x_max, double (*f)(double, double), double *data, double *w, int m){
    if (x_min!=x_max){
        double x = x_min;
        double h = (x_max-x_min)/100;
        for (int i=0;i<100;i++){
            double y = 0;
            for (int j=0;j<m;j++)
                y += w[j]*f(x, data[j]);
            printf("%f\t%f\n", x, y);
            x += h;
        }
    } else {
        double y = 0.0;
        for (int j=0;j<m;j++)
            y += w[j]*f(x_min, data[j]);
        printf("%g\t%g\n", x_min, y);
    }
}

double polinomio(double x, int n){
    return pow(x, n);
}

double cosnpix_6(double x, int n){
    return cos(n*PI*x/6);
}

double exp_r(double x, double xn){
    return exp(-(x-xn)*(x-xn));
}

double sutherland(double T, double m0, double T0, double Su){
    return m0*pow(1000.0*T/T0,3.0/2.0)*(T0+Su)/(1000.0*T+Su);
}

int main(int argc, char **argv){
    if (argc!=5){
        printf("USO: ./%s lambda funcion xmin xmax\n", argv[0]);
        printf("Opciones:\n");
        printf("1. Polinomio de grado n-1\n");
        printf("2. cos(n*pi*x/6)\n");
        return -1;
    }
    //double Ti[] = {0.273, 0.303, 0.323, 0.353, 0.423, 0.573, 1.473};
    double Ti[] = {0.1073, 0.1273, 0.273, 0.473, 0.673, 0.873, 1.473};
    int n = sizeof(Ti)/sizeof(Ti[0]);
    double y_values[n];
    double m0 = 1.919e-2, T0 = 273.0, Su = 139.0;
    double lambda = atof(argv[1]), m = 4;
    for (int i=0;i<n;i++)
        y_values[i] = sutherland(Ti[i], m0, T0, Su);
    //printf("f(1.2) = %g\n", sutherland(1.2,m0,T0,Su));
    //return 0;
    double *w, xmin = atof(argv[3]), xmax = atof(argv[4]);
    switch (atoi(argv[2])){
        case 1: 
            w = least_squared_interpolation(Ti, y_values, n, m, polinomio, lambda);
            if (w==NULL){
                printf("ERROR CALCULANDO LA INTERPOLACION\n");
                return -1;
            }
            least_squares_curve(xmin, xmax, polinomio, w, m);
            break;
        case 2:
            w = least_squared_interpolation(Ti, y_values, n, m, cosnpix_6, lambda);
            if (w==NULL){
                printf("ERROR CALCULANDO LA INTERPOLACION\n");
                return -1;
            }
            least_squares_curve(xmin, xmax, cosnpix_6, w, m);
            break;
        case 3:
            w = least_squared_interpolation_extra(Ti, y_values, n, m, exp_r, lambda);
            if (w==NULL){
                printf("ERROR CALCULANDO LA INTERPOLACION\n");
                return -1;
            }
            least_squares_curve_extra(xmin, xmax, exp_r, Ti, w, m);
            break;
        default:
            printf("OPCION NO VALIDA\n");
            return -1;
    }
    
    free(w);

    return 0;
}