#include <stdio.h>
#include <stdlib.h>
#include "minheap.h"

int main(int argc, char* argv[])
{
    char op;
    int n, val;
    double prio;
    MinHeap h;
    FILE* filein = stdin;

    if (argc > 1) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case 'n': /* new heap */
            fscanf(filein, "%d", &n);
            minheap_init(&h, n);
            break;
        case 'i': /* insert */
            fscanf(filein, "%d %lf", &val, &prio);
            printf("INSERT %d %f\n", val, prio);
            minheap_insert(&h, val, prio);
            break;
        case 'd': /* delete min */
            printf("DELETE_MIN\n");
            minheap_delete_min(&h);
            break;
        case 'm': /* get min */
            val = minheap_min(&h);
            printf("MIN = %d\n", val);
            break;
        case 'c': /* change prio */
            fscanf(filein, "%d %lf", &val, &prio);
            printf("CHANGE_PRIO %d %f\n", val, prio);
            minheap_change_prio(&h, val, prio);
            break;
        case 's': /* get n of elements */
            printf("N = %d\n", minheap_get_n(&h));
            break;
        case 'p': /* print */
            minheap_print(&h);
            break;
        default:
            printf("Unknown command %c\n", op);
        }
    }

    minheap_destroy(&h);

    return EXIT_SUCCESS;
}