#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "minheap.h"

void minheap_print(const MinHeap* h)
{
    int i = 0, j, width = 1;

    assert(h);

    printf("\nn=%d size=%d\n", h->n, h->size);
    while (i < h->n) {
        for (j = 0; j < width && i < h->n; j++) {
            printf("h[%d]=(%d %f) ", i, h->heap[i].key, h->heap[i].prio);
            i++;
        }
        printf("\n");
        width *= 2;
    }
}


/* Costruisce un min-heap vuoto che puo' contenere al massimo
   size elementi */
void minheap_init(MinHeap* h, int size)
{

    assert(h);

    h->n = 0;
    h->size = size;
    h->heap = (HeapElem*)malloc(size * sizeof(HeapElem));
}

void minheap_destroy(MinHeap* h)
{
    assert(h);

    h->n = h->size = 0;
    free(h->heap);
}


/* Restituisce 1 se lo heap e' vuoto */
int minheap_is_empty(const MinHeap* h)
{
    assert(h);

    return (h->n == 0);
}

/* Restituisce 1 se lo heap e' pieno, cioe' e' stata esaurita la
   capienza a disposizione */
int minheap_is_full(const MinHeap* h)
{
    assert(h);

    return (h->n == h->size);
}

/* Restituisce il numero di elementi presenti nello heap */
int minheap_get_n(const MinHeap* h)
{
    assert(h);

    return h->n;
}

/* Restituisce la chiave associata all'elemento con priorit√  minima */
int minheap_min(const MinHeap* h)
{
    assert(!minheap_is_empty(h));

    return h->heap[0].key;
}

/* Inserisce una nuova coppia (key, prio) nello heap. */
void minheap_insert(MinHeap* h, int key, double prio)
{
    /* [TODO] */
}

/* Rimuove la chiave associata alla priorita' minima */
int minheap_delete_min(MinHeap* h)
{
    /* [TODO] */
    return -1;
}

/* Modifica la priorita' associata alla chiave key. La nuova priorit√
   pu√≤ essere maggiore, minore o uguale alla precedente. */
void minheap_change_prio(MinHeap* h, int key, double newprio)
{
    /* [TODO] */
}