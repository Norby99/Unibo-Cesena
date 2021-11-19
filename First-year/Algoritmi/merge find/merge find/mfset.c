#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "mfset.h"

void mfset_init(MFSet* s, int n)
{
    int i;

    assert(s != NULL);

    s->n = n;
    s->p = (int*)malloc(n * sizeof(*(s->p)));
    printf("%d ", n * sizeof(*(s->p)));
    assert(s->p != NULL);
    s->h = NULL; /* Per iniziare suggerisco di ignorare questo array */
    for (i = 0; i < n; i++) {
        s->p[i] = -1;
    }
}

void mfset_destroy(MFSet* s)
{
    assert(s != NULL);

    s->n = 0;
    free(s->p);
    s->p = NULL; /* Non necessario */
}

void mfset_merge(MFSet* s, int x, int y)
{
    int rapX, rapY;
    rapX = mfset_find(s, x);    // serve se un gionro mi va di fare la merge che tiene conto delle altezze
    rapY = mfset_find(s, y);

    s->p[rapY] = x;
}

int mfset_find(MFSet* s, int x)
{
    int i;
    do{
        i = x;
        x = s->p[x];

    } while (x != -1);


    return i;
}