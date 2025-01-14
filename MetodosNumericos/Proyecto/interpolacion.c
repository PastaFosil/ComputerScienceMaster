/**********************************************************
 * This code was written by Richard Zanibbi in May 1998
 * based on code by Donald Quon and Hai Pham,
 * and Dr. R. T. Whitaker of the University of Tennesee
 * for use in Queen's University's CISC 859 course

 * Modified to make use of dynamic memory and independent
 * reading and writing functions by
 * R. Hasimoto-CIMAT, Sept. 2022.
 * 
 * Modified by Juan Carlos Perez Ramirez, Dec. 2024
************************************************************/

/*****************************************
        Metodos Numericos
        2024/12/11
        Juan Carlos Perez Ramirez
        
        Funciones para proyecto 
        de metodos numericos
*****************************************/

#include <stdio.h>
#include <math.h>

#define MAXLENGTH 100
#define MAXVALUE  255
#define ALPHA -0.5
#define ERR 0.0001
#define PI 3.14159265358979323846


// Generacion de matriz de doble precision
unsigned char **genImage(int nRows, int nCols) {
    // Se solicita espacio para la matriz
    unsigned char **matriz, *temp;
    matriz = (unsigned char **)malloc(nRows * sizeof(unsigned char *)); // Espacio para los apuntadores

    // Se verifica que exista espacio
    if(matriz == NULL) {
        printf("Error de asignacion de memoria.\n");
        return NULL;
    }

    // Solicitamos la memoria contigua
    temp = (unsigned char *)calloc(nRows * nCols, sizeof(unsigned char));
    // Se verifica que exista espacio
    if(temp == NULL) {
        printf("Error de asignacion de memoria.\n");
        free(matriz); // Liberacion de memoria en caso de error
        return NULL;
    }

    // Asignacion de la memoria contigua a la matriz
    for(int i = 0; i < nRows; i++) {
        matriz[i] = &temp[i * nCols];
    }

    // Retornando la matriz
    return matriz;
}

// Liberacion de la memoria asignada a la imagen
void free_img(unsigned char **img){
    if (img!=NULL){
        free(img[0]);
        free(img);
    }
}

// Copia los contenidos de la matriz B en A
void copy_img(unsigned char **A, unsigned char **B, int m, int n){
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++)
            A[i][j] = B[i][j];
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

// Devuelve los valores truncados de los pixeles de la imagen
unsigned char nothing(double x, double y, unsigned char **img, int rows, int cols){
        if (x<0 || x>=rows || y<0 || y>=cols)
                return 0;
        if (fabs(x-floor(x))<ERR && fabs(y-floor(y))<ERR)
                return img[(int)x][(int)y];
        else    return 0;
}

// Calcula la interpolacion de vecinos mas cercanos para un pixel
unsigned char nearest_neighbour(double x, double y, unsigned char **img, int rows, int cols){
        if (x<0 || x>=rows || y<0 || y>=cols)
                return 0;
        return img[(int)(floor(x))][(int)(floor(y))];
}

// Calcula la interpolacion bilineal para un pixel
unsigned char bilinear(double x, double y, unsigned char **img, int rows, int cols){
        if (x<0 || x>=rows || y<0 || y>=cols)
                return 0;
        double f1, f2, diff;
        int x1 = (int)floor(x);
        int y1 = (int)floor(y);
        int x2 = (x1 + 1 < rows) ? x1 + 1 : x1; // Clamp within bounds
        int y2 = (y1 + 1 < cols) ? y1 + 1 : y1;

        if (x==x1 && y==y1)
                return img[x1][y1];
        diff = x2-x1;
        f1 = (diff==0)? img[x1][y1] : (x2-x)*img[x1][y1]/diff + (x-x1)*img[x2][y1]/diff;
        f2 = (diff==0)? img[x1][y2] : (x2-x)*img[x1][y2]/diff + (x-x1)*img[x2][y2]/diff;
        
        diff = y2-y1;
        return (diff==0)? f1 : (y2-y)*f1/diff + (y-y1)*f2/diff;
}

// Calcula el kernel cubico
double cubic_kernel(double x){
        double t = fabs(x);
        double t2 = t*t;
        double t3 = t2*t;
        if (t<=1.0)
                return (ALPHA+2)*t3 - (ALPHA+3)*t2 + 1;
        else if (t<=2.0)
                return ALPHA*t3 - 5*ALPHA*t2 + 8*ALPHA*t - 4*ALPHA;
        else
                return 0;
}

// Calcula la interpolacion bicubica para un pixel
unsigned char bicubic(double x, double y, unsigned char **img, int rows, int cols){
        if (x < 1.0) x = 1.0;
        if (x >= rows - 2) x = rows - 2.00;
        if (y < 1.0) y = 1.0;
        if (y >= cols - 2) y = cols - 2.00;

        int x0 = (int)floor(x);
        int y0 = (int)floor(y);
        double result = 0.0;

        double wx[4], wy[4];
        for (int i = -1; i <= 2; i++) {
                wx[i + 1] = cubic_kernel(x - (x0 + i));
                wy[i + 1] = cubic_kernel(y - (y0 + i));
        }

        for (int i = -1; i <= 2; i++) {
                for (int j = -1; j <= 2; j++) {
                int xi = x0 + i;
                int yj = y0 + j;

                // Clamp indices to image bounds
                if (xi < 0) xi = 0;
                if (xi >= rows) xi = rows - 1;
                if (yj < 0) yj = 0;
                if (yj >= cols) yj = cols - 1;

                double px = (double)img[xi][yj];
                double w = wx[i + 1] * wy[j + 1];
                result += px * w;
                }
        }
        if (result < 0.0) result = 0.0;
        else if (result > 255.0) result = 255.0;

        return (unsigned char)result;
}

// Redimensiona la imagen ingresada
unsigned char **resize(unsigned char **img, int rows, int cols, int new_rows, int new_cols, unsigned char (*f)(double, double, unsigned char**, int, int)){
        unsigned char **new_img = genImage(new_rows, new_cols);
        if (new_img==NULL){
                printf("NO SE PUDO ASIGNAR MEMORIA\n");
                return NULL;
        }

        double x_ratio = (double)rows/(double)new_rows;
        double y_ratio = (double)cols/(double)new_cols;
        for (int r=0;r<new_rows;r++){
                for (int c=0;c<new_cols;c++){
                        //printf("r=%d, c=%d\n", r, c);
                        new_img[r][c] = f((double)r*x_ratio, (double)c*y_ratio, img, rows, cols);
                }
        }
        return new_img;
}

// Calcula las nuevas dimensiones de la imagen rotada
void new_dimensions(int rows, int cols, double angle, int *new_rows, int *new_cols) {
    double cos_angle = fabs(cos(angle));
    double sin_angle = fabs(sin(angle));

    *new_rows = (int)ceil(rows * cos_angle + cols * sin_angle);
    *new_cols = (int)ceil(rows * sin_angle + cols * cos_angle);
}

// Rota la imagen ingresada
unsigned char **rotate(unsigned char **img, int rows, int cols, double angle, unsigned char (*f)(double, double, unsigned char**, int, int)){
        double rads = angle*PI/180.0;

        int new_rows, new_cols;
        new_dimensions(rows, cols, rads, &new_rows, &new_cols);

        unsigned char **new_img = genImage(new_rows, new_cols);
        if (new_img==NULL){
                printf("NO SE PUDO ASIGNAR MEMORIA\n");
                return NULL;
        }

        // Calculo del centro de las imagenes
        double xc = (double)rows/2;
        double yc = (double)cols/2;
        double new_xc = (double)new_rows/2;
        double new_yc = (double)new_cols/2;

        for (int i=0;i<new_rows;i++){
                for (int j=0;j<new_cols;j++){
                        double x = (i-new_xc)*cos(rads) + (j-new_yc)*sin(rads) + xc;
                        double y = -(i-new_xc)*sin(rads) + (j-new_yc)*cos(rads) + yc;

                        if ((x>=0 && x<rows) && (y>=0 && y<cols))
                                new_img[i][j] = f(x, y, img, rows, cols);
                        else
                                new_img[i][j] = 0;
                }
        }

        return new_img;
}

unsigned char **gaussian7(unsigned char **image, int rows, int cols){
   int dim1 = rows-6; // dimensiones de la nueva imagen
   int dim2 = cols-6;
   unsigned char **new = (unsigned char **)malloc(dim1*sizeof(unsigned char *)); // asigna espacio para nueva imagen
   unsigned char *pxs = (unsigned char *)calloc(dim1*dim2,sizeof(unsigned char));
   
   if (new==NULL||pxs==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free(pxs);
      free_img(new);
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