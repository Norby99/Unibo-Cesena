#ifndef GRAPH_H
#define GRAPH_H

 /* struttura arco */
typedef struct Edge {
    int src;            /* nodo sorgente        */
    int dst;            /* nodo destinazione    */
    double weight;      /* peso dell'arco       */
    struct Edge* next;
} Edge;

typedef enum { GRAPH_UNDIRECTED, GRAPH_DIRECTED } Graph_type;

/* struttura grafo */
typedef struct {
    int n;              /* numero di nodi               */
    int m;              /* numero di archi              */
    Graph_type t;       /* tipo di grafo (orientato/non orientato) */
    Edge** edges;       /* array di liste di adiacenza  */
    int* in_deg;        /* grado entrante dei nodi      */
    int* out_deg;       /* grado uscente dei nodi       */
} Graph;

/* Inizializza un grafo con n nodi; il tipo di grafo (orientato oppure
   non orientato) √® specificato dal parametro t. */
void graph_init(Graph* g, int n, Graph_type t);

/* Libera tutta la memoria occupata dal grafo e dalle liste di
   adiacenza */
void graph_destroy(Graph* g);

/* Restituisce il tipo di grafo */
Graph_type graph_type(const Graph* g);

/* Aggiunge un nuovo arco (src, dst) con peso "weight". Si puo'
   assumere che l'arco non esista gi√  (si pu√≤ omettere il controllo,
   anche se nella soluzione fornita viene fatto). Nel caso di grafo
   non orientato, occorre aggiungere nelle liste di adiacenza sia
   (src, dst) che (dst, src).  */
void graph_add_edge(Graph* g, int src, int dst, double weight);

/* Restituisce un puntatore al primo nodo della lista di adiacenza
   associata al nodo v (NULL se la lista √® vuota) */
Edge* graph_adj(const Graph* g, int v);

/* Numero di nodi del grafo */
int graph_n_nodes(const Graph* g);

/* Numero di archi del grafo */
int graph_n_edges(const Graph* g);

/* Restituisce il grado uscente del nodo v. */
int graph_out_degree(const Graph* g, int v);

/* Restituisce il grado entrante nel nodo v. */
int graph_in_degree(const Graph* g, int v);

/* Stampa il grafo */
void graph_print(const Graph* g);

/* Legge il grafo dal file f. Restituisce 0 se la lettura ha avuto
   successo. Il grafo g *non deve* essere gi√  stato inizializzato con
   graph_init() */
int graph_read_from_file(FILE* f, Graph* g);

/* Salva il grafo sul file f, con lo stesso formato usato dalla
   funzione graph_read_from_file() */
void graph_write_to_file(FILE* f, const Graph* g);

#endif