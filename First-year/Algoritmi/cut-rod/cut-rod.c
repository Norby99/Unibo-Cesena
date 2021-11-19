#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 1000
#define Max(a,b) (a>b?a:b)

double p[N];    /* p[i] e' il prezzo di vendita di un segmento lungo
                   i, i=1..n; p[0] non viene usato */
int n;          /* lunghezza dell'asta da tagliare */
double r[N];    /* r[j] e' il massimo guadagno che e' possibile ottenere
                   tagliando un'asta lunga j, j=0..n */
int s[N];       /* s[j] e' la lunghezza dell'ultimo taglio della
                   soluzione ottima del problema P(j) */

/* Legge l'input dal file `filein` */
void init( FILE *filein )
{
    double val;
    p[0] = 0.0; /* p[0] non viene usato */
    n = 0;
    while (1 == fscanf(filein, "%lf", &val)) {
        n++;
        assert(n < N);
        p[n] = val;
    }
}

/* Determina la soluzione del problema "cut rod". Restituisce il
   guadagno massimo */
double solve(){
    double q;
    int i,j;

    r[0] = 0;
    for (j = 0; j <= n; j++){
        q = 0;
        for (i = 0; i <= j; i++){
            q = Max(q, p[i]+r[j-i]);
        }
        s[j] = i;
        r[j] = q;
    }
}

/* Stampa la soluzione del problema. */
void print( void ){

    printf(": %d\n", s[n]);
    printf("Guadagno complessivo: %f", r[n]);

}


int main( int argc, char *argv[] )
{
    FILE *filein = stdin;
    char fileName[] = "./cut-rod1.in.txt";

    filein = fopen(fileName, "r");
    if (filein == NULL) {
        fprintf(stderr, "Can not open %s\n", fileName);
        return EXIT_FAILURE;
    }

    init( filein );

    solve();
    print();
    return EXIT_SUCCESS;
}