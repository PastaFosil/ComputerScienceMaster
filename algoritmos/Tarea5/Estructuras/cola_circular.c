/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5
 * 
 * Se crea la estructura de datos cola circular con 
 * operaciones basicas
************************************************************/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
};

struct circular_queue {
    struct node *front, *rear;
    int size;
};

struct node *create_node(int x){
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    new_node->data = x;
    new_node->next = NULL;

    return new_node;
}

struct circular_queue *create_q() {
    struct circular_queue *q = (struct circular_queue *)malloc(sizeof(struct circular_queue));
    if (q==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }

    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return q;
}

void impresion(struct circular_queue *q){
    struct node *temp = q->front;
    
    if (q->size==0){
        printf("COLA VACIA");
        return;
    }

    printf("%d -> ", temp->data);
    temp = temp->next;
    while (temp!=q->front) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
}

void insercion(struct circular_queue **q, int x){
    if ((*q)->size==0){
        (*q)->front = create_node(x);
        (*q)->front->next = (*q)->front;
        (*q)->rear = (*q)->front;
        (*q)->size = 1;
        return;
    } else {
        struct node *temp = (*q)->rear;
        temp->next = create_node(x);
        (*q)->rear = temp->next;
        temp = temp->next;
        temp->next = (*q)->front;
        (*q)->size += 1;
    }
}

void eliminacion(struct circular_queue **q){
    if ((*q)->size==0){
        printf("COLA VACIA");
        return;
    }
    struct node *temp = (*q)->front;

    if ((*q)->front==(*q)->rear){
        (*q)->front = NULL;
        (*q)->rear = NULL;
    } else {
        (*q)->front = (*q)->front->next;
        (*q)->rear->next = (*q)->front;
    }

    free(temp);
    (*q)->size -= 1;
}

void free_q(struct circular_queue *q){
    struct node *temp = q->front;
    struct node *temp2;

    while (temp!=q->rear){
        temp2 = temp->next;
        free(temp);
        temp = temp2;
    }
    free(q->rear);
    free(q);
}

int main(){
    struct circular_queue *q = create_q();
    int choice = 0;
    int x;

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
                insercion(&q, x);
                break;
            case 2:
                eliminacion(&q);
                break;
            case 3:
                printf("\n\n====================================\n\n");
                impresion(q);
                break;
            case 4:
                break;
            default:
                printf("\n\n====================================\n\n");
                printf("INGRESE UNA OPCION VALIDA");
        }
    }

    free_q(q);
}