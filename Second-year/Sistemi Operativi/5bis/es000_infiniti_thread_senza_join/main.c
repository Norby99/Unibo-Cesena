#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>	/* uintptr_t */
#include <inttypes.h>	/* PRIiPTR */
#include <pthread.h>

void printError(int);

void *PrintHello(void *arg) {
	intptr_t var = (intptr_t)arg;
	printf("\n %"  PRIiPTR ": Hello World!\n", var );
	pthread_exit (NULL);
}

void printError(int rc) {
	char msg[100]; int ret;
	if (rc) {
		ret = strerror_r(rc,msg,100);
		if( ret != 0 ) { /* anche la funzione strerror_r potrebbe causare errore */
			printf("strerror failed: errore %i \n", ret ); fflush(stdout);
		}
	/* stampo il messaggio di errore provocato dalla join */
	printf("pthread_join failed: %i %s\n", rc, msg );
	exit(1);
	}
}

int main() {
	pthread_t tid;
	int rc;
	intptr_t t;
	for(t=0; 1; t++){

		printf("Creating thread %" PRIiPTR "\n", t);
		rc = pthread_create (&tid, NULL, PrintHello, (void*)t );
		printError(rc);
	}

	pthread_exit (NULL);  /* far vedere commentando questa riga */
	return(0);
}
