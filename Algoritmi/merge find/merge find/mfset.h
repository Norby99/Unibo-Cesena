#ifndef MERGE_FIND_H
#define MERGE_FIND_H

typedef struct {
    int n;  /* Numero di elementi   */
    int* p; /* Array di padri       */
    int* h; /* Altezze degli alberi */
} MFSet;

/* Inizializza una struttura merge-find composta da n elementi disgiungi */
void mfset_init(MFSet* s, int n);

/* Distruggi la struttura merge-find */
void mfset_destroy(MFSet* s);

/* Fonde gli insiemi contenenti x e y; se x e y gi√  appartengono allo
   stesso insieme, non fa nulla. */
void mfset_merge(MFSet* s, int x, int y);

/* Restituisce l'id del rappresentante dell'insieme contenente x */
int mfset_find(MFSet* s, int x);

#endif