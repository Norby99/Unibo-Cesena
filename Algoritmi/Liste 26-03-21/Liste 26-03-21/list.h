#ifndef LIST_H
#define LIST_H

typedef int ListInfo;

typedef struct ListNode {
    ListInfo val;
    struct ListNode* succ, * pred;
} ListNode;

typedef struct {
    int length;
    ListNode sentinel;
} List;

/* Inizializza una lista vuota */
void list_init(List* L);

/* Restituisce la lunghezza (numero di nodi) della lista L; se L e' la
   lista vuota, restituisce 0 */
int list_length(List* L);

/* Libera la memoria occupata da tutti i nodi della lista L */
void list_destroy(List* L);

/* Idem */
void list_remove_all(List* L);

/* Restituisce 1 se la lista è vuota, 0 altrimenti */
int list_is_empty(List* L);

/* Restituisce il puntatore ad un nodo contenente k in L; se k non è
   presente, restituisce un puntatore alla sentinella */
ListNode* list_search(List* L, ListInfo k);

/* Restituisce il puntatore al primo nodo di L; se L è vuota,
   restituisce un puntatore alla sentinella */
ListNode* list_first(List* L);

/* Restituisce il puntatore all'ultimo nodo di L; se L è vuota,
   restituisce un puntatore alla sentinella */
ListNode* list_last(List* L);

/* Inserisci un nuovo nodo contenente k all'inizio della lista */
void list_add_first(List* L, ListInfo k);

/* Inserisci un nuovo nodo contenente k alla fine della lista */
void list_add_last(List* L, ListInfo k);

/* Rimuove il nodo n dalla lista */
void list_remove(List* L, ListNode* n);

/* Rimuove il primo nodo della lista e ne restituisce il valore */
ListInfo list_remove_first(List* L);

/* Rimuove l'ultimo nodo della lista e ne restituisce il valore */
ListInfo list_remove_last(List* L);

/* Restituisce il successore del nodo n */
ListNode* list_succ(const ListNode* n);

/* Restituisce il predecessore del nodo n */
ListNode* list_pred(const ListNode* n);

/* Ritorna un puntatore all'n-esimo elemento di L (n = 0,
   ... list_length(L)-1).  Se l'elemento non esiste, ritorna un
   puntatore alla sentinella */
ListNode* list_nth_element(List* L, int n);

/* Concatena gli elementi di L2 in coda a quelli di L1. Questa
   funzione non crea nuovi nodi, ma modificare L1 e L2. Al termine di
   questa funzione, la lista L2 diventa la lista vuota. */
void list_concat(List* L1, List* L2);

/* Ritorna 1 se e solo se le liste L1 e L2 contengono gli stessi valori,
   nello stesso ordine. */
int list_equal(List* L1, List* L2);

/* Stampa il contenuto della lista */
void list_print(List* L);

/* Ritorna l'indirizzo della sentinella */
ListNode* list_end(List* L);

#endif