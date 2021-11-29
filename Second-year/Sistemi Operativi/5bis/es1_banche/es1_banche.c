#ifndef _THREAD_SAFE
	#define _THREAD_SAFE
#endif
#ifndef _POSIX_C_SOURCE
	#define _POSIX_C_SOURCE 200112L
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/time.h>	/* gettimeofday()    struct timeval */
#include <time.h>	/* nanosleep()   struct timespec */
#include <pthread.h>
#define NUMTHRDS 10

#include "printerror.h"
#include "DBGpthread.h"

#define NBANCHE 3
#define PTDEPOSITO 5
#define PTPRELIEVO 4

typedef struct banca {
    float T;
    int N;
}banca;

typedef struct{
    banca *array;
    int len;
}ArrayBanche;

pthread_mutex_t  mutex;
ArrayBanche Banks;

void *Depositi(void *arg) {
	char Clabel[128];
	intptr_t indicebanca;

	indicebanca=(intptr_t)arg;
	sprintf(Clabel,"D%" PRIiPTR "" , indicebanca);

	while(1) 
	{
		struct timespec ts={0, 200000000L }; /* 2/10 sec */

		sleep(1);
		DBGpthread_mutex_lock(&mutex,Clabel); 
		Banks.array[indicebanca].T += 10.0;
		Banks.array[indicebanca].N ++;
		printf("Deposito: N %d  T %f \n", Banks.array[indicebanca].N, Banks.array[indicebanca].T);
		fflush(stdout);
		nanosleep(&ts,NULL);

		DBGpthread_mutex_unlock(&mutex,Clabel); 
	}
	pthread_exit(NULL); 
}

void *Prelievi(void *arg) {
	char Clabel[128];
	intptr_t indicebanca;

	indicebanca=(intptr_t)arg;
	sprintf(Clabel,"D%" PRIiPTR "" , indicebanca);

	while(1) 
	{
		struct timespec ts={0, 100000000L }; /* 2/10 sec */

		sleep(1);
		DBGpthread_mutex_lock(&mutex,Clabel); 
		Banks.array[indicebanca].T -= 90.0;
		Banks.array[indicebanca].N ++;
		printf("Deposito: N %d  T %f \n", Banks.array[indicebanca].N, Banks.array[indicebanca].T);
		fflush(stdout);
		nanosleep(&ts,NULL);

		DBGpthread_mutex_unlock(&mutex,Clabel); 
	}
	pthread_exit(NULL); 
}

void *BancadItalia(void *arg) {
    char Clabel[128];

	sprintf(Clabel,"BancadItalia" );

	while(1) 
	{
		int i;  int Num=0; double sum=0.0;

		DBGpthread_mutex_lock(&mutex,Clabel); 
		for( i=0; i<NBANCHE; i++ ) {
			Num += Banks.array[i].N;
			sum += Banks.array[i].T;
		}
		DBGpthread_mutex_unlock(&mutex,Clabel); 
		printf("Report BancaDItalia: Num %d  sum %f \n", Num, sum);
		fflush(stdout);

		sleep(30);
	}
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
	pthread_t th; 
	int rc;
	intptr_t i,k;

    Banks.array = (banca*)malloc(sizeof(banca) * NBANCHE);
    Banks.len = NBANCHE;

    for (i = 0; i < NBANCHE; i++){
        Banks.array[i].T = 0.0;
        Banks.array[i].N = 0;
    }

	rc = pthread_mutex_init (&mutex, NULL);
	if( rc != 0 ) PrintERROR_andExit(errno,"pthread_mutex_init failed"); /* no EINTR */
	for(i=0;i < NBANCHE;i++) 	{
				/* lancio  thread Depositi */
		for(k=0;k<PTDEPOSITO;k++) {
			rc=pthread_create( &th, NULL,Depositi,(void*)i); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
		/* lancio  thread Prelievi */
		for(k=0;k<PTPRELIEVO;k++) {
			rc=pthread_create( &th, NULL,Prelievi,(void*)i); 
			if(rc) PrintERROR_andExit(rc,"pthread_create failed");
		}
	}

	/* lancio thread BancadItalia */
	rc = pthread_create( &th, NULL, BancadItalia, (void*)k); 
	if(rc) PrintERROR_andExit(rc, "pthread_create failed");

	pthread_exit(NULL); 

	return(0); 
}
