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
 * TAREA 7
 * 2024/10/09
 * Funciones usadas en los programas de la tarea 7.
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include "tools.h"

#define MAXLENGTH 100
#define MAXVALUE  255

/********************************************************************************************/
// ARBOL BINARIO

typedef struct node {
    struct node *left;
    unsigned long long frequence;
    int intensity;
    struct node *right;
}NODE;

/*Crea un nodo con el valor val especificado*/
NODE *create_node(int val){
    NODE *t;

    t = (NODE *)malloc(sizeof(NODE));
    if (t==NULL){
        printf("CREATE. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    t->frequence = val;
    t->intensity = -1;
    t->left = t->right = NULL;
    return(t);
}

/*Inserta el nodo val al arbol especificado*/
NODE *insert(NODE *root, int val){
    if (root==NULL)
        return create_node(val);
    
    if (val > root->frequence)
        root->right = insert(root->right, val);
    else if (val < root->frequence)
        root->left = insert(root->left, val);
    
    return root;
}

// Devuelve el apuntador a la raiz de un arbol identico a root
NODE *copy_tree(NODE *root){
    if (root==NULL){
        printf("COPY. ARBOL VACIO\n");
        return NULL;
    }

    NODE *copy = create_node(root->frequence);
    if (copy==NULL){
        printf("COPY. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    copy->intensity = root->intensity;
    if (root->left!=NULL)
        copy->left = copy_tree(root->left);
    if (root->right!=NULL)
        copy->right = copy_tree(root->right);

    return copy;
}

/*Imprime el arbol ingresado*/
void show_tree(NODE *node){
    if (node != NULL){
        printf("(%lld, %d)", node->frequence, node->intensity);
        
        if (node->left!=NULL)
            show_tree(node->left);
        //printf("\n");
        if (node->right!=NULL)
            show_tree(node->right);
    } else
        printf("ARBOL VACIO\n");
}

/*Libera la memoria del arbol ingresado*/
void cut_tree(NODE *t){
    if (t==NULL)
        return;
    cut_tree(t->left);
    cut_tree(t->right);
    free(t);
}

/********************************************************************************************/
// IMAGENES

// Liberacion de la memoria asignada a la imagen
void free_img(unsigned char **img){
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
      printf("nwritten = %ld,",nwritten);
      	
      fclose(file);
      return(1);
}


/********************************************************************************************/
// CIFRADO

/*Calcula el histograma de la imagen de dimensiones rows x cols ingresada*/
int *histogram(unsigned char **img, int rows, int cols){
    int *freq = (int *)calloc(MAXVALUE+1,sizeof(int));
    if (freq==NULL){
        printf("HISTOGRAM. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    int total = rows*cols;
    unsigned char *ptr = &img[0][0];
    // recorre la imagen y suma la ocurrencia del valor de intensidad en su indice correspondiente
    for (int i=0;i<total;i++){
        freq[*ptr]++;
        ptr++;
    }
    
    return freq;
}

// Ordena los ultimos n elementos de la lista segun su frecuencia
void order_list(NODE ***list, int n){
    int pass=0;
    while (pass==0){
        pass = 1;
        // evalua si el par de elementos esta ordenado
        for (int i=MAXVALUE+1-n;i<MAXVALUE;i++){
            if ((*list)[i]->frequence>(*list)[i+1]->frequence){
                NODE *aux = (*list)[i+1];
                (*list)[i+1] = (*list)[i];
                (*list)[i] = aux;
                pass = 0;
            }
        }
    }
}

// Devuelve una lista de direcciones a nodos en cuyo ultimo elemento se encuentra el arbol completo
void huffman_code(NODE **root, int *hist){
    // lista que guardara apuntadores a los nodos del arbol
    NODE **list = (NODE **)malloc((MAXVALUE+1)*sizeof(NODE *));
    if (list==NULL){
        printf("HUFFMAN. ERROR ASIGNANDO MEMORIA\n");
        return;
    }
    // inicializacion de lista, ordenados por intensidad
    for (int i=0;i<=MAXVALUE;i++){ 
        list[i] = create_node(hist[i]);
        if (list[i]==NULL){
            printf("HUFFMAN. ERROR ASIGNANDO MEMORIA\n");
            for (int j=0;j<i;j++)
                free(list[j]);
            free(list);
            return;
        }
        list[i]->intensity = i;
    }

    int elements = MAXVALUE+1; // cantidad de elementos por integrar al arbol binario
    order_list(&list, elements);
    //for (int i=0;i<256;i++)
        //printf("%d: %lld\n", list[i]->intensity, list[i]->frequence);

    while (elements>1){
        // crea un nodo cuyas ramas seran los dos elementos con las dos frecuencias mas bajas
        NODE *lowest1 = list[MAXVALUE+1-elements];
        NODE *lowest2 = list[MAXVALUE+2-elements];

        if (lowest1->frequence==0)
            elements--;
        else {
            NODE *temp = create_node(lowest1->frequence+lowest2->frequence);
            if (temp==NULL){
                printf("HUFFMAN. ERROR ASIGNANDO MEMORIA\n");
                for (int i=0;i<=MAXVALUE;i++)
                    cut_tree(list[i]);
                free(list);
                return;
            }
            temp->left = lowest1;
            temp->right = lowest2;

            list[MAXVALUE+2-elements] = temp; // actualizacion de nodo de menor frecuencia
            elements--; // actualizacion de los elementos por agregar al arbol
            order_list(&list, elements); // ordena la lista con el nuevo nodo
        }
    }

    // se crea una copia del arbol para liberar la memoria de la lista
    *root = copy_tree(list[MAXVALUE]);

    // liberacion de la lista
    cut_tree(list[MAXVALUE]);
    for (int j=0;j<=MAXVALUE;j++)
        free(list[j]);
    free(list);
}

/*
Guarda el codigo de cada una de las intensidades en su correspondiente 
lugar en la lista codes, construyendolo a partir del arbol root
*/
void build_codes(NODE *root, char **codes, char *current_code, int depth) {
    if (root == NULL) return;

    // Si el nodo ingresado es hoja, se guarda el codigo
    if (root->left == NULL && root->right == NULL) {
        current_code[depth] = '\0';  // senalacion el final de la lista
        strcpy(codes[root->intensity], current_code);  // almacenamiento del codigo en su lugar correspondiente 
        return;
    }

    // agrega 0 o 1 segun la rama que se sigue en el arbol
    current_code[depth] = '0';
    build_codes(root->left, codes, current_code, depth + 1);

    current_code[depth] = '1';
    build_codes(root->right, codes, current_code, depth + 1);
}

// Devuelve una lista con los codigos de cada nivel de intensidad a partir del arbol root
char **get_codes(NODE *root){
    // asignacion de memoria para la lista de codigos
    char **codes = (char **)malloc((MAXVALUE+1)*sizeof(char *));
    if (codes==NULL){
        printf("GET. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    // inicializacion de los codigos
    for (int i=0;i<=MAXVALUE;i++){
        codes[i] = (char *)malloc((MAXVALUE+1)*sizeof(char));
        if (codes[i]==NULL){
            printf("GET. ERROR ASIGNANDO MEMORIA\n");
            for (int j=0;j<i;j++)
                free(codes[j]);
            return NULL;
        }
        codes[i][0] = '\0';
    }

    char code[MAXVALUE+1]; // creacion de lista que almacenara los codigos
    build_codes(root, codes, code, 0); // construccion y almacenamiento de los codigos

    return codes;
}

/*
Escribe 8 bits del cifrado a la vez
*/
void write_bit(FILE *file, unsigned char *buffer, int *count, int bit){
    if (bit) // escribe el bit 1 en la posicion (7-(*count)%8) del buffer
        *buffer |= (1<<(7-(*count)%8));
    (*count)++;

    // escribe el byte llenado y se reinician el buffer y la cuenta
    if ((*count)==8){
        fwrite(buffer, sizeof(unsigned char), 1, file);
        *buffer = 0;
        *count = 0;
    }
}

/* 
Comprime la imagen img con los codigos de Huffman generados (almacenados en codes) 
y devuelve el numero de bits escritos
*/
int huffman_cipher(char *filename, unsigned char **img, int rows, int cols, char **codes){
    FILE* file = fopen(filename, "w");
    if (file==NULL)	{
        printf("CIPHER. ERROR CREANDO ARCHIVO\n");
	    return -1;
    }

    // escribe las dimensiones de la imagen
    fprintf(file,"%d %d \n", rows, cols);

    long long written = 0; // cuenta el numero total de bits escritos
    unsigned char *ptr = &img[0][0]; // apuntador al pixel que se comprimira
    unsigned char *end = (&img[rows-1][cols-1])+1; // apuntador al final de la imagen
    unsigned char buffer = 0; // byte que almacenara los bits a escribir
    int count = 0; // contara la cantidad de bits asignados al buffer
    while (ptr!=end){ // recorre todos los pixeles de la imagen
        char *aux = codes[*ptr]; // codigo del nivel de intensidad del pixel actual
        int i = 0; // indice del bit actual del codigo
        while (aux[i]!='\0'){ // asigna todos los bits del codigo para escribirlos en el archivo
            write_bit(file,&buffer,&count,aux[i]-'0');
            // actualizacion del indice y la cantidad de bits escritos en el archivo
            i++;
            written++;
        }
        // actualizacion del apuntador
        ptr++;
    }
    // si no se lleno el ultimo byte, los bits restantes se dejan en ceros para escribirlo en el archivo
    count = 7;
    write_bit(file,&buffer,&count,0);

    fclose(file);
    return written;
}

// Lee el archivo file un byte a la vez, y lo almacena  en chunk para leer bit a bit
void read_byte(FILE *file, char *chunk){
    unsigned char byte;
    int bit;

    // intenta leer un byte del archivo, si lo logra, lo guarda en chunk, si no, deja chunk en ceros
    if (fread(&byte, sizeof(unsigned char), 1, file)==1){
        for (int i=7;i>=0;i--){
            bit = (byte>>i)&1;
            chunk[7-i] = bit+'0';
        }
        return;
    } else
        strcpy(chunk,"00000000");
}

/*
Lee el archivo filename y descomprime la imagen de dimensiones rows x cols 
a partir del arbol de codigos de Huffman root
*/
unsigned char **huffman_decipher(char *filename, int *rows, int *cols, NODE *root){
    FILE *file = fopen(filename, "rb");
    if (file==NULL)	{
        printf("DECIPHER. ERROR CREANDO ARCHIVO\n");
	    return NULL;
    }
    
    // lectura de dimensiones de la imagen
    fscanf(file, "%d %d", rows, cols);
    unsigned char **img = (unsigned char **)malloc(*rows*sizeof(unsigned char *));
    unsigned char *pxs = (unsigned char *)malloc((*rows)*(*cols)*sizeof(unsigned char));
    if (img==NULL || pxs==NULL){
        printf("DECIPHER. ERROR ASIGNANDO MEMORIA\n");
        fclose(file);
        free(img);
        free(pxs);
        return NULL;
    }
    
    unsigned char *pxl_ptr = pxs; // apuntador a los pixeles de la imagen a construir
    unsigned char *end = &pxs[(*rows)*(*cols)-1]+1; // apuntador al final del espacio de memoria de la imagen
    unsigned char byte[9]; // almacenara el byte leido de filename
    byte[8] = '\0';
    int i = 8; // contador de bits leidos del byte
    // recorre todos los pixeles que tendra la imagen descomprimida
    while (pxl_ptr!=end){
        NODE *temp = root; // reinicia el apuntador a la raiz del arbol
        // recorre el arbol de acuerdo al codigo encontrado hasta llegar al nodo hoja correspondiente
        while (temp->intensity==-1){
            // si ya se han extraido todos los bits del byte leido, se lee el siguiente byte
            if (i==8){
                read_byte(file, byte);
                i = 0;
            }
            // sigue las ramas del arbol senaladas por el codigo
            if (byte[i]=='0'){
                temp = temp->left;
            }
            if (byte[i]=='1')
                temp = temp->right;
            i++; // actualizacion del numero de bits leidos
        }
        // asigna el valor de intensidad al pixel actual
        *pxl_ptr = temp->intensity;
        pxl_ptr++; // actualizacion al siguiente pixel
    }

    fclose(file);

    for (int i=0; i<*rows; i++)
        img[i] = pxs+i*(*cols);
    return img;
}