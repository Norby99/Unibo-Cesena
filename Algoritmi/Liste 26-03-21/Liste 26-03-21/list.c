#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

/* Crea (mediante malloc() ) e restituisce un puntatore ad un nuovo
   nodo di una lista; il nodo contiene il valore v. I puntatori al
   nodo successivo e precedente puntano entrambi al nodo appena creato */
static ListNode* list_new_node(int v)
{
    ListNode* r = (ListNode*)malloc(sizeof(ListNode));
    assert(r); /* evitiamo un warning con VS */
    r->val = v;
    r->succ = r->pred = r;
    return r;
}

/* Restituisce l'indirizzo di memoria della sentinella di L */
ListNode* list_end(List* L)
{
    assert(L);

    return &(L->sentinel);
}

void list_init(List* L)
{
    assert(L);

    L->length = 0;
    L->sentinel.pred = L->sentinel.succ = &(L->sentinel);
}

int list_length(List* L)
{
    assert(L);

    return L->length;
}


void list_destroy(List* L)
{
    list_remove_all(L);
}

void list_remove_all(List* L)
{
    ListNode* l = L->sentinel.succ;
    assert(l);

    while (l != &L->sentinel) {
        l = l->succ;
        free(l->pred);
    }

    L->sentinel.succ = L->sentinel.pred = &L->sentinel;

    L->length = 0;
    
}

/* Nota: questa funzione assume che ListInfo sia il tipo "int" */
void list_print(List* L)
{
    const ListNode* node;

    assert(L);

    printf("(");
    for (node = list_first(L); node != list_end(L); node = list_succ(node)) {
        printf("%d ", node->val);
    }
    printf(")\n");
}

int list_is_empty(List* L)
{
    assert(L);

    return (list_first(L) == list_end(L));
}

ListNode* list_search(List* L, ListInfo k)
{
    ListNode* l = &L->sentinel;
    assert(l);

    do {
        l = l->succ;
        if (l->val == k) {
            return l;
        }
    } while (l != &L->sentinel);

    return &L->sentinel;
}

ListNode* list_first(List* L)
{
    assert(L);

    return L->sentinel.succ;
}

ListNode* list_last(List* L)
{
    assert(L);

    return L->sentinel.pred;
}


/* Inserisce un nuovo nodo contenente k all'inizio della lista */
void list_add_first(List* L, ListInfo k)
{
    ListNode* l = (ListNode*)malloc(sizeof(ListNode));
    assert(l);
    l->val = k;
    l->pred = &L->sentinel;

    if (L->length == 0) {
        l->succ = &L->sentinel;
        L->sentinel.pred = l;
    }
    else {
        l->succ = L->sentinel.succ;
        L->sentinel.succ->pred = l;
    }
    L->sentinel.succ = l;

    L->length++;
}

/* Inserisce un nuovo nodo contenente k alla fine della lista */
void list_add_last(List* L, ListInfo k)
{
    ListNode* l = (ListNode*)malloc(sizeof(ListNode));
    assert(l);
    l->val = k;
    l->succ = &L->sentinel;

    if (L->length == 0) {
        l->pred = &L->sentinel;
        L->sentinel.succ = l;
    }
    else {
        l->pred = L->sentinel.pred;
        L->sentinel.pred->succ = l;
    }

    L->sentinel.pred = l;

    L->length++;

}

/* Rimuove il nodo n dalla lista L */
void list_remove(List* L, ListNode* n)
{
    ListNode* l = L->sentinel.succ;
    assert(l);

    n->pred->succ = n->succ;
    n->succ->pred = n->pred;
    free(n);

    L->length--;
}

ListInfo list_remove_first(List* L)
{
    ListInfo k = L->sentinel.succ->val;
    list_remove(L, L->sentinel.succ);
    return k;
}

ListInfo list_remove_last(List* L)
{
    ListInfo k = L->sentinel.pred->val;
    list_remove(L, L->sentinel.pred);
    return k;
}

ListNode* list_succ(const ListNode* n)
{
    assert(n);

    return n->succ;
}

ListNode* list_pred(const ListNode* n)
{
    assert(n);

    return n->pred;
}

ListNode* list_nth_element(List* L, int n) 
{
    if (n < L->length) {
        ListNode* l = &L->sentinel;
        assert(l);

        for (int i = 0; i <= n; i++){
            l = l->succ;
        }
        return l;
    }else
        return list_end(L);
}

void list_concat(List* L1, List* L2)
{
    L1->sentinel.pred->succ = L2->sentinel.succ;
    L2->sentinel.succ->pred = L1->sentinel.pred;

    L2->sentinel.pred->succ = &L1->sentinel;
    L1->sentinel.pred = L2->sentinel.pred;

    L2->sentinel.succ = L2->sentinel.pred = &L2->sentinel;
}

int list_equal(List* L1, List* L2)
{
    if (L1->length == L2->length) {
        ListNode* l1 = &L1->sentinel;
        ListNode* l2 = &L2->sentinel;
        assert(l1);
        assert(l2);

        do {
            l1 = l1->succ;
            l2 = l2->succ;
            if (l1->val != l2->val)
                return 0;
        } while (l1 != &L1->sentinel);

    }
    else
        return 0;

    return 1;
}