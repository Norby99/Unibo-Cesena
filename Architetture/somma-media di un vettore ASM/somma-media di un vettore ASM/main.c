#include <stdio.h>

void main() {
	int Vettore[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1, -2, -3 };
	unsigned int Lung = sizeof(Vettore) / sizeof(int);
	int Somma;
	int Media;

	__asm {
		XOR EAX, EAX	//Somma
		MOV ECX, Lung	//L perche loop funziona solo con ECX
		
Ciclo:
		ADD EAX, Vettore[4*ECX-4]
		LOOP Ciclo

		MOV Somma, EAX
		CDQ		//Converte estendendo il segno la DWORD EAX nella QWORD EDX:EAX
		IDIV Lung	//Esegue la divisione tra EDX ed EAX

		MOV Media, EAX
	}

	printf("Somma= %d\n", Somma);
	printf("Media= %d\n", Media);
}