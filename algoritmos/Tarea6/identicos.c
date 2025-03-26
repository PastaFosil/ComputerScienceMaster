/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 6
 * 
 * Se construye la estructura de datos arbol binario y se 
 * analiza si un arbol ingresado es subarbol de otro
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *left;
    char *data;
    struct node *right;
}NODE;

NODE *create_node(char *str){
    NODE *t;

    t = (NODE *)malloc(sizeof(NODE));
    if (t==NULL){
        printf("CREATE. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    t->data = str;
    t->left = t->right = NULL;
    return(t);
}

NODE *insert(NODE *root, char *str){
    if (root==NULL)
        return create_node(str);
    
    if (strcmp(root->data,str)>0)
        root->right = insert(root->right, str);
    else if (strcmp(root->data,str)<0)
        root->left = insert(root->left, str);
    
    return root;
}

void show_tree(NODE *node){
    if (node != NULL){
        printf("%s ", node->data);
        if (node->left!=NULL)
            show_tree(node->left);
        if (node->right!=NULL)
            show_tree(node->right);
        printf("\n");
    } else
        printf("ARBOL VACIO\n");
}

// Imprime los elementos de t2 que no estan en t2
void identic(NODE *t1, NODE *t2){
    if (t2==NULL)
        return;
    
    if (strcmp(t1->data,t2->data)!=0)
        printf("%s\n", t2->data);

    if (t1->left!=NULL)
        identic(t1->left, t2->left);
    else if (t1->left==NULL)
        identic(t1, t2->left);
    
    if (t1->right!=NULL)
        identic(t1->right, t2->right);
    else if (t1->right==NULL)
        identic(t1, t2->right);
}

void cut_tree(NODE *t){
    if (t!=NULL){
        // revisa si el nodo actual tiene ramas
        if (t->left!=NULL)
            cut_tree(t->left);
        if (t->right!=NULL)
            cut_tree(t->right);
        free(t); // si no hay ramas, libera la memoria del nodo actual
    }
}

int main(){
    NODE *root = NULL;
    NODE *sub = NULL;

    root = insert(root,"Juan");
    root = insert(root,"Pepe");
    root = insert(root,"Alex");
    root = insert(root,"Roman");
    root = insert(root,"Angela");
    root = insert(root,"Adriana");
    
    sub = insert(sub,"Juan");
    sub = insert(sub,"Pepe");
    sub = insert(sub,"Alex");
    sub = insert(sub,"Ramon");
    sub = insert(sub,"Monica");
    sub = insert(sub,"Adriana");
    
    identic(root, sub);
    
    //show_tree(root);
    //printf("\n\n");
    //show_tree(sub);

    cut_tree(root);
    cut_tree(sub);
}