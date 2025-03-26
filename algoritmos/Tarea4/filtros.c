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

#define MAXLENGTH 100
#define MAXVALUE  255

// liberacion de la memoria asignada
void free_matrix(unsigned char **mtx){
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
      printf("nwritten = %ld,",nwritten);
      	
      fclose(file);
      return(1);
}

// filtro gaussiano de 3x3
unsigned char **gaussian3(unsigned char **image, int rows, int cols){
   int dim1 = rows-2; // dimensiones de la nueva imagen
   int dim2 = cols-2;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }
   double gauss3[3][3] = {{1.0,2.0,1.0},
                        {2.0,4.0,2.0},
                        {1.0,2.0,1.0}};
   for (int i=0;i<3;i++){
      for (int j=0;j<3;j++)
         gauss3[i][j] = gauss3[i][j]/16.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<3;k++){
            for (int m=0;m<3;m++){
               new[i][j] += image[i+k][j+m]*gauss3[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

// filtro gaussiano de 5x5
unsigned char **gaussian5(unsigned char **image, int rows, int cols){
   int dim1 = rows-4; // dimensiones de la nueva imagen
   int dim2 = cols-4;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }

   double gauss5[5][5] = {{1.0,4.0,7.0,4.0,1.0},
                           {4.0,16.0,26.0,16.0,4.0},
                           {7.0,26.0,41.0,26.0,7.0},
                           {4.0,16.0,26.0,16.0,4.0},
                           {1.0,4.0,7.0,4.0,1.0}};
   for (int i=0;i<5;i++){
      for (int j=0;j<5;j++)
         gauss5[i][j] = gauss5[i][j]/273.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<5;k++){
            for (int m=0;m<5;m++){
               new[i][j] += image[i+k][j+m]*gauss5[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

// filtro gaussiano de 7x7
unsigned char **gaussian7(unsigned char **image, int rows, int cols){
   int dim1 = rows-6; // dimensiones de la nueva imagen
   int dim2 = cols-6;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }

   double gauss7[7][7] = {{0.0,0.0,1.0,2.0,1.0,0.0,0.0},
                        {0.0,3.0,13.0,22.0,13.0,3.0,0.0},
                        {1.0,13.0,59.0,97.0,59.0,13.0,1.0},
                        {2.0,22.0,97.0,159.0,97.0,22.0,2.0},
                        {1.0,13.0,59.0,97.0,59.0,13.0,1.0},
                        {0.0,3.0,13.0,22.0,13.0,3.0,0.0},
                        {0.0,0.0,1.0,2.0,1.0,0.0,0.0}};
   for (int i=0;i<7;i++){
      for (int j=0;j<7;j++)
         gauss7[i][j] = gauss7[i][j]/1003.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<7;k++){
            for (int m=0;m<7;m++){
               new[i][j] += image[i+k][j+m]*gauss7[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

// filtro uniforme de 3x3
unsigned char **uniform3(unsigned char **image, int rows, int cols){
   int dim1 = rows-2; // dimensiones de la nueva imagen
   int dim2 = cols-2;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }
   double unif3[3][3];
   for (int i=0;i<3;i++){
      for (int j=0;j<3;j++)
         unif3[i][j] = 1/9.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<3;k++){
            for (int m=0;m<3;m++){
               new[i][j] += image[i+k][j+m]*unif3[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

// filtro uniforme de 5x5
unsigned char **uniform5(unsigned char **image, int rows, int cols){
   int dim1 = rows-4; // dimensiones de la nueva imagen
   int dim2 = cols-4;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }

   double unif5[5][5];
   for (int i=0;i<5;i++){
      for (int j=0;j<5;j++)
         unif5[i][j] = 1/25.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<5;k++){
            for (int m=0;m<5;m++){
               new[i][j] += image[i+k][j+m]*unif5[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

// filtro uniforme de 7x7
unsigned char **uniform7(unsigned char **image, int rows, int cols){
   int dim1 = rows-6; // dimensiones de la nueva imagen
   int dim2 = cols-6;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_matrix(new);
      return NULL;
   }

   double unif7[7][7];
   for (int i=0;i<7;i++){
      for (int j=0;j<7;j++)
         unif7[i][j] = 1/49.0;
   }
   
   for (int i=0;i<dim1;i++){
      new[i] = pxs+i*dim2;
   }

   // se recorren todos los pixeles de la imagen resultante
   for (int i=0;i<dim1;i++){
      for (int j=0;j<dim2;j++){
         // se aplica el kernel a una region particular
         for (int k=0;k<7;k++){
            for (int m=0;m<7;m++){
               new[i][j] += image[i+k][j+m]*unif7[k][m];
            }
            // si hay valores por encima del maximo, se normalizan
            if (new[i][j]>255)
               new[i][j] = 255;
         }
         
      }
   }

   return new;
}

int main() {
   int i, j, max, ok, rows, cols;
   int count = 0;
   unsigned char **image,**img3,**img5,**img7;
   char file_in[80], file_out[80];

   // Archivo de entrada
   printf("Archivo pgm de entrada: ");
   scanf("%s",file_in);
   image = pgmRead(file_in,&rows,&cols);
   if (image == NULL) {
      printf("Error pgmRead\n");
      exit(1);
   }

  // Procesamiento de la imagen: ** INCLUIR SU CODIGO AQUI **

   for (int i=0;i<3;i++){
      // tamano de los operadores
      j = 3+2*i;
      // aplicacion de los filtros correspondientes
      switch (j){
         case 3:
            //img3 = gaussian3(image,rows,cols);
            img3 = uniform3(image,rows,cols);
            ok = pgmWrite("barbara_uniform3.pgm",rows-j+1,cols-j+1,img3,NULL); // almacenamiento de la imagen resultante
            break;
         case 5:
            //img5 = gaussian5(image,rows,cols);
            img5 = uniform5(image,rows,cols);
            ok = pgmWrite("barbara_uniform5.pgm",rows-j+1,cols-j+1,img5,NULL); // almacenamiento de la imagen resultante
            break;
         case 7:
            //img7 = gaussian7(image,rows,cols);
            img7 = uniform7(image,rows,cols);
            ok = pgmWrite("barbara_uniform7.pgm",rows-j+1,cols-j+1,img7,NULL); // almacenamiento de la imagen resultante
            break;
         default:
            printf("NO ES UN TAMANO DE FILTRO VALIDO\n");
            break;
      }
   }
   free_matrix(image);
   free_matrix(img3);
   free_matrix(img5);
   free_matrix(img7);
}