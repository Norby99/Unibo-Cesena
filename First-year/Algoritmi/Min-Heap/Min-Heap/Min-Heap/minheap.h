/****************************************************************************
 *
 * minheap.h -- Interfaccia Min-Heap binario
 *
 * Copyright (C) 2021 Moreno Marzolla
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

#ifndef MINHEAP_H
#define MINHEAP_H

typedef struct {
    int key;
    double prio;
} HeapElem;

typedef struct {
    HeapElem* heap;
    int* pos;
    int n; /* quante coppie (chiave, prio) sono effettivamente presenti nello heap */
    int size; /* massimo numero di coppie (chiave, prio) che possono essere contenuti nello heap */
} MinHeap;

/* Inizializza uno heap vuoto in grado di contenere al piu' size
   coppie (chiave, priorit�); le chiavi possono essere esclusivamente
   gli interi 0 .. size - 1; ogni chiave pu� essere presente nello
   heap al pi� una volta. Inizialmente lo heap � vuoto. */
void minheap_init(MinHeap* h, int size);

/* Svuota lo heap, eliminando tutti i valori in esso presenti. */
void minheap_clear(MinHeap* h);

/* Dealloca la memoria occupata dallo heap h. Dopo aver invocato
   questa funzione, lo heap deve essere reinizializzato con
   minheap_init() nel caso lo si voglia riutilizzare */
void minheap_destroy(MinHeap* h);

/* Restituisce 1 se e solo se lo heap e' vuoto */
int minheap_is_empty(const MinHeap* h);

/* Restituisce 1 se e solo se lo heap e' pieno */
int minheap_is_full(const MinHeap* h);

/* Ritorna il numero di coppie effettivamente presenti nello heap */
int minheap_get_n(const MinHeap* h);

/* Restituisce la chiave associata alla minima priorit�; non modifica
   lo heap */
int minheap_min(const MinHeap* h);

/* Inserisce una nuova chiave key con priorita' prio nello heap; key
   non deve essere gia' presente nello heap. */
void minheap_insert(MinHeap* h, int key, double prio);

/* Rimuove dallo heap la coppia (chiave, prio) con priorit� minima, e
   restituisce la chiave di tale coppia. */
int minheap_delete_min(MinHeap* h);

/* Modifica la priorita' associata alla chiave key, che deve essere
   gi� presente nello heap. */
void minheap_change_prio(MinHeap* h, int key, double new_prio);

/* Stampa il contenuto dello heap */
void minheap_print(const MinHeap* h);

#endif
