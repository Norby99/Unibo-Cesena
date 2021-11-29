CFLAGSCONSTRERROR=-ansi -std=c90 -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE -D_POSIX_C_SOURCE=200112L
CFLAGS=-ansi -std=c90 -Wpedantic -Wall -D_REENTRANT -D_THREAD_SAFE 
LIBRARIES=-lpthread
DBGPTHREADFUNCTION_SOURCE_DIR=./

all: 	 es1_banche.exe

es1_banche.exe:  es1_banche.o DBGpthread.o 
	gcc -o es1_banche.exe es1_banche.o DBGpthread.o ${LIBRARIES}

es1_banche.o: es1_banche.c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.h ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc -c ${CFLAGS} -I${DBGPTHREADFUNCTION_SOURCE_DIR} es1_banche.c

DBGpthread.o: ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c ${DBGPTHREADFUNCTION_SOURCE_DIR}printerror.h
	gcc ${CFLAGSCONSTRERROR} -c ${DBGPTHREADFUNCTION_SOURCE_DIR}DBGpthread.c -I${DBGPTHREADFUNCTION_SOURCE_DIR}


.PHONY:	clean

clean:
	-rm -f DBGpthread.o  es1_banche.o  es1_banche.exe

run:
	./es1_banche.exe