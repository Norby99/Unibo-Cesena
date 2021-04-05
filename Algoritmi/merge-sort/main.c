/****************************************************************************
 *
 * merge-sort.c -- Implementazione algoritmo Merge Sort.
 *
 * Copyright (C) 2021 Nicolas Farabegoli, Moreno Marzolla
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ****************************************************************************/

 /***
 % LabASD - Merge sort
 % Nicolas Farabegoli <nicolas.farabegoli2@unibo.it>
   Moreno Marzolla <moreno.marzolla@unibo.it>
 % Ultimo aggiornamento: 2021-03-18

 Implementare l'algoritmo _Merge sort_ in modo tale che i test forniti
 passino correttamente. Merge sort è stato ideato da John Von Neumann
 nel 1945.

 ![Figura 1: John von Neumann By LANL -
 <http://www.lanl.gov/history/atomicbomb/images/NeumannL.GIF> (archive
 copy), Attribution,
 >https://commons.wikimedia.org/w/index.php?curid=3429594>](von-neumann.jpg)

 L'algoritmo Merge Sort basa il suo funzionamento sul paradigma _divide
 et impera_. L'array da ordinare viene suddiviso in due metà che
 vengono ricorsivamente ordinate. Le due metà ordinate vengono poi
 ricomposte mediante una procedura `merge()`. Si faccia riferimento
 alla Figura 1 per un esempio.

 ![Figura 1: Merge Sort](merge-sort.png)

 Nell'algoritmo Merge Sort la fase _divide_ è semplice, mentre la fase
 _impera_ (fusione di due array ordinati) è più laboriosa da
 realizzare.

 Detto $T(n)$ il tempo necessario ad ordinare un array di lungehzza $n$
 usando Merge Sort, possiamo definire $T(n)$ in modo ricorsivo come
 segue:

 $$
 T(n) = \begin{cases}
 c_1 & \mbox{se } n \leq 1 \\
 2T(n/2) + c_2 n & \mbox{altrimenti}
 \end{cases}
 $$

 dove $c_1$ e $c_2$ sono opportune costanti. La componente $c_2 n$
 indica il tempo necessario alla fase di fusione (merge), che è appunto
 proporzionale alla somma delle dimensioni delle metà da
 fondere. Applicando il Master Theorem si ha $T(n) = \Theta(n \log n)$.

 In questo programma viene proposta la versione ricorsiva; chi lo
 desidera può realizzare la versione iterativa di MergeSort.

 ## Suggerimenti

 La funzione `merge()` richiede un buffer di memoria temporaneo per
 effettuare la fusione (_merge_) dei sottovettori ordinati. È possibile
 allocare e deallocare buffer temporanei all'interno della funzione, ma
 questo sarebbe molto inefficiente.

 Nel codice fornito, la funzione `merge_sort()` alloca un buffer
 temporaneo di lunghezza $n$ e passa tale buffer alle funzioni di
 supporto `merge_sort_rec()` e `merge()`.  Queste ultime possono usare
 liberamente tale buffer, quando necessario, eliminando l'overhead che
 sarebbe causato dalle ripetute chiamate a `malloc()` e `free()`.

 ## Per approfondire

 Questo programma misura in modo grossolano il tempo necessario a ordinare
 l'array. Considerando array di dimensioni sufficientemente grandi (es.,
 100000 elementi o più), come varia il tempo di esecuzione nei seguenti casi?

 - array di input già ordinato;

 - array di input "quasi" ordinato (esempio, array ottenuto da un array
   ordinato in cui si scambiano tra loro due elementi in posizioni
   casuali $i$, $j$);

 - array di input ordinato in senso decrescente;

 - array di input casuale;

 - array di input composto da tutti valori uguali

 Per eseguire le prove di cui sopra occorre modificare opportunamente
 la funzione `main()` per generare degli array di input opportuni. Nel
 caso possano risultare utili, vengono fornite due funzioni `randab()`
 e `random_shuffle()`, la cui specifica è indicata nei commenti al
 codice.

 ## File

 - [merge-sort.c](merge-sort.c)

 ***/

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
