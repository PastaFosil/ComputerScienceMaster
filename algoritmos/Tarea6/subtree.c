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

int subtree(NODE *root, NODE *sub){
    int is_it = 0;
    if (root->data==sub->data){
        // si ambos nodos tienen rama izquierda, revisa si ambas coinciden
        if (root->left!=NULL && sub->left!=NULL)
            is_it = subtree(root->left, sub->left);
        // si ninguno tiene rama izquierda, continua la comparacion con la derecha
        else if (root->left==NULL && sub->left==NULL)
            is_it = 1;
        else return 0; // si uno tiene y el otro no, no es subarbol del otro
        // si ambos nodos tienen rama derecha, revisa si ambas coinciden
        if (root->right!=NULL && sub->right!=NULL)
            is_it = subtree(root->right, sub->right);
        // si ninguno tiene rama izquierda, continua la comparacion con la derecha
        else if (root->right==NULL && sub->right==NULL)
            is_it = 1;
        else return 0; // si uno tiene y el otro no, no es subarbol del otro

        return 1;
    } else {
        if (root->left!=NULL)
            is_it = subtree(root->left, sub);
        if (is_it==0 && root->right!=NULL)
            is_it = subtree(root->right, sub);
        return is_it;
    } 
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

    root = insert(root,5);
    root = insert(root,2);
    root = insert(root,7);
    root = insert(root,1);
    root = insert(root,4);
    root = insert(root,6);
    root = insert(root,8);
    
    sub = insert(sub,7);
    sub = insert(sub,6);
    //sub = insert(sub,8);

    int is_it = subtree(root, sub);
    printf("Subarbol? %d\n",is_it);

    show_tree(root);
    cut_tree(root);
    cut_tree(sub);
}