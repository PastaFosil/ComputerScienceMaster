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
 * TAREA 6
 * 
 * Se construye la estructura de datos arbol binario e imprime 
 * los ancestros de un nodo en particular
************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAXLENGTH 100
#define MAXVALUE  255
#define M 10

// liberacion de la memoria asignada
void free_img(unsigned char **mtx){
    if (mtx!=NULL){
        free(mtx[0]);
        free(mtx);
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


      /* Read in the data for binary (P5) or ascii (P2) PGM formats   */
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

      /* Write data */
      for (i = 0;i < rows;i++) {
          nwritten += fwrite((void*)image[i],sizeof(unsigned char),cols,file);
      }
      printf("nwritten = %ld\n",nwritten);
      	
      fclose(file);
      return(1);
}

// Generacion de espacio para imagen
unsigned char **genImg(int nRows, int nCols) {
    // Se solicita espacio para la imagen
    unsigned char **imagen, *temp;
    imagen = (unsigned char **)malloc(nRows * sizeof(unsigned char *)); // Espacio para los apuntadores

    // Se verifica que exista espacio
    if(imagen == NULL) {
        printf("Error de asignacion de memoria.\n");
        return NULL;
    }

    // Solicitamos la memoria contigua
    temp = (unsigned char *)calloc(nRows * nCols, sizeof(unsigned char));
    // Se verifica que exista espacio
    if(temp == NULL) {
        printf("Error de asignacion de memoria.\n");
        free(imagen); // Liberacion de memoria en caso de error
        return NULL;
    }

    // Asignacion de la memoria contigua a la imagen
    for(int i = 0; i < nRows; i++) {
        imagen[i] = &temp[i * nCols];
    }

    // Retornando la imagen
    return imagen;
}

double next_x(double x_j){
    double lambda = 4.0;
    return (lambda*x_j*(1-x_j));
}

/*
Cifra o descifra la imagen img mediante un algoritmo basado en ruleta 
inicializado con la trayectoria x de M mapas aleatorios, cifrando el
primer pixel con x[map].
*/
unsigned char **cipher(unsigned char **img, int rows, int cols, double *x, int map){
    unsigned char **ciphered = genImg(rows, cols); // arreglo que guardara la imagen cifrada
    if (ciphered==NULL){
        printf("CIPHER. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    printf("(%d, %d)\n", rows, cols);
    // apuntadores empleador en el algoritmo
    unsigned char *img_ptr = &img[0][0];
    unsigned char *end_ptr = &img[rows-1][cols-1]+1;
    unsigned char *ciphered_ptr = &ciphered[0][0];
    double T[5]; // lista de los ultimos 5 mapas modificados
    
    // cifra cada pixel de la imagen
    long long j = 0;
    while (img_ptr!=end_ptr){
        // cifra 4 pixeles antes de realizar una perturbacion a la trayectoria
        for (int i=0;i<5;i++){
            j++;
            // actualizacion de mapa
            x[map] = next_x(x[map]);
            T[i] = x[map]; // almacenamiento de mapa actualizado
            *ciphered_ptr = (unsigned char)(*img_ptr^(unsigned char)(x[map]*255)); // cifrado de pixel actual
            // actualizacion de apuntadores a pixeles de la imagen original y la cifrada
            img_ptr++;
            ciphered_ptr++;
            // calculo del siguiente mapa a modificar
            map = (int)(x[map]*255)%M;
            
            // si se llega al final de la imagen antes de terminar el ciclo, se devuelve la imagen cifrada
            if (img_ptr==end_ptr)
                return ciphered;
        }

        // interaccion de las 5 iteraciones pasadas de la ruleta
        unsigned char H = (unsigned char)T[0];
        for (int i=1;i<5;i++)
            H ^= (unsigned char)T[i];
        // cifrado de quinto pixel
        map = (int)(T[4]*255)%M;
        x[map] = next_x(x[map]);
        x[map] += (unsigned char)255 & H;
        *ciphered_ptr = (unsigned char)(*img_ptr^(unsigned char)(x[map]*255));
        // actualizacion de apuntadores
        img_ptr++;
        ciphered_ptr++;
    }

    printf("j=%lld\n",j);
    return ciphered;
}

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s imagen.pgm\n",argv[0]);
        return -1;
    }

    int i, j, max, ok, rows, cols;
    unsigned char **image, **new_image;
    char *filename = argv[1]; // nombre del archivo de input

    // lectura de la imagen
    image = pgmRead(filename,&rows,&cols);
    if (image == NULL) {
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        exit(1);
    }

    double x[M];
    double x_copy[M];
    for (int i=0;i<M;i++){
        x[i] = rand()/(double)RAND_MAX;
        x_copy[i] = x[i];
    }
    int map = rand() % M;

    new_image = cipher(image,rows,cols,x,map);
    if (new_image==NULL){
        printf("NO SE PUDO CIFRAR LA IMAGEN D:\n");
        return -1;
    }
    pgmWrite("cifrado.pgm",rows,cols,new_image,NULL);

    free_img(image);

    image = cipher(new_image,rows,cols,x_copy,map);
    if (new_image==NULL){
        printf("NO SE PUDO CIFRAR LA IMAGEN D:\n");
        return -1;
    }
    pgmWrite("descifrado.pgm",rows,cols,image,NULL);

    free_img(image);
    free_img(new_image);

    return 0;
}