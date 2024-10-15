/*****************************************
        Metodos Numericos
        2024/09/22
        Juan Carlos Perez Ramirez
        Funciones de metodos numericos
*****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#ifndef TOOLS_H
#define TOOLS_H

/*Lectura de archivos*/
double **leerMatriz_txt(char *fileName, int *nRows, int *nCols);
double *leerVector_txt(char *fileName, int *nElements);

/*Escritura de archivos*/
void escribirMatriz_txt(char *fileName, double **matriz, int nRows, int nCols);
void escribirVector_txt(char *fileName, double *vector, int nElements);

/*Memoria dinamica*/
double **genMatriz_double(int nRows, int nCols);
double *genVector_double(int nElements);
void free_matrix(double **mtx);

/*Imprimir matriz o vector*/
void print_matrix(double **mtx, int m, int n);
void print_vector(double *vec, int n);
void print_vector(double *vec, int n);

/*Operaciones basicas de algebra lineal*/
double **MxM(double **A, int Arows, int Acols, double **B, int Brows, int Bcols);
double *MxV(double **mtz, double *vec, int n);
double dot_product(double *u, double *v, int n);
double norm(double *v, int n);
void unit_vector(double **v, int n);
void *proyection(double *a, double *u, double **v, int n);
void traspose(double **mtx, int n);

/*Resolucion de sistemas de ecuaciones*/
double *solve_lower(double **matriz, double *vector, int n);
double *solve_upper(double **matriz, double *vector, int n);
double *solve_diagonal(double *matriz, double *vector, int n);
double *solve_upper_lower(double **U, double **L, double *vector, int n);
double *solve_lower_diagonal_upper(double **L, double *D, double **LT, double *vector, int n);

/*Solucion por metodos de factorizacion*/
void factor_LLT(double **matriz, double **LT, int n);
void factor_LDLT(double **matriz, double *D, double **LT, int n);
void factor_crout(double **matriz, double **U, int n);

/*Solucion por metodos iterativos*/
double *solve_gauss_seidel(double **matrix, double *vector, int n);
double *solve_jacobi(double **matrix, double *vector, int n);

/*Calculo de valores y vectores propios*/
void substract_contribution(double *vec, int n, double **eigenvectors, int k);
int power_iteration(double **mtx, int n, double **eigenvectors, int k);
double **power_iteration_generalized(double **mtx, int n, int k);
//int inverse_power_iteration(double **mtx, int n, double **eigenvectors, int k);
int inverse_power_iteration(double **L, double **U, double **x0, int n, double **eigenvectors, int k, int MAX);
double **inverse_power_iteration_generalized(double **mtx, int n, int k);
void rotate_mtx(double **A, int n, int *pq, double cosTheta, double sinTheta);
double off(double **mtx, int n);
int *greatest_off_diag(double **mtx, int n);
double **jacobi_mtx(int n, int *pq, double cosTheta, double sinTheta);
double **jacobi_eigen_method(double **A, int n, int MAX);

#endif
