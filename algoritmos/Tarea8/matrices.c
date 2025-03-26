/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 8
 * 
 * Suma o multiplica matrices haciendo uso de multiprocesos
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>

int ***create_matrix(int xy, int z){
    int ***mtx = (int ***)malloc(z*sizeof(int **));
    int **temp_mtx = (int **)malloc(xy*z*sizeof(int *));
    int *temp = (int *)calloc(xy*xy*z, sizeof(int));
    if (mtx==NULL || temp_mtx==NULL || temp==NULL){
        printf("CREATE. ERROR ASIGNANDO MEMORIA\n");
        free(temp);
        free(temp_mtx);
        free(mtx);
        return NULL;
    }
    
    for (int i=0;i<z;i++){
        mtx[i] = temp_mtx+xy*i;
        for (int j=0;j<xy;j++)
            mtx[i][j] = temp+xy*xy*i+xy*j;
    }

    return mtx;
}

void print_matrix(int ***A, int xy, int z){
    for (int i=0;i<z;i++){
        for (int j=0;j<xy;j++){
            for (int k=0;k<xy;k++)
                printf("%d ",A[i][j][k]);
            printf("\n");
        }
        printf("\n");
    }
}

void free_matrix(int*** matrix) {
    if (matrix!=NULL){
        free(matrix[0][0]);
        free(matrix[0]);
        free(matrix);
    }
}

int ***sum_matrices(int ***mat1, int ***mat2, int xy, int z, int nproc){
    int ***res = create_matrix(xy,z);
    if (res==NULL){
        printf("SUM. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    // creacion de memoria compartida
    int shmid_res = shmget(IPC_PRIVATE, xy*xy*z*sizeof(int), IPC_CREAT | 0666);
    if (shmid_res==-1){
        printf("SUM. ERROR SHMGET\n");
        free_matrix(res);
        return NULL;
    }

    // vinculacion de memoria
    int *shm_res = (int *)shmat(shmid_res, NULL, 0);
    if (shm_res==(int *)-1){
        printf("SUM. ERROR EN SHMAT\n");
        shmctl(shmid_res, IPC_RMID, NULL);
        free_matrix(res);
        return NULL;
    }

    // cada hijo se encarga de sumar un par de submatrices 2D en particular
    int pid;
    for (int process_id=0;process_id<nproc;process_id++){
        if ((pid = fork())<0){
            printf("SUM. FORKING ERROR.\n");
            shmdt(shm_res);
            shmctl(shmid_res, IPC_RMID, NULL);
            free_matrix(res);
            return NULL;
        }
        if (pid==0){
            for (int i=process_id;i<z;i+=nproc){
                for (int j=0;j<xy;j++){
                    for (int k=0;k<xy;k++){
                        shm_res[k+j*xy+i*xy*xy] = mat1[i][j][k] + mat2[i][j][k];
                    }
                }
            }
            exit(0);
        }
    }

    // padre espera a que hijos terminen
    while(wait(NULL) != -1);

    for (int i=0;i<z;i++){
        for (int j=0;j<xy;j++){
            for (int k=0;k<xy;k++){
                res[i][j][k] = shm_res[k+j*xy+i*xy*xy];
            }
        }
    }
    
    // desvinculacion y eliminacion de memoria compartida
    shmdt(shm_res);
    shmctl(shmid_res, IPC_RMID, NULL);
    
    return res;
}

int ***product_matrices(int ***mat1, int ***mat2, int xy, int z, int nproc){
    int ***res = create_matrix(xy,z);
    if (res==NULL){
        printf("PRODUCT. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    // creacion de memoria compartida
    int shmid_res = shmget(IPC_PRIVATE, xy*xy*z*sizeof(int), IPC_CREAT | 0666);
    if (shmid_res==-1){
        printf("PRODUCT. ERROR SHMGET\n");
        free_matrix(res);
        return NULL;
    }

    // vinculacion de memoria
    int *shm_res = (int *)shmat(shmid_res, NULL, 0);
    if (shm_res==(int *)-1){
        printf("PRODUCT. ERROR EN SHMAT\n");
        shmctl(shmid_res, IPC_RMID, NULL);
        free_matrix(res);
        return NULL;
    }

    // cada hijo se encarga de sumar un par de submatrices 2D en particular
    int pid;
    for (int process_id=0;process_id<nproc;process_id++){
        if ((pid = fork())<0){
            printf("PRODUCT. FORKING ERROR.\n");
            shmdt(shm_res);
            shmctl(shmid_res, IPC_RMID, NULL);
            free_matrix(res);
            return NULL;
        }
        if (pid==0){
            for (int i=process_id;i<z;i+=nproc){
                for (int j=0;j<xy;j++){
                    for (int k=0;k<xy;k++){
                        shm_res[k+j*xy+i*xy*xy] = mat1[i][j][k] * mat2[i][j][k];
                    }
                }
            }
            free_matrix(mat1);
            free_matrix(mat2);
            free_matrix(res);

            exit(0);
        }
    }

    // padre espera a que hijos terminen
    while(wait(NULL) != -1);

    for (int i=0;i<z;i++){
        for (int j=0;j<xy;j++){
            for (int k=0;k<xy;k++){
                res[i][j][k] = shm_res[k+j*xy+i*xy*xy];
            }
        }
    }
    
    // desvinculacion y eliminacion de memoria compartida
    shmdt(shm_res);
    shmctl(shmid_res, IPC_RMID, NULL);
    
    return res;
}

int main(int argc, char **argv){
    if (argc<3){
        printf("Uso: ./%s DIMENSION CAPAS\n", argv[0]);
        return -1;
    }

    int dim = atoi(argv[1]); // dimension de las matrices cuadradas
    int k = atoi(argv[2]); // capas de las matrices
    int N[] = {4,8,12}; // numero de procesos a utilizar
    int ***mat1, ***mat2, ***res;
    struct timeval start, end;
    
    // asignacion de memoria a matrices
    mat1 = create_matrix(dim, k);
    if (mat1==NULL){
        printf("ALGO SALIO MAL\n");
        return -1;
    }
    mat2 = create_matrix(dim, k);
    if (mat2==NULL){
        printf("ALGO SALIO MAL\n");
        free_matrix(mat1);
        return -1;
    }

    // inicializacion de matrices
    for (int i=0;i<k;i++)
        for (int j=0;j<dim;j++)
            for (int l=0;l<dim;l++){
                mat1[i][j][l] = 3;
                mat2[i][j][l] = 2;
            }

    printf("Procesos\tTiempo\n");
    for (int i=0;i<3;i++){
        gettimeofday(&start, NULL);

        //res = product_matrices(mat1, mat2, dim, k, N[i]);
        res = sum_matrices(mat1, mat2, dim, k, N[i]);
        if (res==NULL){
            printf("OCURRIO UN PROBLEMA. \n");
            free_matrix(mat1);
            free_matrix(mat2);

            return -1;
        }

        gettimeofday(&end, NULL);

        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("%d\t\t%.6f s\n", N[i], elapsed);
        
        free_matrix(res);
    }
    
    //printf("\n");
    //print_matrix(res,dim,k);
    //printf("\n");

    free_matrix(mat1);
    free_matrix(mat2);

    return 0;
}