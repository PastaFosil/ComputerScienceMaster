/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 6
 * 
 * Se construye la estructura de datos arbol binario e imprime 
 * los ancestros de un nodo en particular
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *left;
    int data;
    struct node *right;
}NODE;

NODE *create_node(int val){
    NODE *t;

    t = (NODE *)malloc(sizeof(NODE));
    if (t==NULL){
        printf("CREATE. ERROR ASIGNANDO MEMORIA\n");
        return NULL;
    }
    t->data = val;
    t->left = t->right = NULL;
    return(t);
}

NODE *insert(NODE *root, int val){
    if (root==NULL)
        return create_node(val);
    
    if (val > root->data)
        root->right = insert(root->right, val);
    else if (val < root->data)
        root->left = insert(root->left, val);
    
    return root;
}

// Imprime arbol completo
void show_tree(NODE *node){
    if (node != NULL){
        printf("%d ", node->data);
        if (node->left!=NULL)
            show_tree(node->left);
        if (node->right!=NULL)
            show_tree(node->right);
        printf("\n");
    } else
        printf("ARBOL VACIO\n");
}

// Imprime los ancestros de un nodo en particular
int ancestry(NODE *root, int val){
    if (root==NULL){
        printf("NO EXISTE EL NODO INGRESADO\n");
        return 0;
    }

    int found = 0;

    if (val > root->data)
        found = ancestry(root->right, val);
    else if (val < root->data)
        found = ancestry(root->left, val);

    // busca el valor deseado, lo imprime y marca como encontrado e imprime sus ancestros si es hoja
    if (root->data==val){
        if (root->left==NULL && root->right==NULL){
            printf("%d ", root->data);
            return 1;
        } else {
            printf("EL NODO INGRESADO NO ES HOJA\n");
            return 0;
        }
    }

    if (found==1){
        printf("%d ", root->data);
        return 1;
    }

    return found;
}

// Libera la memoria de todos los nodos de un arbol binario
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

    root = insert(root,1);
    root = insert(root,3);
    root = insert(root,2);
    root = insert(root,7);
    root = insert(root,6);
    root = insert(root,4);
    root = insert(root,5);
    root = insert(root,9);
    root = insert(root,8);

    show_tree(root);

    ancestry(root, 9);
    printf("\n");

    cut_tree(root);
}