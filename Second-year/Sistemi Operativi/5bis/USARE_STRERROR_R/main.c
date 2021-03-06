#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h> /* per strerror_r */

int main() {
	pthread_t tid; int rc; void *ptr;
	/* prendo il mio stesso identificatore di thread */
	tid = pthread_self();
	/* tento join con me stesso ma provoca errore perché mi bloccherei aspettando la mia morte */
	rc = pthread_join( tid , &ptr );

	if (rc) {
		char msg[100]; int ret;
		ret=strerror_r(rc,msg,100);
		if( ret != 0 ) { /* anche la funzione strerror_r potrebbe causare errore */
			printf("strerror failed: errore %i \n", ret ); fflush(stdout);
		}
	/* stampo il messaggio di errore provocato dalla join */
	printf("pthread_join failed: %i %s\n", rc, msg );
	}

	return(0);
}
