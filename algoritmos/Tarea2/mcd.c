/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 2

El programa encuentra el maximo comun divisor entre los dos
numeros de una fraccion y los reduce a sus terminos minimo
*/

#include <stdio.h>

int main(){
    int a, b, mcd=1, min, div=1;

    printf("Ingrese los dos numeros de la fraccion a/b:\n");
    printf("a = ");
    scanf("%d",&a);
    printf("b = ");
    scanf("%d",&b);

    if (a<b) // se encuentra el menor numero de los dos
        min = a;
    else
        min = b;
    while (div<=min){ // se prueban divisores de 1 hasta min
        if ((a%div==0)&&(b%div==0)) // si se encuentra un divisor comun, se guarda en mcd
            mcd = div;
        div++;
    }
    printf("%d/%d = %d/%d\n", a,b,a/mcd,b/mcd); // se devuelve el resultado con el mayor divisor comun encontrado
    
    return 0;
}