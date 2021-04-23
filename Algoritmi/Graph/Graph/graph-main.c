#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


int main(int argc, char* argv[]) {
    Graph G;
    FILE* filein = stdin;
    argv[1] = "graph10.in.txt";

    if (1) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    graph_read_from_file(filein, &G);
    graph_print(&G);
    graph_destroy(&G);
    return EXIT_SUCCESS;
}