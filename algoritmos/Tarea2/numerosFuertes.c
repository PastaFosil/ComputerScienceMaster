/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 2

El programa encuentra los numeros fuertes dentro del
intervalo especificado
*/

#include <stdio.h>

int digits(int num){
    // Cuenta los digitos del numero ingresado
    int digits, factor;
    digits = 0;
	factor = 1;
	if (num==0) // si el numero ingresado es 0, se tiene 1 digito
        return 1;
	while (factor<=num){ // si el numero es diferente de 0, factor se multiplica por 10 hasta ser mayor a num (con esto se encuentra el numero de digitos de num)
		factor *= 10;
		digits++;
	}
    return digits;
}

void get_digits(int num, int digits, int *ptr){
    // Extrae los digitos del numero ingresado y los guarda en los ultimos digits elementos del arreglo
    int factor;
    factor = 1;
    for (int i=1;i<digits;i++) // se 
        factor *= 10;
    while (factor>1){ // se emplea la variable factor para extraer el ultimo digito de num y eliminarlo de este
		*ptr = num/factor; // se guarda el digito extraido en el elemento al que apunta ptr (asigna del primero al ultimo)
		num -= *ptr*factor; // se elimina el digito extraido de num
		factor /= 10;
		ptr++;
	}
    *ptr = num;
    ptr -= digits-1;
}

void is_strong(int *ptr, int digits, int fac[]){
    // Revisa si el numero guardado en el arreglo al que apunta ptr es un numero fuerte y lo imprime en caso de serlo
    int num=0, factor=1, sum_factorial=0, current_digit;
    for (int i=1;i<=digits;i++){ // recorre el arreglo de digitos de unidades hacia arriba
        current_digit = *(ptr+digits-i);
        sum_factorial += *(fac+current_digit); // suma el factorial del digito tomado (cuyo valor esta guardado en fac)
        num += current_digit*factor; // reconstruye el numero a partir de sus digitos
        factor *= 10; // eleva el factor a la siguiente potencia de 10
    }
    if (num==sum_factorial){
        printf("%d = %d! ",num, *ptr);
        for (int i=1;i<digits;i++)
            printf("+ %d! ", *(ptr+i));
        printf("es un numero fuerte!\n");
    }
}

int main(){
    int fac[10], a, b, used_digits, bDigits, *ptr;

    *fac = 1;
    for (int i=1;i<10;i++) // se calculan los valores del factorial de cada digito
        *(fac+i) = *(fac+i-1)*i;

    printf("Ingrese los limites del intervalo [a,b]\n");
    printf("a = ");
    scanf("%d",&a);
    printf("b = ");
    scanf("%d",&b);

    bDigits = digits(b); // almacena el numero de digitos de b
    int arr[bDigits]; // se crea un arreglo que almacena maximo bDigits (los digitos del numero mas grande)
    for (int i=a;i<=b;i++){ // recorre todos los numeros del intervalo definido
        used_digits = digits(i); // almacena el numero de digitos de i
        ptr = &arr[bDigits-used_digits]; // posiciona el apuntador para llenar los ultimos used_digits elementos del arreglo
        get_digits(i,used_digits,ptr); // guarda los digitos de a en arr
        is_strong(ptr,used_digits,fac); // imprime un mensaje si el numero actual es fuerte
    }
    
   return 0;
}