/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * PARCIAL 2

 * 02/10/2024
************************************************************/


#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

int main(){
    int n;
    long double x = 1.0;
    long double r;

    printf("[");
    for (int i=0;i<20;i++){
        x *= 10.0;
        r = x*(sqrtl(x+1.0)-sqrtl(x));
        printf("%Lf, ", r);
    }
    printf("\n\n");

    x = 1.0;
    for (int i=0;i<20;i++){
        x *= 10.0;
        r = x/(sqrtl(x+1.0)+sqrtl(x));
        printf("%Lf, ", r);
    }
    printf("\n");
}