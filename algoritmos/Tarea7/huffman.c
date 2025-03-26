/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 7
 * 
 * Comprime una imagen mediante el cifrado de Huffman
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "tools.c"

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s imagen.pgm\n",argv[0]);
        return -1;
    }

    int i, j, max, ok, rows, cols;
    int count = 0;
    unsigned char **image;
    char *filename = argv[1]; // nombre del archivo de input

    // lectura de la imagen
    image = pgmRead(filename,&rows,&cols);
    if (image == NULL) {
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        exit(1);
    }
    long long size = rows*cols; // total de pixeles en la imagen
    

    // calculo del histograma
    int *hist = histogram(image,rows,cols);

    // generacion del arbol de la codificacion de Huffman
    NODE *root;
    huffman_code(&root, hist);

    // almacenamiento de los codigos de cada valor de intensidad
    char **codes = get_codes(root);

    // calculo de la longitud de codigo esperada (por caracter)
    double mean = 0;
    for (int i=0;i<=MAXVALUE;i++)
        mean += (double)(strlen(codes[i])*hist[i]);
    mean /= (double)size;

    // generacion del nombre del archivo de la imagen comprimida
    filename[strlen(filename)-4] = '\0';
    strcat(filename, "_huffman.bin");
    // compresion de la imagen y almacenamiento en archivo bin, devuelve el numero de bits escritos
    long long new_size = huffman_cipher(filename, image, rows, cols, codes);

    // calculo de la razon de compresion
    double cr = 0.0;
    for (int i=0;i<=MAXVALUE;i++)
        cr += (double)(hist[i]*strlen(codes[i]));
    cr = (double)(8*size)/cr;

    double bpp = (double)new_size/(double)size; // bits-per-pixel

    printf("Tamano de la imagen original: %lld bits\n", size*8);
    printf("Tamano de la imagen cifrada: %lld bits\n", new_size);
    printf("Longitud de codigo esperada: %lf bits\n", mean);
    printf("Razon de compresion: %lf\n", cr);
    printf("Bits-per-pixel: %lf\n", bpp);
    /*
    CR y bpp difieren porque ambos miden cosas diferentes:
    - CR calcula cuantas veces mas pequena es la imagen comprimida respecto de la original
    - bpp calcula cuantos bits se necesitan en promedio para guardar la informacion de un pixel
    */
    
    free_img(image);
    for (int i=0;i<=MAXVALUE;i++)
        free(codes[i]);
    free(codes);
    free(hist);

    image = huffman_decipher(filename,&rows, &cols,root);
    ok = pgmWrite("deciphered.pgm",rows,cols,image,NULL);
    printf("\n");

    free_img(image);
    cut_tree(root);
    return 0;
}