/**********************************************************
 * This code was written by Richard Zanibbi in May 1998
 * based on code by Donald Quon and Hai Pham,
 * and Dr. R. T. Whitaker of the University of Tennesee
 * for use in Queen's University's CISC 859 course

 * Modified to make use of dynamic memory and independent
 * reading and writing functions by
 * R. Hasimoto-CIMAT, Sept. 2022.
************************************************************/

/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4

 * En el codigo se aplican filtros gaussianos y uniformes
 * a una imagen ingresada
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

#define MAXLENGTH 100
#define MAXVALUE  255

// Liberacion de la memoria asignada a la imagen
void free_image(unsigned char **img){
    if (img!=NULL){
        free(img[0]);
        free(img);
    }
}

unsigned char **pgmRead(char *fileName,int *rows,int *cols)
{
      FILE *filePointer;   
      char line[MAXLENGTH]; /* for character input from file */
      int maximumValue = 0; /* max value from header */
      int binary;           /* flag to indicate if file is binary (P5) or ascii (P2)*/
      long numberRead = 0;  /* counter for number of pixels read */
      long i,j;             /* (i,j) for loops */
      int test,temp;        /* for detecting EOF(test) and temp storage */
      unsigned char *tempPtr, **image;

      /* Open the file, return an error if necessary. */
      if ((filePointer = fopen(fileName,"r")) == NULL) {
	   printf ("ERROR: cannot open file\n\n");
	   fclose (filePointer);
	   return NULL;
      }
    
      *cols = *rows =0;

      /* Check the file signature ("Magic Numbers" P2 and P5); skip comments
         and blank lines (CR with no spaces before it).*/
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
           fgets (line,MAXLENGTH,filePointer);

      if (line[0]=='P' && (line[1]=='2')) {
	   binary = 0;
	 /*   printf ("\nFile Format: P2\n"); */
      }
      else if (line[0]=='P' && (line[1]=='5')) {
	   binary = 1;
	  /*  printf ("\nFORMAT: P5\n"); */
      }
      else {
	   printf ("ERROR: incorrect file format\n\n");
	   fclose (filePointer);
	   return (0);
      } 

      /* Input the width, height and maximum value, skip comments and blank
       * lines. */
      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
         fgets (line,MAXLENGTH,filePointer);
      sscanf (line,"%d %d",cols,rows);

      fgets (line,MAXLENGTH,filePointer);
      while (line[0]=='#' || line[0]=='\n') 
         fgets(line,MAXLENGTH,filePointer);
      sscanf (line,"%d",&maximumValue);

      if ((*cols)<1 ||(*rows)<1 || maximumValue<0 || maximumValue>MAXVALUE){
	   printf ("ERROR: invalid file specifications (cols/rows/max value)\n\n");
	   fclose (filePointer);
	   return (0);
      }

      // creating memory for the input image
      image = (unsigned char **)malloc((*rows) * sizeof(unsigned char *));
      if (image == NULL)
      {
         puts("\nFailure to allocate room for pointers");
         return NULL;
      }

      tempPtr = (unsigned char *) malloc((*rows)*(*cols)*sizeof(unsigned char));
      if (tempPtr == NULL)
      {
         puts("\nFailure to allocate room for the array");
         return NULL;
      }
      for (i = 0; i < *rows; i++)
      {
        image[i] = tempPtr + ( i * (*cols) );
      }


      /* Read in the frequence for binary (P5) or ascii (P2) PGM formats   */
      if (binary) {
	   for (i = 0; i < (*rows); i++) {
	        numberRead += fread((void *)&(image[i][0]),
		  sizeof(unsigned char),(*cols),filePointer); 
		if (feof(filePointer)) break;
	   }
      }
      else {
	   for (i= 0; i < (*rows); i++) {
	        for (j =0; j < (*cols); j++) { 
	             test = fscanf (filePointer,"%d",&temp);
		     if (test == EOF) break;
		     image[i][j] = (unsigned char)temp;

		     numberRead++;
		}
		if (test == EOF) break;
	   }
      }

      /* close the file and return 1 indicating success */
      fclose (filePointer);
      return image;
}



/* INPUT: a filename (char*), the dimensions of the pixmap (rows,cols of
 *   type long), and a pointer to a 2D array (MAXROWS x MAXCOLS) in row
 *   major order.
 * OUTPUT: an integer is returned indicating if the desired file was written
 *   (in P5 PGM format (binary)).  A 1 is returned if the write was completed
 *   and 0 if it was not.  An error message is returned if the file is not
 *   properly opened.  
 */ 

int pgmWrite(char* filename, int rows,int cols,unsigned char **image,char* comment_string)
{
    FILE* file;        /* pointer to the file buffer */
    int maxval;        /* maximum value in the image array */
    long nwritten = 0; /* counter for the number of pixels written */
    long i,j;          /* for loop counters */

    /* open the file; write header and comments specified by the user. */
    if ((file = fopen(filename, "w")) == NULL)	{
        printf("ERROR: file open failed\n");
        return(0);
    }
    fprintf(file,"P5\n");

    if (comment_string != NULL) fprintf(file,"# %s \n", comment_string);
        
    /* write the dimensions of the image */	
    fprintf(file,"%d %d \n", cols, rows);

    /* NOTE: MAXIMUM VALUE IS WHITE; COLOURS ARE SCALED FROM 0 - */
    /* MAXVALUE IN A .PGM FILE. */
        
    /* WRITE MAXIMUM VALUE TO FILE */
    fprintf(file, "%d\n", (int)255);

    /* Write frequence */
    for (i = 0;i < rows;i++) {
        nwritten += fwrite((void*)image[i],sizeof(unsigned char),cols,file);
    }
    printf("nwritten = %ld\n",nwritten);
            
    fclose(file);
    return(1);
}

void process_chunk(int process_id, unsigned char **image, int rows, int cols, unsigned char **new_image, int row_i, int row_f, int col_i, int col_f){
    double gauss5[5][5] = {{1.0,4.0,7.0,4.0,1.0},
                           {4.0,16.0,26.0,16.0,4.0},
                           {7.0,26.0,41.0,26.0,7.0},
                           {4.0,16.0,26.0,16.0,4.0},
                           {1.0,4.0,7.0,4.0,1.0}};
    for (int i=0;i<5;i++){
        for (int j=0;j<5;j++)
            gauss5[i][j] = gauss5[i][j]/273.0;
    }

    // se recorren todos los pixeles de la imagen resultante
    for (int i=row_i;i<row_f;i++){
        for (int j=col_i;j<col_f;j++){
            // se aplica el kernel a una region particular
            for (int k=0;k<5;k++){
                for (int m=0;m<5;m++){
                    int x = i+k;
                    int y = j+m;
                    // solo se realiza la operacion si los elementos pertenecen a la imagen
                    if (x<rows && y<cols)
                        (*new_image)[i*(cols-4)+j] += (int)((double)image[x][y]*gauss5[k][m]);
                    // si hay valores por encima del maximo, se normalizan
                    if ((*new_image)[i*(cols-4)+j]>255)
                        (*new_image)[i*(cols-4)+j] = 255;
                }
            }
        }
    }
}

// filtro gaussiano de 5x5
unsigned char **gaussian5(unsigned char **image, int rows, int cols, int nprocs){
    int dim1 = rows-4; // dimensiones de la nueva imagen
    int dim2 = cols-4;
    unsigned char **new_img = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
    if (new_img==NULL){
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        return NULL;
    }
    unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
    if (pxs==NULL){
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        free(new_img);
        return NULL;
    }
   
    for (int i=0;i<dim1;i++)
        new_img[i] = pxs+i*dim2;

    int shmid = shmget(IPC_PRIVATE, dim1*dim2*sizeof(unsigned char), IPC_CREAT | 0666);
    if (shmid==-1){
        printf("SUM. ERROR SHMGET\n");
        free_image(new_img);
        return NULL;
    }

    // vinculacion de memoria
    unsigned char *shm = (unsigned char *)shmat(shmid, NULL, 0);
    if (shm==(unsigned char *)-1){
        printf("SUM. ERROR EN SHMAT\n");
        shmctl(shmid, IPC_RMID, NULL);
        free_image(new_img);
        return NULL;
    }

    int pid;
    for (int p=0;p<nprocs;p++){
        if ((pid = fork())<0){
            printf("PRODUCT. FORKING ERROR.\n");
            shmdt(shm);
            shmctl(shmid, IPC_RMID, NULL);
            free_image(new_img);
            return NULL;
        }
        if (pid==0){
            int start_row, end_row, start_col, end_col;

            switch (nprocs) {
                case 2:
                    // Divide by rows (horizontal split)
                    if (p == 0) {
                        start_row = 0;
                        end_row = dim1-1;
                        start_col = 0;
                        end_col = dim2/2;
                    } else if (p == 1) {
                        start_row = 0;
                        end_row = dim1-1;
                        start_col = dim2/2;
                        end_col = dim2-1;
                    }
                    break;
                case 4:
                    // Divide by 2x2 grid (quadrants)
                    if (p == 0) {
                        start_row = 0;
                        end_row = dim1/2;
                        start_col = 0;
                        end_col = dim2/2;
                    } else if (p == 1) {
                        start_row = 0;
                        end_row = dim1/2;
                        start_col = dim2/2;
                        end_col = dim2;
                    } else if (p == 2) {
                        start_row = dim1/2;
                        end_row = dim1;
                        start_col = 0;
                        end_col = dim2/2;
                    } else if (p == 3) {
                        start_row = dim1/2;
                        end_row = dim1;
                        start_col = dim2/2;
                        end_col = dim2;
                    }
                    break;
                case 9:
                    // Divide by 3x3 grid
                    start_row = (p/3)*(dim1/3); // Row start based on the process number
                    end_row = start_row+(dim1/3);
                    start_col = (p%3)*(dim2/3); // Column start based on the process number
                    end_col = start_col+(dim2/3);
                    break;
                default:
                    printf("NUMERO DE PROCESOS INVALIDO\n");
                    shmdt(shm);
                    shmctl(shmid, IPC_RMID, NULL);
                    free_image(image);
                    free_image(new_img);
                    exit(1);
            }

            // cada hijo opera en su region asignada
            process_chunk(p, image, rows, cols, &shm, start_row, end_row, start_col, end_col);

            free_image(image);
            free_image(new_img);

            exit(0);
        }
    }

    // padre espera a que hijos terminen
    while(wait(NULL) != -1);

    for (int i=0;i<dim1;i++){
        for (int j=0;j<dim2;j++)
            new_img[i][j] = shm[i*dim2+j];
    }

    // desvinculacion y eliminacion de memoria compartida
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    return new_img;
}

int main(int argc, char **argv) {
    if (argc<2){
        printf("Uso: ./%s imagen.pgm\n", argv[0]);
        return -1;
    }
    int i, j, max, ok, rows, cols;
    int N[] = {2,4,9};
    unsigned char **image,**img5;
    struct timeval start, end;
    
    // Archivo de entrada
    image = pgmRead(argv[1],&rows,&cols);
    if (image == NULL) {
        printf("Error pgmRead\n");
        exit(1);
    }

    printf("Procesos\tTiempo\n");
    for (int i=0;i<3;i++){
        gettimeofday(&start, NULL);
        img5 = gaussian5(image,rows,cols,9);
        if (img5==NULL){
            printf("OCURRIO UN PROBLEMA. \n");
            free_image(image);
            return -1;
        }

        gettimeofday(&end, NULL);

        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("%d\t\t%.6f s\n", N[i], elapsed);
        
        //if (i<2)
            free_image(img5);
    }

    //ok = pgmWrite("multip_barbara.pgm",rows-4,cols-4,img5,NULL); // almacenamiento de la imagen resultante

    free_image(image);
    //free_image(img5);
}