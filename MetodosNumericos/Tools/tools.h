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

/*Operaciones basicas de algebra lineal*/
double point_product(double *u, double *v, int n);
double norm(double *v, int n);

/*Resolucion de sistemas de ecuaciones*/
double *solve_lower(double **matriz, double *vector, int n);
double *solve_upper(double **matriz, double *vector, int n);

/*Solucion por metodos de factorizacion*/
double *solve_choleskyLLT(double **matriz, double *vector, int n);

#endif