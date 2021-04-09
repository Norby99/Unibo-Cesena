#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

 /* Nota: il main assume che BSTKey sia il tipo "int" */
int main(int argc, char* argv[])
{
    char op;
    BSTKey k;
    BST T;
    BSTNode* n;
    FILE* filein = stdin;

    if (argc > 1) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    bst_init(&T);

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case '+': /* insert */
            fscanf(filein, "%d", &k);
            printf("INSERT %d\n", k);
            bst_insert(&T, k);
            break;
        case '-': /* delete */
            fscanf(filein, "%d", &k);
            printf("DELETE %d\n", k);
            n = bst_search(&T, k);
            if (n) {
                bst_delete(&T, n);
            }
            else {
                printf("NOT FOUND\n");
            }
            break;
        case '?': /* search */
            fscanf(filein, "%d", &k);
            printf("SEARCH %d = ", k);
            n = bst_search(&T, k);
            if (n) {
                printf("FOUND\n");
            }
            else {
                printf("NOT FOUND\n");
            }
            break;
        case 's': /* size */
            printf("SIZE = %d\n", bst_size(&T));
            break;
        case 'h': /* height */
            printf("HEIGHT = %d\n", bst_height(&T));
            break;
        case 'p': /* print */
            bst_print(&T);
            break;
        default:
            printf("Unknown command %c\n", op);
            exit(EXIT_FAILURE);
        }
    }

    bst_destroy(&T);

    return EXIT_SUCCESS;
}