#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

#define MAXLEN 100
#define MAP_SIZE 8

char* analizedText(char[]);
int getPos(char);

char map[MAP_SIZE][4] = { "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz" };

int main( int argc, char *argv[] )
{
    //FILE *f;
    char text[4] = "ciao"; 
    char *result;

    /*if (argc != 2) {
        fprintf(stderr, "Uso: %s parola\n", argv[0]);
        return EXIT_FAILURE;
    }

    if ((f = fopen("wordlist.txt", "r")) == NULL) {
        fprintf(stderr, "Errore nell'apertura di \"wordlist.txt\"\n");
        return EXIT_FAILURE;
    }*/

    result = analizedText(text);
    printf("Il risultato e'= %s", result);


    //fclose(f);
    return EXIT_SUCCESS;
}

char* analizedText(char text[]){
    char *result;
    int temp;
    char *converter;
    result = (char*)malloc(sizeof(char)*strlen(text));
    converter = (char*)malloc(sizeof(char));
    
    for(int i=0; i < strlen(text); i++){
        temp = getPos(text[i]);
        if(temp > 1){
            itoa(temp, converter, 10);
            result[i] = converter[0];
        }else{
            printf("Il valore non e' stato trovato!\n");
            break;
        }
    }
    return result;
    
}

int getPos(char c){
    for(int i=0; i < MAP_SIZE; i++)
        for(int j=0; j < strlen(map[i]); j++)
            if(map[i][j] == c)
                return i+2;
    return -1;
}