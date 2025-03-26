/* 
JUAN CARLOS PEREZ RAMIREZ
PROGRAMACION Y ALGORITMOS
TAREA 2

El programa acepta un entero ingresado por el usuario, 
lo imprime en sentido inverso y dice si es palindromo o no
*/

#include <stdio.h>

int main(){
	int num, aux, digits, factor, *ptr, *revPtr;

    printf("Ingrese un numero:\n");
	scanf("%d", &num);
	aux = num; // se guarda el valor de num en aux
	digits = 0;
	factor = 1;
	if (num==0) // si el numero ingresado es 0, se tiene 1 digito
		digits = 1;
	while (factor<=num){ // si el numero es diferente de 0, factor se multiplica por 10 hasta ser mayor a num (con esto se encuentra el numero de digitos de num)
		factor *= 10;
		digits++;
	}
	factor /= 10;
	
	int arr[digits],revArr[digits]; // se crea arreglo con la cantidad de digitos del numero ingresado

	ptr = arr; // se asigna a ptr la direccion de memoria del primer elemento de arr (digitos de num)
	revPtr = &revArr[digits-1]; // // se asigna a revPtr la direccion de memoria del ultimo elemento del arreglo de revArr (digitos en sentido inverso de num)
	while (factor>1){ // se emplea la variable factor para extraer el ultimo digito de num y eliminarlo de este
		*ptr = num/factor; // se guarda el digito extraido en el elemento al que apunta ptr (asigna del primero al ultimo)
		*revPtr = *ptr; // se guarda el digito extraido en el elemento al que apunta revPtr (asigna del ultimo al primero)
		num -= *ptr*factor; // se elimina el digito extraido de num
		factor /= 10;
		ptr++;
		revPtr--;
	}
	*ptr = num;
	*revPtr = num;
	ptr = arr; // se reasigna la direccion del primer elemento de arr a ptr

	int palindrome=1;
	for (int i=0;i<digits;i++){ // recorre los elementos de ambos arreglos
		if (*ptr!=*revPtr){ // checa que ambos arreglos sean iguales
			palindrome = 0;
		}
		printf("%d", *revPtr); // imprime los digitos en orden inverso
		ptr++;
		revPtr++;
	}
	if (palindrome==1)
		printf("\n\n%d es palindromo! :D\n", aux);
	else
		printf("\n\n%d NO es palindromo! D:\n", aux);

	return 0;	
}