#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"

void graph_init(Graph* g, int n, Graph_type t) {
    int i;

    assert(g != NULL);

    g->n = n;
    g->m = 0;
    g->t = t;
    g->edges = (Edge**)malloc(n * sizeof(Edge*));
    assert(g->edges != NULL);
    g->in_deg = (int*)malloc(n * sizeof(*(g->in_deg)));
    assert(g->in_deg != NULL);
    g->out_deg = (int*)malloc(n * sizeof(*(g->out_deg)));
    assert(g->out_deg != NULL);
    for (i = 0; i < n; i++) {
        g->edges[i] = NULL;
        g->in_deg[i] = g->out_deg[i] = 0;
    }
}

void graph_destroy(Graph* g) {
    int i;
    Edge* temp, *e;


    for (i = 0; i < g->n; i++) {    //scorro l'array edges

        if (g->edges[i] == NULL)
            continue;
        e = g->edges[i]; assert(e != NULL);
        while (e->next != NULL) {
            temp = e;
            e = e->next;
            free(temp);
        }
        free(e);
    }
    free(g->edges);
    free(g->in_deg);
    free(g->out_deg);
}

Graph_type graph_type(const Graph* g) {
    return g->t;
}


void graph_add_edge(Graph* g, int src, int dst, double weight) {

    Edge* e = (Edge*)malloc(sizeof(Edge)); assert(e != NULL);  //pfff

    g->in_deg[dst]++;
    g->out_deg[src]++;
    g->m++;

    e->src = src;
    e->dst = dst;
    e->weight = weight;
    if (g->edges[src] != NULL)
        e->next = g->edges[src];
    else
        e->next = NULL;

    g->edges[src] = e; assert(g->edges[src] != NULL);  //pfff

    if (!g->t) {  // se il grafo non e' orientato aggiungo il nodo anche alla destinazione
        e->src = dst;
        e->dst = src;
        if (g->edges[dst] != NULL)
            e->next = g->edges[dst];
        else
            e->next = NULL;

        g->edges[dst] = e; assert(g->edges[dst] != NULL);  //pfff
        free(e->next);
    }
    
}

int graph_n_nodes(const Graph* g) {
    assert(g != NULL);

    return g->n;
}

int graph_n_edges(const Graph* g) {
    assert(g != NULL);

    return g->m;
}

int graph_out_degree(const Graph* g, int v) {
    assert(g != NULL);
    assert(v >= 0 && v < graph_n_nodes(g));
    return g->out_deg[v];
}

int graph_in_degree(const Graph* g, int v) {
    assert(g != NULL);
    assert(v >= 0 && v < graph_n_nodes(g));
    return g->in_deg[v];
}

Edge* graph_adj(const Graph* g, int v) {
    assert(g != NULL);
    assert(v >= 0 && v < graph_n_nodes(g));

    return g->edges[v];
}

void graph_print(const Graph* g) {
    int i;

    assert(g != NULL);

    if (graph_type(g) == GRAPH_UNDIRECTED) {
        printf("UNDIRECTED\n");
    }
    else {
        printf("DIRECTED\n");
    }

    for (i = 0; i < g->n; i++) {
        const Edge* e;
        int out_deg = 0; /* ne approfittiamo per controllare la
                            correttezza dei gradi uscenti */
        printf("[%2d] -> ", i);
        for (e = graph_adj(g, i); e != NULL; e = e->next) {
            printf("(%d, %d, %f) -> ", e->src, e->dst, e->weight);
            out_deg++;
        }
        assert(out_deg == graph_out_degree(g, i));
        printf("NULL\n");
    }
}

int graph_read_from_file(FILE* f, Graph* g) {
    int n, m, t;
    int src, dst;
    int i; /* numero archi letti dal file */
    double weight;

    assert(g != NULL);
    assert(f != NULL);

    if (3 != fscanf(f, "%d %d %d", &n, &m, &t)) {
        fprintf(stderr, "ERRORE durante la lettura dell'intestazione del grafo\n");
        return -1;
    };
    assert(n > 0);
    assert(m >= 0);
    assert((t == GRAPH_UNDIRECTED) || (t == GRAPH_DIRECTED));

    graph_init(g, n, t);
    /* Ciclo di lettura degli archi. Per rendere il programma pi√π
       robusto, meglio non fidarsi del valore `m` nell'intestazione
       dell'input. Leggiamo informazioni sugli archi fino a quando ne
       troviamo, e poi controlliamo che il numero di archi letti (i)
       sia uguale a quello dichiarato (m) */
    i = 0;
    while (3 == fscanf(f, "%d %d %lf", &src, &dst, &weight)) {
        graph_add_edge(g, src, dst, weight);
        i++;
    }
    if (i != m) {
        fprintf(stderr, "WARNING: ho letto %d archi, ma l'intestazione ne dichiara %d\n", i, m);
    }
    return 0;
}

void graph_write_to_file(FILE* f, const Graph* g) {
    int v;
    int n, m, t;

    assert(g != NULL);
    assert(f != NULL);

    n = graph_n_nodes(g);
    m = graph_n_edges(g);
    t = graph_type(g);

    fprintf(f, "%d %d %d\n", n, m, t);
    for (v = 0; v < n; v++) {
        const Edge* e;
        for (e = graph_adj(g, v); e != NULL; e = e->next) {
            assert(e->src == v);
            /* Se il grafo √® non orientato, dobbiamo ricordarci che
               gli archi compaiono due volte nelle liste di
               adiacenza. Nel file pero' dobbiamo riportare ogni arco
               una sola volta, dato che sar√  la procedura di lettura a
               creare le liste di adiacenza in modo corretto. Quindi,
               ogni coppia di archi (u,v), (v,u) deve comparire una
               sola volta nel file. Per comodit√ , salviamo nel file la
               versione di ciascun arco in cui il nodo sorgente √®
               minore del nodo destinazione. */
            if ((graph_type(g) == GRAPH_DIRECTED) || (e->src < e->dst)) {
                fprintf(f, "%d %d %f\n", e->src, e->dst, e->weight);
            }
        }
    }
}