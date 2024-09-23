/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 3

El programa toma un archivo de texto y cuenta la cantidad de bytes que contiene,
las probabilidades de ocurrencia de cada letra, y la probabilidad de que aparezca
una letra dado que la anterior fue una 'e'
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int rows, total_elements=0, *length, *arr, **structured_arr;
    printf("Numero de renglones: "); // pide el numero de renglones
    scanf("%d",&rows);
    length = (int *)calloc(rows,sizeof(int)); // arreglo con las longitudes de cada renglon
    if (length==NULL){
        puts("NO SE PUDO ASIGNAR MEMORIA :(\n");
        return -1;
    }
    
    for (int i=0;i<rows;i++){ // llena el arreglo de las longitudes con la informacion de cada renglon
        printf("Ingrese longitud de renglon %d: ",i+1);
        scanf("%d",length+i);
        total_elements += *(length+i);
    }

    arr = (int *)calloc(total_elements,sizeof(int)); // arreglo con el total de elementos
    if (arr==NULL){
        puts("NO SE PUDO ASIGNAR MEMORIA :(\n");
        return -1;
    }

    // asigna memoria para el puntero doble
    structured_arr = (int **)malloc(rows*sizeof(int *));
    if (structured_arr==NULL){
        puts("NO SE PUDO ASIGNAR MEMORIA :(\n");
        return -1;
    }
    printf("\n");
    // guarda las direcciones del primer elemento de cada renglon
    int last=0;
    for (int i=0;i<=rows;i++){
        structured_arr[i] = arr+last;
        last += length[i];
    }

    for (int i=0;i<rows;i++){ // imprime el arreglo con su estructura original
        int *ptr=structured_arr[i];
        while (ptr<=structured_arr[i+1]-1){
            printf("%d ",*ptr);
            ptr++;
        }
        printf("\n");
    }

    int ordered=0, aux;
    while (ordered==0){ // ordena los elementos de length de menor a mayor
        ordered = 1;
        for (int i=1;i<rows;i++){
            if (length[i]<length[i-1]){
                ordered = 0;
                aux = length[i];
                length[i] = length[i-1];
                length[i-1] = aux;
            }
        }
    }
    printf("\n");
    last=0; // asigna las direcciones del primer elemento de cada renglon con el nuevo orden
    for (int i=0;i<=rows;i++){
        structured_arr[i] = arr+last;
        last += length[i];
    }

    for (int i=0;i<rows;i++){ // imprime el arreglo con su nueva estructura
        int *ptr=structured_arr[i];
        while (ptr<=structured_arr[i+1]-1){
            printf("%d ",*ptr);
            ptr++;
        }
        printf("\n");
    }


    free(length);
    free(arr);
    free(structured_arr);
    
}