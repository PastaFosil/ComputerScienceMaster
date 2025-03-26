/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 5
 * 
 * Diagonaliza la matriz ingresada y calcula la matriz
 * de transformacion necesaria para lograrlo
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

#define TOL 0.000001
#define MAX_ITER 1000000

// Trasposicion de matriz
void traspose(double **mtx, int n){
    double aux;
    for (int i=0;i<n;i++){
        for (int j=0;j<i;j++){
            aux = mtx[i][j];
            mtx[i][j] = mtx[j][i];
            mtx[j][i] = aux;
        }
    }
}

// Generacion de matriz de Jacobi
double **jacobi_mtx(int n, int *pq, double cosTheta, double sinTheta){
    double **J = genMatriz_double(n,n);
    if (J==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<n;i++)
        J[i][i] = 1.0;

    J[pq[0]][pq[0]] = cosTheta;
    J[pq[1]][pq[1]] = cosTheta;
    J[pq[0]][pq[1]] = sinTheta;
    J[pq[1]][pq[0]] = -sinTheta;

    return J;
}

// Rota la matriz ingresada a un angulo theta
void rotate_mtx(double **A, int n, int *pq, double cosTheta, double sinTheta){
    double Aip,Aiq,App,Aqq,Apq;
    for (int i=0;i<n;i++){
        if (i!=pq[0] && i!=pq[1]){
            Aip = A[i][pq[0]];
            Aiq = A[i][pq[1]];

            A[i][pq[0]] = cosTheta*Aip - sinTheta*Aiq;
            A[i][pq[1]] = cosTheta*Aiq + sinTheta*Aip;
            A[pq[0]][i] = A[i][pq[0]];
            A[pq[1]][i] = A[i][pq[1]];
        } else {
            App = A[pq[0]][pq[0]];
            Aqq = A[pq[1]][pq[1]];
            Apq = A[pq[0]][pq[1]];

            A[pq[0]][pq[0]] = cosTheta*cosTheta*App + sinTheta*sinTheta*Aqq - 2.0*sinTheta*cosTheta*Apq;
            A[pq[1]][pq[1]] = cosTheta*cosTheta*Aqq + sinTheta*sinTheta*App + 2.0*sinTheta*cosTheta*Apq;
            A[pq[0]][pq[1]] = 0.0;
            A[pq[1]][pq[0]] = 0.0;
        }
    }
}

// Raiz cuadrada de la suma de los cuadrados de los elementos fuera de la diagonal (matriz simetrica)
double off(double **mtx, int n){
    double sum = 0.0;
    for (int i=0;i<n;i++)
        for (int j=0;j<i;j++)
            sum += 2*mtx[i][j]*mtx[i][j];
    return sqrt(sum);
}

// Indices del elemento con el mayor valor absoluto
int *greatest_off_diag(double **mtx, int n){
    int *pq = (int *)calloc(2,sizeof(int));
    double max = fabs(mtx[0][1]);

    if (pq==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<n;i++){
        for (int j=0;j<i;j++){
            if (max<fabs(mtx[i][j])){
                max = fabs(mtx[i][j]);
                pq[0] = i;
                pq[1] = j;
            }
            if (max<fabs(mtx[j][i])){
                max = fabs(mtx[j][i]);
                pq[0] = j;
                pq[1] = i;
            }
        }
    }

    return pq;
}

// Encuentra los valores propios ( diag(A) ) y vectores propios (cols T) de A
double **jacobi_eigen_method(double **A, int n){
    int *pq; // indices del mayor elemento
    double **J; // matriz de Jacobi para la iteracion actual
    double tau, t, cosTheta, sinTheta; // parametros para calcular angulo de rotacion
    double **T = genMatriz_double(n,n);
    if (T==NULL){
        printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    for (int i=0;i<n;i++)
        T[i][i] = 1.0;

    for (int i=0;i<MAX_ITER;i++){
        double **new_mtx; // matriz auxiliar para realizar calculos
        pq = greatest_off_diag(A,n);
        tau = (A[pq[1]][pq[1]]-A[pq[0]][pq[0]])/(2*A[pq[0]][pq[1]]); 
        // elige la raiz con el menor valor absoluto
        t = -tau-sqrt(1+tau*tau);
        if (fabs(-tau+sqrt(1+tau*tau))<fabs(t))
            t = -tau+sqrt(1+tau*tau);
        cosTheta = 1.0/(sqrt(1+t*t)); // coseno del angulo de rotacion theta
        sinTheta = t*cosTheta; // seno del angulo de rotacion theta
        
        rotate_mtx(T,n,pq,cosTheta,sinTheta);
        rotate_mtx(A,n,pq,cosTheta,sinTheta);
        /*
        J = jacobi_mtx(n, pq, cosTheta, sinTheta); // inicializacion de J
        
        // actualizacion de transformacion total
        new_mtx = MxM(T,J,n);
        if (new_mtx==NULL){
            printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
            free_matrix(J);
            free_matrix(T);
            free(pq);
            return NULL;
        }
        copy_matrix(T, new_mtx, n, n);
        free_matrix(new_mtx);
        
        // rotacion de A
        new_mtx = MxM(A,J,n);
        if (new_mtx==NULL){
            printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
            free_matrix(J);
            free_matrix(T);
            free(pq);
            return NULL;
        }
        traspose(J,n);
        double **new_mtx2 = MxM(J,new_mtx,n);
        if (new_mtx2==NULL){
            printf("JACOBI. ERROR ASIGNANDO MEMORIA\n");
            free_matrix(J);
            free_matrix(T);
            free_matrix(new_mtx);
            free(pq);
            return NULL;
        }
        copy_matrix(A,new_mtx2,n,n);
        free_matrix(new_mtx2);
        free_matrix(new_mtx);

        free_matrix(J);
        free(pq);
        */

        if (off(A,n)<TOL)
            return T;
    }

    printf("NO SE ENCONTRO UNA SOLUCION SATISFACTORIA\n");
    free_matrix(T);
    return NULL;
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s matriz.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double **T = jacobi_eigen_method(mtx,n, MAX_ITER,TOL);
    if (T==NULL){
        free_matrix(mtx);
        printf("NO SE ENCONTRO UN RESULTADO SATISFACTORIO\n");
        return -1;
    }
    int ones = 0, zeros = 0;
    printf("Valores propios:\n");
    for (int i=0;i<n;i++){
        if ((int)mtx[i][i]==1)
            ones++;
        if ((int)mtx[i][i]==0)
            zeros++;
        printf("%d ",(int)mtx[i][i]);
    }
    printf("\nunos=%d\n",ones);
    printf("ceros=%d\n",zeros);
    printf("\n\n");
    printf("Vectores propios:\n");
    //traspose(T,n);
    print_matrix(T,n,n);

    free_matrix(mtx);
    free_matrix(T);
    return 0;
}