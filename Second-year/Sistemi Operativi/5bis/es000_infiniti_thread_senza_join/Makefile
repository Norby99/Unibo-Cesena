CFLAGS=-ansi -Wpedantic -Wall -Werror -D_THREAD_SAFE -D_REENTRANT -D_POSIX_C_SOURCE=200112L
LIBRARIES=-lpthread

all: 	main.exe 

main.exe: main.o
	gcc -o main.exe main.o ${LIBRARIES}

main.o: main.c
	gcc ${CFLAGS} -c main.c 


.PHONY:	clean

clean:
	rm -f main.o main.exe 

run:
	./main.exe
