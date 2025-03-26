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
#include "../Tools/tools.c"

// Aplica el metodo de Gram-Schmidt a las columnas de A
void gram_schmidt(double ***A, int rows, int cols){
    double *temp = genVector_double(rows);
    double *proy = genVector_double(rows);
    double a[rows];
    for (int i=0;i<cols;i++){
        for (int j=0;j<rows;j++)
            a[j] = (*A)[j][i];
        for (int j=0;j<i;j++){// resta las contribuciones de las proyecciones de a en las u ya calculadas
            for (int k=0;k<rows;k++)
                temp[k] = (*A)[k][j];
            proyection(a,temp,&proy,rows);
            for (int k=0;k<rows;k++)
                (*A)[k][i] -= proy[k];
        }
    }
    free(temp);
    free(proy);
}

/*
Encuentra los k menores valores y vectores propios de la matriz A de n x n.
Los valores propios se almacenan en el vector lambda, y se devuelven los 
vectores propios como las columnas en una matriz .
*/
double **subspace_iteration(double **A, double **lambda, int n, int k, double shift){
    double **phi = genMatriz_double(n,k); // matriz de vectores propios
    double **phiT = genMatriz_double(k,n+1); // matriz de vectores propios traspuesta
    (*lambda) = genVector_double(k); // valores propios
    double **A_copy = genMatriz_double(n,n); // almacenara matriz triangular inferior
    double **U = genMatriz_double(n,n); // almacenara matriz triangular superior
    double **Q; // matriz transformada en el subespacio
    double **Z = genMatriz_double(k,k); // matrix de transformacion de Jacobi
    double *temp = genVector_double(k); // vector para evaluar convergencia
    if (phi==NULL || (*lambda)==NULL || A_copy==NULL || U==NULL || Z==NULL || temp==NULL){
        printf("SUBSPACE. ERROR ASIGNANDO MEMORIA\n");
        free_matrix(phi);
        free((*lambda));
        free_matrix(A_copy);
        free_matrix(U);
        free_matrix(Z);
        free(temp);
        return NULL;
    }

    copy_matrix(A_copy,A,n,n);
    factor_crout(A_copy,U,n); // factorizacion de A en triangular superior e inferior

    // inicializacion de phi y lambda0 (primera proposicion de valores propios)
    srand(1);
    for (int i=0;i<n;i++){
        for (int j=0;j<k;j++){
            phi[i][j] = (double)rand()/(double)RAND_MAX;
            (*lambda)[j] = 10000.0;
        }
    }
    printf("\nphi\n");
    print_matrix(phi,n,k);
    
    for (int l=0;l<N;l++){
        for (int i=0;i<k;i++){
            double *temp_vec = genVector_double(n);
            for (int j=0;j<n;j++)
                temp_vec[i] = phi[j][i];
            if (inverse_power_iteration(A_copy,U,&temp_vec,n,phiT,k,5)==-1){
                printf("SUBSPACE. ALGO SALIO MAL\n");
                return NULL;
            }
            for (int j=0;j<n;j++)
                phi[j][i] = phiT[i][j];
        }
        double **temp2 = MxM(A,n,n, phi,n,k);
        Q = MxM(phiT,k,n, temp2,n,k);

        Z = jacobi_eigen_method(Q,k,5); // aplica Jacobi a T
        for (int i=0;i<k;i++){ // actualizacion de vector para evaluar convergencia
            temp[i] = Q[i][i] - (*lambda)[i];
            (*lambda)[i] = Q[i][i];
        }
        if (norm(temp,k)<TOL){
            free_matrix(phiT);
            free_matrix(A_copy);
            free_matrix(Q);
            free_matrix(Z);
            free(temp);
            return phi;
        }
        
        phi = MxM(phi,n,k, Z,k,k);
        gram_schmidt(&phi,n,k);
        
        free_matrix(Q);
    }

    printf("SUBSPACE. NO SE ENCONTRO UN RESULTADO SATISFACTORIO\n");
    free_matrix(phiT);
    free_matrix(A_copy);
    free_matrix(Q);
    free_matrix(Z);
    free(temp);
    return NULL;
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s matriz.txt\n", argv[0]);
        return -1;
    }

    int m,n;
    double **mtx = leerMatriz_txt(argv[1],&m,&n);
    double *lambda;
    int k = 2;

    double **phi = subspace_iteration(mtx,&lambda,n,k,1);

    printf("\nphi\n");
    print_matrix(phi,n,k);
    printf("\nlambda\n");
    print_vector(lambda,k);

    free_matrix(mtx);
    free(lambda);
    free_matrix(phi);

    return 0;
}