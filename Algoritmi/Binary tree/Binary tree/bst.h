#ifndef BST_H
#define BST_H

typedef int BSTKey;

/* Struttura che rappresenta un nodo del BST */
typedef struct BSTNode {
    struct BSTNode* parent, * left, * right;
    BSTKey key;
} BSTNode;

/* Struttura che rappresenta un BST; il campo "size" consente di
   conoscere il numero di nodi dell'albero in tempo O(1), senza
   bisogno di contarli ogni volta. */
typedef struct {
    int size;
    BSTNode* root;
} BST;

/* Inizializza un BST vuoto */
void bst_init(BST* T);

/* Svuota l'albero, liberando la memoria occupata dai nodi */
void bst_clear(BST* T);

/* Distruggi l'albero e il suo contenuto. Dopo questa funzione,
   l'albero potrebbe richiedere di essere reinizializzato con
   bst_init(). */
void bst_destroy(BST* T);

/* Inserisce la chiave k nel BST T. Se la chiave è gi�  presente
   nell'albero, se ne deve inserire una nuova copia (è indifferente se
   nel sottoalbero sinistro o destro). */
void bst_insert(BST* T, BSTKey k);

/* Restituisce il nodo contenente la chiave k; se la chiave non è
   presente, restituisce NULL. */
BSTNode* bst_search(const BST* T, BSTKey k);

/* Cancella il nodo n da T. */
void bst_delete(BST* T, BSTNode* n);

/* Restituisce il numero di nodi in T. */
int bst_size(const BST* T);

/* Restituisce 1 se e solo se l'albero è vuoto */
int bst_is_empty(const BST* T);

/* Restituisce l'altezza dell'albero T. L'altezza di un albero vuoto
   si considera -1; l'altezza di un albero composto dalla sola radice
   si considera 0. */
int bst_height(const BST* T);

/* Stampa il contenuto dell'albero T. */
void bst_print(const BST* T);

/* Stampa il contenuto dell'albero T in modo più "leggibile". */
void bst_pretty_print(const BST* T);

#endif