#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

 /* Fonde le due porzioni ordinate array[p..q] e array[q+1..r]. Usa
    buffer[] come array temporaneo. */

void merge(int *array, int p, int q, int r, int *buffer){
    int i = p, j = q + 1, k = 0;

    while (i <= q && j <= r) {
        if (array[i] <= array[j]) {
            buffer[k] = array[i];
            i = i + 1;
        }
        else {
            buffer[k] = array[j];
            j = j + 1;
        }
        k = k + 1;
    }

    while (i <= q)
        buffer[k++] = array[i++];

    while(j <= r)
        buffer[k++] = array[j++];

    for (k = p; k <= r; k++)
        array[k] = buffer[k - p];

}

/* Ordina la porzione di array compresa tra gli indici p e r (inclusi) */
void merge_sort_rec(int* v, int p, int r, int* buffer)
{
    int q;
    if (p < r)
    {
        q = (p + r) / 2;
        merge_sort_rec(v, p, q, buffer);
        merge_sort_rec(v, q + 1, r, buffer);
        merge(v, p, q, r, buffer);
    }
}

void merge_sort(int* v, int n)
{
    int* buffer = (int*)malloc(n * sizeof(*buffer));
    assert(buffer); /* evita un warning con VS */
    merge_sort_rec(v, 0, n - 1, buffer);
    free(buffer);
}

void print_array(const int* v, int n)
{
    int i;

    printf("[ ");
    for (i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("]");
}

/* Restituisce un valore casuale compreso tra a e b (estremi inclusi) */
int randab(int a, int b)
{
    return a + rand() % (b - a + 1);
}

/* Permuta il contenuto dell'array v in modo casuale. Questa operazione
   viene eseguita in modo iterativo come segue:

   - al primo passo si scambia v[0] con uno elemento casuale scelto
     in v[1..n-1];

   - al secondo passo si scambia v[1] con un elemento casuale scelto
     in v[2..n-1];

   ...

   - all'i-esimo passo si scambia v[i] con un elemento casuale scelto
     in v[i+1..n-1]

   È possibile dimostrare che questo procedimento, applicato fino alla fine,
   produce un "rimescolamento casuale" (random shuffle) di v[].
*/
void random_shuffle(int* v, int n)
{
    int i;

    for (i = 0; i < n - 1; i++) {
        const int j = randab(i, n - 1);
        const int tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
    }
}

/* Restituisce -1 se *p1 (interpretato come intero) è minore di *p2
   (sempre interpretato come inter), 0 se sono uguali, 1 se il primo è
   maggiore del secondo. */
int compare(const void* p1, const void* p2)
{
    int v1 = *(int*)p1;
    int v2 = *(int*)p2;
    return (v1 - v2);
}

/* Confronta il contenuto di due array v1 e v2 di lunghezza n;
   restituisce l'indice del primo elemento il cui valore differisce
   nei due array, oppure -1 se gli array hanno lo stesso contenuto */
int compare_vec(const int* v1, const int* v2, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        if (v1[i] != v2[i])
            return i;
    }
    return -1;
}

/* Ordina l'array v[] di lunghezza n. Confrontiamo il risultato
   dell'ordinamento con quello prodotto dalla funzione qsort() della
   libreria standard C. Restituisce 1 se il test ha successo, 0
   altrimenti. */
int test(int* v, int n)
{
    int result;
    int* tmp = (int*)malloc(n * sizeof(*tmp));
    clock_t tstart, elapsed;
    int diff;

    assert(tmp); /* evita un warning con VS */
    memcpy(tmp, v, n * sizeof(*v));
    qsort(tmp, n, sizeof(*tmp), compare);
    tstart = clock();
    merge_sort(v, n);
    elapsed = clock() - tstart;
    diff = compare_vec(v, tmp, n);
    if (diff < 0) {
        printf("Test OK (%f seconds)\n", elapsed / (double)CLOCKS_PER_SEC);
        result = 1;
    }
    else {
        printf("Test FALLITO: v[%d]=%d, atteso=%d\n", diff, v[diff], tmp[diff]);
        result = 0;
    }
    free(tmp);
    return result;
}

#define ARRAY_LEN(v) (sizeof(v)/sizeof(v[0]))

int main(void)
{
    int v1[] = {0, 8, 1, 7, 2, 6, 3, 5, 4};
    int v2[] = {0, 1, 0, 6, 10, 10, 0, 0, 1, 2, 5, 10, 9, 6, 2, 3, 3, 1, 7};
    int v3[] = {-1, -3, -2};
    int v4[] = {2, 2, 2};

    test(v1, ARRAY_LEN(v1));
    test(v2, ARRAY_LEN(v2));
    test(v3, ARRAY_LEN(v3));
    test(v4, ARRAY_LEN(v4));

    return EXIT_SUCCESS;
}
