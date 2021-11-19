from math import *

def swap(a, b, l):
    l[a], l[b] = l[b], l[a]

def generate(n, A):
    c = []
    for i in range(n):
        c.append(0)

    print(A)
    i = 1
    while i < n:
        if c[i] < i:
            if i%2 == 0:
                swap(0, i, A)
            else:
                swap(c[i], i, A)
            print(A)
            c[i] += 1
            i = 1
        else:
            c[i] = 0
            i += 1



"""
*Calcolo la grandezza "fact" della lista e la salvo nella variabile esterna
Inserisco i primi numeri nella seconda riga
?Dichiaro riga e i (posso usare un registro solo es AL, AH)
*Ciclo i fino ad arrivare ad "n", che e' una variabile esterna
Se A[i] e' minore di i
    Se i e' pari
        cambio A[ riga*n ] con A[ i + riga*n ]
    altrimenti
        cambio A[ A[i] + riga*n ] con A[ i + riga*n ]
    alla fine
        incremento riga
        incremento A[i]
        incremento i

Altrimenti
    imposto A[i] = 0
    incremento i

Se riga < fact:     # copio la riga precedente in quella successiva
    k = 0
    A[ riga*n + k ] = A[(riga-1)*n + k]
    Se k < n, salta alla riga 48

Infine impostare la prima riga

!Si potrebbe considerare di tenere riga*n come un'unica variabile, poi al massimo sotrarre n
"""
def prova(n, A):
    fact = factorial(n)
    i = n
    while i < fact*n:   # questo ciclo sara' inutile nell'assembly
        A.append(0)
        i += 1

    k = 0
    for j in A[0 : n]:
        A[n+k] = j
        k +=1

    for j in range(n):
        A[j] = 0

    riga = 1
    i = 1
    while i < n:
        
        if A[i] < i:
            if i%2 == 0:
                swap((riga*n), i+(riga*n), A)
            else:
                swap(A[i]+(riga*n), i+(riga*n), A)
            riga += 1
            A[i] += 1
            i = 1
        else:
            A[i] = 0
            i += 1

        if riga < fact:
            k = 0
            for j in A[(riga-1)*n : n+(riga-1)*(n)]:
                A[riga*n+k] = j
                k += 1

            

    for i in range(n):
        A[i] = i+1

    for i in range(0, len(A), n):
        print(A[i:i+n])


def heap(k, A):
    if k == 1:
        print(A)
    else:
        heap(k-1, A)
        for i in range(k-1):
            if k%2 == 0:
                swap(i, k-1, A)
            else:
                swap(0, k-1, A)
            heap(k-1, A)


A = list(range(1, 4))
prova(len(A), A)
