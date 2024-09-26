/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5
 * 
 * Se crea la estructura de datos lista ligada con operaciones
 * basicas
************************************************************/

#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *next;
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

int impresion(struct node *head){
    struct node *temp;
    int i = 0; // cuenta el numero de nodos para su uso en otras funciones
    
    if (head==NULL){
        printf("LA LISTA ESTA VACIA\n");
        return -1;
    }
    temp = head;
    while (temp!=NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
        i++;
    };

    return i;
}

void insercion(struct node **head, int x){
    if (*head==NULL){
        *head = create_node(x);
        return;
    } else {
        int n, index, j;
        struct node *temp = *head;
        struct node *new_node;

        printf("Elija en que posicion insertar el nuevo dato:\n");
        printf("Datos:\t");
        n = impresion(*head);
        printf("\n");
        printf("Indice:\t");
        for (j=0;j<n;j++){
            printf("%d -> ",j);
            temp = temp->next;
        }
        printf("%d\n", j);
        printf("R: ");
        scanf("%d",&index);

        if (index>n||index<0) {
            printf("NO SE INGRESO UNA POSICION VALIDA\n");
            return;
        }

        if (index==0){
            new_node = create_node(x);
            new_node->next = *head;
            *head = new_node;
        } else if (index==n){
            temp = *head;
            for (int i=1;i<n;i++)
                temp = temp->next;
            temp->next = create_node(x);
        } else {
            temp = *head;
            for (int i=0;i<index-1;i++){
                temp = temp->next;
            }
            new_node = create_node(x);
            new_node->next = temp->next;
            temp->next = new_node;
        }
    }
}

void eliminacion(struct node **head){
    if (*head==NULL){
        printf("LISTA VACIA\n");
        return;
    } else {
        int n, index, j;
        struct node *temp = *head;

        printf("Elija en que posicion del dato a eliminar:\n");
        printf("Datos:\t");
        n = impresion(*head);
        printf("\n");
        printf("Indice:\t");
        for (j=0;j<n;j++){
            printf("%d -> ",j);
            temp = temp->next;
        }
        printf("\n");
        printf("R: ");
        scanf("%d",&index);

        if (index>n||index<0) {
            printf("NO SE INGRESO UNA POSICION VALIDA\n");
            return;
        }

        if (index==0){
            temp = *head;
            free(*head);
            *head = temp->next;
        } else if (index==n-1){
            temp = *head;
            for (int i=1;i<n-1;i++)
                temp = temp->next;
            free(temp->next);
            temp->next = NULL;
        } else {
            struct node *temp2;
            temp = *head;
            temp2 = temp->next;
            for (int i=0;i<index-1;i++){
                temp = temp->next;
                temp2 = temp->next;
            }
            temp->next = temp2->next;
            free(temp2);
        }
    }
}

void invertir_lista(struct node **head){
    struct node *temp1 = NULL;
    struct node *temp2 = *head;
    struct node *temp3;

    if (*head==NULL){
        printf("LISTA VACIA\n");
        return;
    }

    while (temp2!=NULL){
        temp3 = temp2->next;
        temp2->next = temp1;
        temp1 = temp2;
        temp2 = temp3;
    }

    *head = temp1;
}

void free_list(struct node *head){
    struct node *temp = head;
    struct node *temp2;

    while (temp!= NULL){
        temp2 = temp->next;
        free(temp);
        temp = temp2;
    }
    
}

int main(){
    struct node *head = NULL;
    int choice = 0;
    int x;

    while (choice!=5){
        printf("\n\n====================================\n\n");
        printf("Ingrese la operacion deseada:\n\n");
        printf("1. Inserción\n2. Eliminación\n3. Invertir lista ligada\n4. Impresión\n5. Fin\n\n");
        printf("R: ");
        scanf("%d", &choice);

        switch (choice){
            case 1:
                printf("\n\nElemento a ingresar: ");
                scanf("%d", &x);
                insercion(&head, x);
                break;
            case 2:
                eliminacion(&head);
                break;
            case 3:
                invertir_lista(&head);
                break;
            case 4:
                printf("\n\n====================================\n\n");
                impresion(head);
                break;
            case 5:
                break;
            default:
                printf("\n\n====================================\n\n");
                printf("INGRESE UNA OPCION VALIDA");
        }
    }

    free_list(head);
    
}