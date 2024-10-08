/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 6
 * 
 * Se construye la estructura de datos de tabla hash 
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLMAX 1
#define HASH_GROWTH 10

typedef struct node {
    char *data;
    struct node *next;
}NODE;

typedef struct hash {
    int colisions;
    NODE *next;
}HASH;

// Imprime la lista de un indice de la tabla
void print_list(HASH *head){
    NODE *temp;
    
    if (head==NULL||head->next==NULL){
        printf("LA LISTA ESTA VACIA\n");
        return;
    }
    temp = head->next;
    while (temp!=NULL) {
        printf("%s -> ", temp->data);
        temp = temp->next;
    }
    printf("\n");
    return;
}

// Imprime la tabla completa
void print_hash(HASH **table, int size){
    for (int i=0;i<size;i++)
        print_list(table[i]);
}

// Crea un nodo de la lista de nombres
NODE *create_node(char *str){
    // asigna memoria de nodo
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    if (new_node==NULL){
        printf("CREATE NODE. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    // asigna memoria de string
    new_node->data = (char *)malloc(strlen(str)+1);
    if (new_node->data==NULL){
        printf("CREATE NODE. ERROR ASIGNANDO MEMORIA DE STRING\n");
        free(new_node);
        return NULL;
    }
    
    // inicializa valores del nodo
    strcpy(new_node->data, str);
    new_node->next = NULL;

    return new_node;
}

// Inserta str en la lista a la que apunta head segun su orden lexicografico y devuelve el numero de colisiones
int insert_node(HASH **head, char *str){
    // actualiza los punteros hasta encontrar la posicion en la que corresponde str
    NODE *prev = NULL;
    NODE *temp = (*head)->next;
    while (temp!=NULL && strcmp(temp->data, str)<0){
        prev = temp;
        temp = temp->next;
    }

    // crea el nodo a ser insertado
    NODE *new_node = create_node(str);
    if (new_node==NULL){
        printf("INSERT. ERROR ASIGNANDO MEMORIA\n");
        return -1;
    }

    // actualiza los punteros de la lista
    if (prev==NULL){
        new_node->next = (*head)->next;
        (*head)->next = new_node;
    } else {
        prev->next = new_node;
        if (temp==NULL)
            new_node->next = NULL;
        else
            new_node->next = temp;
    }

    return ++(*head)->colisions;
}

// Libera la memoria de la lista ligada
void free_list(HASH *head){
    NODE *temp = head->next;
    NODE *temp2;

    while (temp!= NULL){
        temp2 = temp->next;
        free(temp->data);
        free(temp);
        temp = temp2;
    }
}

// Libera la memoria de la tabla hash
void free_hash(HASH **table, int size){
    for (int i=0;i<size;i++){
        free_list(table[i]);
        free(table[i]);
    }
    free(table);
}

// Crea una tabla hash del tamano especificado
HASH **create_hash(int size){
    // asigna memoria para tabla
    HASH **table = (HASH **)malloc(size*sizeof(HASH *));
    if (table==NULL){
        printf("CREATE. PROBLEMA ASIGNANDO MEMORIA\n");
        return NULL;
    }

    // asigna memoria para las listas y las inicializa
    for (int i=0;i<size;i++){
        table[i] = (HASH *)malloc(sizeof(HASH));
        if (table[i]==NULL){
            printf("CREATE. PROBLEMA ASIGNANDO MEMORIA\n");
            for (int j=0;j<i;j++)
                free(table[j]);
            return NULL;
        }
        table[i]->colisions = 0;
        table[i]->next = NULL;
    }
    
    return table;
}

// Calcula el indice que ocupara str en la tabla hash (funcion djb2)
unsigned long hash_function(char *str, int size){
    unsigned long hash = 5381;
    char *c = str;

    while (*c!='\0'){
        hash = ((hash<<5)+hash)+*c;
        c++;
    }

    return hash % size;
}

// Asigna el elemento al indice calculado por hash_function y lo inserta en el, de vuelve el numero de colisiones
int asign_element(HASH **table, char *str, int size){
    int index = hash_function(str, size);
    return insert_node(&table[index], str);
}

// Si table es NULL, crea una tabla hash de tamano old_size, si no, reasigna espacio para albergar old_size+HASH_GROWTH elementos
int rehash(HASH ***table, int *old_size){
    // nuevo tamano de la tabla
    int new_size = *old_size + HASH_GROWTH;
    // asigna memoria a la nueva tabla
    HASH **new_table = create_hash(new_size);
    if (new_table==NULL){
        printf("REHASH. PROBLEMA ASIGNANDO MEMORIA\n");
        return -1;
    }
    
    // llena la nueva tabla con los valores guardados hasta el momento
    for (int i=0;i<*old_size;i++){
        NODE *temp_node = (*table)[i]->next; // apuntador a los nodos de la lista del elemento temp+i
        while (temp_node!=NULL){
            if (asign_element(new_table,temp_node->data,new_size)==-1){
                printf("REHASH. PROBLEMA LLENANDO NUEVA TABLA\n");
                free_hash(new_table,new_size);
                return -1;
            }
            temp_node = temp_node->next;
        }
    }
    
    // reasignacion de los valores de la tabla y actualizacion del tamano
    free_hash(*table, *old_size);
    *table = new_table;
    *old_size = new_size;

    return 0;
}

// Llena la tabla hash y realiza un rehash si es necesario
int fill_hash(HASH ***table, char *str, int *size){
    // insercion de nuevo elemento en la tabla
    int result = asign_element(*table, str, *size);
    if (result==-1){
        printf("FILL. OCURRIO UN PROBLEMA\n");
        return -1;
    } else if (result>COLMAX) { // si se excede el numero de colisiones permitido, se aumenta el tamano de la tabla
        result = rehash(table, size);
        if (result==-1){
            printf("FILL. OCURRIO UN PROBLEMA\n");
            return -1;
        }
    }

    return result;
}

int main(){
    int size = 10;
    int result;
    
    HASH **table = create_hash(size);
    result = fill_hash(&table,"apple",&size);
    result = fill_hash(&table,"banana",&size);
    result = fill_hash(&table,"apples",&size);
    result = fill_hash(&table,"appleseed",&size);
    result = fill_hash(&table,"appla",&size);
    
    printf("\n");
    print_hash(table,size);
    printf("\n");

    free_hash(table, size);

    return 0;
}