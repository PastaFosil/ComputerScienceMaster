/*****************************************
        Metodos Numericos
        2024/12/11
        Juan Carlos Perez Ramirez
        
        Proyecto de metodos numericos
*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpolacion.c"

int main(int argc, char *argv[]) {
   if (argc != 3) {
      printf("Uso: ./%s <image.pgm> <nueva escala | angulo de rotacion>\n", argv[0]);
      exit(1);
   }
   int i, j, max, ok, rows, cols;
   int count = 0;
   double scale = atof(argv[2]);
   unsigned char **image,**new_img;
   char file_in[80], file_out[80];
   int new_rows, new_cols;
   char outfile[50];


   image = pgmRead(argv[1],&rows,&cols);
   if (image == NULL) {
      printf("Error pgmRead\n");
      exit(1);
   }

   // ===============================================================================================
   // REESCALAMIENTO Y ROTACION CON TODOS LOS METODOS

   
   unsigned char (*funcs[])(double, double, unsigned char**, int, int) = {nothing, nearest_neighbour, bilinear, bicubic};
   char names[][30] = {"nothing_", "nearest_", "bilinear_", "bicubic_"};
   for (int i=0;i<4;i++){
         printf("Interpolacion %s\n", names[i]);

         //new_img = resize(image,rows,cols,rows*scale,cols*scale,funcs[i]); // para reescalar
         new_img = rotate(image,rows,cols,45.0,funcs[i]); // para rotar
         if (new_img==NULL){
                 printf("NO SE PUDO ASIGNAR MEMORIA\n");
                 free_img(image);
                 return 1;
         }
         strcpy(outfile, names[i]);
         strcat(outfile, argv[1]);
         // para escalar
         //ok = pgmWrite(outfile,rows*scale,cols*scale,new_img,NULL); // almacenamiento de la imagen resultante
         
         // para rotar
         new_dimensions(rows, cols, 45.0, &new_rows, &new_cols);
         ok = pgmWrite(outfile,new_rows,new_cols,new_img,NULL); // almacenamiento de la imagen resultante

         free_img(new_img);
   }
   free_img(image);

   // ===============================================================================================
   // PARA MANIPULAR IMAGEN DE TEXTO


   /*unsigned char **new_img2 = resize(image,rows,cols,rows*scale,cols*scale,bilinear);
   if (new_img2==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free_img(image);
      return 1;
   }
   
   rows = rows*scale;
   cols = cols*scale;

   new_img = gaussian7(new_img2,rows,cols);
   if (new_img==NULL){
      printf("NO SE PUDO ASIGNAR MEMORIA\n");
      free_img(image);
      free_img(new_img2);
      return 1;
   }
   
   strcpy(outfile, "manipulated_");
   strcat(outfile, argv[1]);
   ok = pgmWrite(outfile,rows-6,cols-6,new_img,NULL); // almacenamiento de la imagen resultante

   free_img(new_img);
   free_img(new_img2);
   free_img(image);*/

   return 0;
}
