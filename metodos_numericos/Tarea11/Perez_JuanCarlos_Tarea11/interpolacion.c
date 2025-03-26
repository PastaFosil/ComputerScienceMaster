/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 11
 * 
 * Calcula la funcion de interpolacion de un conjunto de
 * datos a partir del metodo de minimos cuadrados.
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
    double x = x_min;
    double h = (x_max-x_min)/100.0;
    for (int i=0;i<100;i++){
        double y = 0.0;
        for (int j=0;j<m;j++)
            y += w[j]*f(x, j);
        printf("%f\t%f\n", x, y);
        x += h;
    }
}

// Genera puntos de la curva de interpolacion
void least_squares_curve_extra(double x_min, double x_max, double (*f)(double, double), double *data, double *w, int m){
    double x = x_min;
    double h = (x_max-x_min)/100;
    for (int i=0;i<100;i++){
        double y = 0;
        for (int j=0;j<m;j++)
            y += w[j]*f(x, data[j]);
        printf("%f\t%f\n", x, y);
        x += h;
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

int main(int argc, char *argv[]){
    if (argc!=2){
        printf("USO: %s <opcion>\n", argv[0]);
        printf("Opciones:\n");
        printf("1. Polinomio de grado 3\n");
        printf("2. Coseno de pi x\n");
        //printf("3. Exponencial\n");
        return 0;
    }
    int n = 30, m = 5, choice = atoi(argv[1]);
    double x_values[n], y_values[n];
    double (*funcion)(double, int);
    for (int i=0;i<n;i++)
        x_values[i] = -6 + i*12.0/(n-1);

    double noise = 1.0; // ruido agregado a los datos generados

    funcion = cosnpix_6;
    for (int i=0;i<n;i++)
        y_values[i] = x_values[i] + pow(x_values[i], 2.0) + pow(x_values[i], 3.0) + pow(x_values[i], 4.0) + noise*((double)rand()/RAND_MAX-0.5);

    // Para interpolar datos con la funcion con la que fueron generados
    /*switch (choice){
        case 1: // polinomio
            funcion = polinomio;
            for (int i=0;i<n;i++)
                y_values[i] = x_values[i] + pow(x_values[i], 2.0) + pow(x_values[i], 3.0) + pow(x_values[i], 4.0) + noise*((double)rand()/RAND_MAX-0.5);
            break;
        case 2: // Coseno de 6pi x
            funcion = cosnpix_6;
            for (int i=0;i<n;i++)
                y_values[i] = cosnpix_6(x_values[i], 1) + cosnpix_6(x_values[i], 2) + noise*((double)rand()/RAND_MAX-0.5);
                //y_values[i] = sin(x_values[i]) + noise*((double)rand()/RAND_MAX-0.5);
            break;
        case 3: // Exponencial
            for (int i=0;i<n;i++)
                y_values[i] = exp_r(x_values[i], ) + 0.1*((double)rand()/RAND_MAX-0.5);
            funcion = exp_r;
            break;
        default:
            return 0;
    }*/

    // Para interpolar con polinomio y cosnpix_6
    double *w = least_squared_interpolation(x_values, y_values, n, m, funcion, 0.0);
    least_squares_curve(-6, 6, funcion, w, m);
    // Para interpolar con exp_r
    //double *w = least_squared_interpolation_extra(x_values, y_values, n, m, exp_r, 0.0);
    if (w==NULL){
        printf("ERROR CALCULANDO LA INTERPOLACION\n");
        return -1;
    }

    //least_squares_curve_extra(-6, 6, exp_r, x_values, w, m);
    //for (int i=0;i<n;i++)
        //printf("%f\t%f\n", x_values[i], y_values[i]);
    
    free(w);

    return 0;
}