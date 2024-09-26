/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5
 * 
 * Se crea la estructura de datos pila con operaciones
 * basicas
************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define N 5

struct stack {
    int stackList[N];
    int top;
};

void insercion(struct stack *pila, int x) {
    if (pila->top<N-1){
        pila->top += 1;
        pila->stackList[pila->top] = x;
    } else
        printf("STACK LLENO\n");
}

void eliminacion(struct stack *pila){
    if (pila->top>-1)
        pila->top -= 1;
    else{
        printf("\n\n====================================\n\n");
        printf("STACK VACIO\n");
    }
}

void impresion(struct stack pila){
    if (pila.top>-1){
        printf("\n\n====================================\n\n");
        printf("Stack actual:\n\n");
        printf("Indice\tElemento\n");
        for (int i=pila.top;i>-1;i--){
            printf("%d\t%d\n", i, pila.stackList[i]);
        }
    } else{
        printf("\n\n====================================\n\n");
        printf("STACK VACIOS");
    }
}

int main(){
    struct stack p;
    p.top = -1;
    int choice = 0, x;

    while (choice!=4){
        printf("\n\n====================================\n\n");
        printf("Ingrese la operacion deseada:\n\n");
        printf("1. Inserción\n2. Eliminación\n3. Impresión\n4. Fin\n\n");
        printf("R: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                printf("\n\nElemento a ingresar: ");
                scanf("%d", &x);
                insercion(&p, x);
                break;
            case 2:
                eliminacion(&p);
                break;
            case 3:
                impresion(p);
                break;
            case 4:
                break;
            default:
                printf("\n\n====================================\n\n");
                printf("INGRESE UNA OPCION VALIDA");
        }
    }

    return 0;
}