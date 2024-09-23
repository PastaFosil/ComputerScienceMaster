/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 3

El programa toma un archivo de texto y cuenta la cantidad de bytes que contiene,
las probabilidades de ocurrencia de cada letra, y la probabilidad de que aparezca
una letra dado que la anterior fue una 'e'

Para ejecutar:
./contarLetras texto.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define num_letras 27


int main(int argc, char **argv){
    double letras_prob[num_letras][num_letras], letras_ocurrencias[num_letras],next_letter[num_letras];
    int prev_letter=0, total_bytes=0, lines=0, total_letras=0;
    char ch;

    FILE *file = fopen(argv[1],"r");
    if (file==NULL){
        puts("NO SE PUDO LEER EL ARCHIVO :(\n");
        return -1;
    }

    for (int i=0;i<num_letras;i++){ // inicializa los valores de los arreglos
        letras_ocurrencias[i] = 0.0; // almacenara la cantidad de ocurrencias de cada letra
        next_letter[i] = 0.0; // almacenara la cantidad de letras que siguen a cada una (dada una e, cuantas veces esta seguida de otra letra)
        for (int j=0;j<num_letras;j++){
            letras_prob[i][j] = 0.0; // almacenara la cantidad de ocurrencias de cada letra considerando la letra anterior
        }
    }

    while ((ch=fgetc(file))!=EOF){ // recorre el archivo hasta el final
        total_bytes += 1;

        if (ch==-95||ch==-127) // normalizar a
            ch = 97;
        else if (ch==-87||ch==-119) // normalizar e
            ch = 101;
        else if (ch==-83||ch==-115) // normalizar i
            ch = 105;
        else if (ch==-77||ch==-109) // normalizar o
            ch = 111;
        else if (ch==-70||ch==-102||ch==-68||ch==-100) // normalizar u
            ch = 117;
        else if ((ch==-79)||(ch==-111)) // se convierten ñ y Ñ a 123
            ch = 123;
        
        if ((ch>=65)&&(ch<=90)) // las mayusculas se convierten en minusculas
            ch += 32;
        if ((ch>='a')&&(ch<=123)){ // se cuenta la ocurrencia de las minusculas dada la letra anterior
            if (prev_letter!=0){
                letras_prob[prev_letter-'a'][ch-'a'] += 1.0; // renglon: letra anterior, columna: letra actual
            } else
                letras_ocurrencias[ch-'a'] += 1.0; // renglon: letra anterior, columna: letra actual
            prev_letter = ch;
        } else if (ch==' ') {
            prev_letter = 0;
        } else if (ch=='\n') {
            prev_letter = 0;
            lines++;
        }
    }
    
    for (int i=0;i<num_letras;i++){ // calcula las ocurrencias de cada letra y la cantidad de veces que una letra sigue a otra
        for (int j=0;j<num_letras;j++){
            letras_ocurrencias[i] += letras_prob[j][i]; // ocurrencias de letras
            next_letter[i] += letras_prob[i][j]; // veces que una letra sigue a otra
        }
        total_letras += letras_ocurrencias[i]; // calcula el total de letras
    }

    printf("Numero de lineas: %d\n",lines);
    printf("Numero de bytes: %d\n", total_bytes);
    printf("\n\n");

    printf("X\tP(X)\n");
    for (int i=0;i<num_letras;i++){ 
        letras_ocurrencias[i] /= total_letras; // probabilidad de que aparezca una letra
        printf("%c\t%g\n",i+'a',letras_ocurrencias[i]);
        for (int j=0;j<num_letras;j++){
            if (next_letter[i]>0.0)
                letras_prob[i][j] /= next_letter[i]; // probabilidades condicionales de que una letra siga a otra
        }
    }

    printf("\n\nX\tP(X|Y='e')\n");
    for (int j=0;j<num_letras;j++){
        printf("%c\t%g\n",j+'a',letras_prob['e'-'a'][j]);
    }
}
