/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5

 * Se crean estructuras de describen un punto o un 
 * rectangulo y se analizan usando funciones
************************************************************/

#include <stdio.h>

int main(){
    int n = 6;
    char *str[6] = {"Juan 6.0 23 A Matutino",
                   "Isaac 7.0 24 B Matutino",
                   "Nayeli 8.0 27 C Matutino",
                   "Kathi 9.0 23 D Vespertino",
                   "Miriam 10.0 23 E Vespertino",
                   "Tapia 10.0 25 F Vespertino"
                };

    write_students("alumnos.txt", str, n);

    return 0;
}