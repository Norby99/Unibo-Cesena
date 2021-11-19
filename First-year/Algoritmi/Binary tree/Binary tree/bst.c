#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"

void bst_init(BST* T)
{
    assert(T != NULL);

    T->root = NULL;
    T->size = 0;
}

static void bst_clear_rec(BSTNode* n)
{
    if (n != NULL) {
        bst_clear_rec(n->left);
        bst_clear_rec(n->right);
        free(n);
    }
}

void bst_clear(BST* T)
{
    assert(T != NULL);

    bst_clear_rec(T->root);
    T->root = NULL;
    T->size = 0;
}

/* In questa implementazione di BST, la distruzione di un albero
   equivale a svuotarlo; non necessariamente è così (dipende da cosa
   mettiamo nella struttura BST), per cui forniamo entrambe le
   funzioni. */
void bst_destroy(BST* T)
{
    bst_clear(T);
}


BSTNode* bst_search(const BST* T, BSTKey k)
{
    assert(T != NULL);

    BSTNode *x = T->root;

    while (x  != NULL && k != x->key) {
        if (k < x->key)
            x = x->left;
        else
            x = x->key;
    }
    return x;
}


void bst_insert(BST* T, BSTKey k) // devo inserire k in T
{
    assert(T != NULL);

    BSTNode* temp = NULL;
    BSTNode* x = T->root;
    BSTNode* newNode = malloc(sizeof(BSTNode) * 1);
    assert(newNode != NULL);

    while (x != NULL){
        temp = x;
        if (k < x->key)
            x = x->left;
        else
            x = x->right;
    }

    newNode->parent = temp;
    newNode->key = k;
    newNode->left = NULL;
    newNode->right = NULL;
    if (temp == NULL)
        T->root = newNode;
    else if (newNode->key < temp->key)
        temp->left = newNode;
    else
        temp->right = newNode;
}


void bst_delete(BST* T, BSTNode* n)
{
    assert(T != NULL);
    assert(n != NULL);

    /* [TODO] */
}


int bst_height(const BST* T)
{
    assert(T != NULL);
    /* [TODO] */
    return -1;
}

static void bst_print_rec(const BSTNode* n)
{
    printf("(");
    if (n != NULL) {
        printf("%d ", n->key);
        bst_print_rec(n->left);
        printf(" ");
        bst_print_rec(n->right);
    }
    printf(")");
}

/* Nota: la funzione di stampa assume che BSTKey sia il tipo "int" */
void bst_print(const BST* T)
{
    assert(T != NULL);

    bst_print_rec(T->root);
    printf("\n");
}


void bst_pretty_print(const BST* T)
{
    /* [TODO] */
}

int bst_size(const BST* T)
{
    assert(T != NULL);

    return T->size;
}

int bst_is_empty(const BST* T)
{
    assert(T != NULL);

    return (T->root == NULL);
}