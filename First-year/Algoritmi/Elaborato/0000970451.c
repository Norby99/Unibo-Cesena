/*  
    nome: Gabos Norbert
    matricola: 0000970451
    gruppo: A
    mail: tiberiunorbert.gabos@studio.unibo.it

Modalita' di funzionamento:
    Il programma prende come input un file contenente le scatole, con cui popola un'array.
    Una volta ottenuta l'array, la ordino in base ad un lato (in questo caso il lato x) in ordine decrescente (per l'ordinamento ho preso spunto dall'algoritmo di quick sort visto a lezione).
    Ottenuta l'array ordinata, si parte dal fondo e per ogni scatola, scorro l'array verso destra e tra quelle cerco le scatole che possono stare dentro ad essa e scelgo quella con piu' scatole all'interno.
    Per velocizzare la ricerca ho utilizzato l'attributo BoxCounter. Avrei potuto utilizzare anche il puntatore newBox, ma avrebbe rallentato il programma, in quanto per ogni scatola, avrebbe dovuto trovare il numero di scatole max che poteva contenere.
    Infine ritorno la scatola con maggior scatole all'interno e la stampo.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Box{
    int id;     // variabile che contiene il numero (o nome) della scatola, in quanto l'array verra successivamente riordinata, perdendo l'ordine originale di esse
    float x, y, z;  // queste variabili servono per memorizzare le dimensioni delle scatole
    struct Box *newBox;    // questo tiene traccia della scatola piu con piu scatole dentro, in cui puo' stare
    int BoxCounter; // variabile che memorizza quante scatole al massimo possono stare dentro in questa scatola. NB. questa variabile non e' necessare, in quanto questo numero potrebbe essere ricavato mediante la variabile "newBox", ma e' stata inserita per ottimizzare la ricerca.
}Box;

typedef struct{
    Box *array;
    int len;
}ArrayBox;

void readFromFile(FILE*, ArrayBox*);    // Funzione che popola l'array di scatole

void orderArrayDec(ArrayBox*);  // Funzioni che implementano il reverse Quicksort
void quicksort_rev(ArrayBox*, int, int);
int partition(ArrayBox*, int, int);
void swap(ArrayBox*, int, int);

bool canContainIt(Box, Box);    // Funzione che restituisce true solo se la prima scatola puo' contenere la seconda
Box* fitTheBoxes(ArrayBox*);    // Funzione che cerrca di inserire le scatole una dentro l'altra e infine restituisco la scatola con piu scatole all'interno
void findBestBox(ArrayBox*, int);   // Funzione che data una scatola, trova la scatola che ha piu satole dentro e che puo contenere

void printBestBoxPath(Box*); // Stampa il risultato finale

int main( int argc, char *argv[] ){

    ArrayBox Boxes;
    FILE *filein = stdin;

    if (argc > 1) {
        filein = fopen(argv[1], "r");
        if (filein == NULL) {
            fprintf(stderr, "Can not open %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    if (1 != fscanf(filein, "%d", &Boxes.len)) {
        fprintf(stderr, "Missing size\n");
        return EXIT_FAILURE;
    }

    readFromFile(filein, &Boxes);
    orderArrayDec(&Boxes);

    printBestBoxPath(fitTheBoxes(&Boxes));
    return 0;
}

/*
Questa funzione scorre le scatole dalla piu "piccola" alla piu "grande".
Visto che le scatole non possono essere ruotate, esse possono essere ordinate per qualsiasi critero (x, y, z o volume)
*/
Box* fitTheBoxes(ArrayBox *B){
    int maxVal = B->array[0].BoxCounter, bestBox = 0;

    for (int i = B->len-1; i >= 0; i--){    // Scorre le scatole dalla piu "piccola" alla piu "grande"
        findBestBox(B, i);
        if(maxVal <= B->array[i].BoxCounter){
            maxVal = B->array[i].BoxCounter;
            bestBox = i;
        }
    }
    return &B->array[bestBox];
}

void findBestBox(ArrayBox *B, int n){
    int bestBox = n, bestNum = B->array[n].BoxCounter;  // Si tiene traccia della scatola migliore

    if(n == B->len-1)
        B->array[n].BoxCounter = 0;
    else{
        for (int i = n+1; i < B->len; i++){ // scorre le scatole dalla selezioanta fino alla piu piccola
            if (B->array[i].BoxCounter >= bestNum && canContainIt(B->array[n], B->array[i])){   // trovata una che la soddisfa controlla se e' migliore dell'ultima selezionata
                bestNum = B->array[i].BoxCounter;
                bestBox = i;
            }
        }
        if (bestBox == n)   // infine controlla che abbia trovato una scatola e se si, la inserisce e tiene traccia di quante scatole ci stanno al massimo all'interno di essa
            B->array[n].BoxCounter = 0;
        else{
            B->array[n].BoxCounter = 1 + bestNum;
            B->array[n].newBox = &B->array[bestBox];
        }
    }
}

bool canContainIt(Box B1, Box B2){
    return (B1.x > B2.x && B1.y > B2.y && B1.z > B2.z);
}

void orderArrayDec(ArrayBox *B){
    quicksort_rev(B, 0, B->len-1);
}

void quicksort_rev(ArrayBox *B, int start, int end){
    if (start < end) {
        const int split = partition(B, start, end);
        quicksort_rev(B, start, split - 1);
        quicksort_rev(B, split + 1, end);
    }
}

int partition(ArrayBox *B, int start, int end){
    const int pivot = B->array[end].x;
    int i = (start - 1), j;

    for (j = start; j < end; j++) {
        if (B->array[j].x > pivot) {
            i++;
            swap(B, i, j);
        }
    }
    swap(B, i+1, end);
    return i + 1;
}

void swap(ArrayBox *B, int i, int j){
    Box tmp = B->array[i];
    B->array[i] = B->array[j];
    B->array[j] = tmp;
}

void readFromFile(FILE *filein, ArrayBox *Boxes){
    int i = 0;

    Boxes->array = (Box*)malloc(sizeof(Box) * Boxes->len);
    while (3 == fscanf(filein, " %f %f %f", &Boxes->array[i].x, &Boxes->array[i].y, &Boxes->array[i].z)) {
        Boxes->array[i].id = i;
        Boxes->array[i].newBox = NULL;
        Boxes->array[i].BoxCounter = 0;
        i++;
    }
}

void printBestBoxPath(Box *B){
    printf("%d scatole\n", B->BoxCounter+1);
    while (B != NULL){
        printf("scatola %4d: %7.2f %7.2f %7.2f\n", B->id, B->x, B->y, B->z);
        B = B->newBox;
    }
}