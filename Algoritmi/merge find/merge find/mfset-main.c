#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include "mfset.h"

int main(int argc, char* argv[])
{
    char op;
    int x, y, n;
    MFSet s;
    FILE* filein = stdin;
    int i;
    argv[1] = "mfset.in";

    filein = fopen(argv[1], "r");
    if (filein == NULL) {
        fprintf(stderr, "Can not open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (1 != fscanf(filein, "%d", &n)) {
        fprintf(stderr, "Missing size\n");
        return EXIT_FAILURE;
    }
    printf("CREATE %d\n", n);
    mfset_init(&s, n);

    while (1 == fscanf(filein, " %c", &op)) {
        switch (op) {
        case 'm': /* merge */
            fscanf(filein, "%d %d", &x, &y);
            printf("UNION %d %d\n", x, y);
            mfset_merge(&s, x, y);
            break;
        case 'q': /* query */
            fscanf(filein, "%d %d", &x, &y);
            printf("QUERY %d %d = %d\n", x, y,
                mfset_find(&s, x) == mfset_find(&s, y));
            break;
        default:
            printf("Unknown command %c\n", op);
        }
    }

    for (i = 0; i < s.n; i++) {
        printf("%d ", s.p[i]);  // lo ho fatto per i test
    }

    mfset_destroy(&s);

    return EXIT_SUCCESS;
}