#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

/* tengo tutti gli include, perche mi scoccia andare a riprenderli piu avanti*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>	/* gettimeofday()    struct timeval */
#include <time.h>	/* nanosleep()   struct timespec */
#include <pthread.h>

#define NUM_THREADS 10

void * func( void *);

int main() {
    pthread_t tid;
    int t, rc;
    int *p;

    for(t=0;t < NUM_THREADS;t++) {
        p = (int *) malloc( sizeof(int) );
        *p = t;
        rc = pthread_create( & tid , NULL, func, (void *) p );
    }

    return 0;
}

void * func( void *arg ) {
    int indice;
    /* uso arg come puntatore a int */
    indice = * ((int*)arg);
    /* rilascio memoria */
    free( arg ); /* non fare in caso sbagliato */
    printf(" ho ricevuto %i \n", indice );
    pthread_exit( NULL );
}
