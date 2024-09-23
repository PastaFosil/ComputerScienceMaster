/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 3

El programa toma un arreglo de enteros sin signo y encuentra sus
tres elementos enteros mayores y sus tres bytes mayores
*/

#include <stdio.h>

void type_three_largest(int *a, int len_a, int **largest, int *index_largest, int **second_largest, int *index_second_largest, int **third_largest, int *index_third_largest){
    /*
    Encuentra los tres enteros mayores presentes en el arreglo 
    y devuelve sus indices y sus direcciones de memoria
    */

    // se asigna a los apuntadores de los tres numeros la direccion del primer elemento de a
    *largest = a;
    *second_largest = a;
    *third_largest = a;

    // se asigna a los indices de los tres numeros el del primer elemento de a
    *index_largest = *index_second_largest = *index_third_largest = 0;

    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el mayor
        if (*(a+i)>**largest){
            *largest = a+i; // almacena la direccion de memoria del mayor
            *index_largest = i; // almacena el indice del mayor
        }
    }
    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el segundo mayor
        if ((*(a+i)>**second_largest)&&(i!=*index_largest)){ // se salta el indice donde se encuentra el mayor
            *second_largest = a+i; // almacena la direccion de memoria del segundo mayor
            *index_second_largest = i; // almacena el indice del segundo mayor
        }
    }
    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el tercero mayor
        if ((*(a+i)>**third_largest)&&(i!=*index_largest)&&(i!=*index_second_largest)){ // se salta el indice donde se encuentran el mayor y el segundo mayor
            *third_largest = a+i; // almacena la direccion de memoria del tercero mayor
            *index_third_largest = i; // almacena el indice del tercero mayor
        }
    }
}


void type_three_largest_byte(int *a, int len_a, unsigned char **largest, int *index_largest, unsigned char **second_largest, int *index_second_largest, unsigned char **third_largest, int *index_third_largest){
    /*
    Encuentra los tres bytes mayores presentes en el arreglo 
    y devuelve sus indices y sus direcciones de memoria
    */

    unsigned char *arr; // crea un apuntador para bytes individuales
    arr = (unsigned char *)a; // asigna a arr la direccion del primer byte de a
    
    // se asigna a los apuntadores de los tres numeros la direccion del primer elemento de a
    *largest = arr;
    *second_largest = arr;
    *third_largest = arr;
    
    // se asigna a los indices de los tres numeros el del primer elemento de a
    *index_largest = *index_second_largest = *index_third_largest = 0;

    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el mayor byte
        if (*(arr+i)>**largest){
            *largest = arr+i; // almacena la direccion de memoria del mayor byte
            *index_largest = i; // almacena el indice del mayor byte
        }
    }
    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el segundo mayor byte
        if ((*(arr+i)>**second_largest)&&(i!=*index_largest)){ // se salta el indice donde se encuentra el mayor byte
            *second_largest = arr+i; // almacena la direccion de memoria del segundo mayor byte
            *index_second_largest = i; // almacena el indice del segundo mayor byte
        }
    }
    for (int i=1;i<len_a;i++){ // recorre el arreglo para encontrar el tercero mayor byte
        if ((*(arr+i)>**third_largest)&&(i!=*index_largest)&&(i!=*index_second_largest)){ // se salta el indice donde se encuentran el mayor y el segundo mayor bytes
            *third_largest = arr+i; // almacena la direccion de memoria del tercero mayor byte
            *index_third_largest = i; // almacena el indice del tercero mayor byte
        }
    }
}

int main(){
    unsigned int arr[] = {1,2,3, 87, 500, 764, 342, 1009, 8005};
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // declaracion de las variables empleadas en busqueda de los enteros mayores
    int len, *largest, index_largest, *second_largest, index_second_largest, *third_largest, index_third_largest;
    len = sizeof(arr)/sizeof(arr[0]); // longitud del arreglo

    type_three_largest(arr,len,&largest,&index_largest,&second_largest,&index_second_largest,&third_largest,&index_third_largest);
    printf("Los tres enteros mayores del arreglo son:\n");
    printf("1: arr[%d]=%u\n",index_largest,*largest);
    printf("2: arr[%d]=%u\n",index_second_largest,*second_largest);
    printf("3: arr[%d]=%u\n",index_third_largest,*third_largest);
    
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // declaracion de las variables empleadas en busqueda de los bytes mayores
    unsigned char *largest_byte, *second_largest_byte, *third_largest_byte;
    int len_byte, index_largest_byte, index_second_largest_byte, index_third_largest_byte;
    len_byte = 4*len; // longitud de arr en bytes

    type_three_largest_byte(arr,len_byte,&largest_byte,&index_largest_byte,&second_largest_byte,&index_second_largest_byte,&third_largest_byte,&index_third_largest_byte);
    printf("Los tres bytes mayores del arreglo son:\n");
    printf("1: arr[%d]=%u\n",index_largest_byte,*largest_byte);
    printf("2: arr[%d]=%u\n",index_second_largest_byte,*second_largest_byte);
    printf("3: arr[%d]=%u\n",index_third_largest_byte,*third_largest_byte);
}