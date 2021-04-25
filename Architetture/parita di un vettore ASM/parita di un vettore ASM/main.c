#include <stdio.h>

void main() {

	unsigned char Vettore[] = { 3, 7, 3, 7, 5 };
	unsigned int Lung = sizeof(Vettore) / sizeof(Vettore[0]);
	int Ris;

	__asm {
		XOR EAX, EAX // Somma
		MOV ECX, Lung	// Lung
CicloByte:
		MOV BL, Vettore[ECX-1]	// Val
CicloBit:
		CMP BL, 0
		JE ProssimoByte
		TEST BL, 1
		JZ ProssimoBit	// Se == 0
		INC EAX

ProssimoBit:
		SHR BL, 1
		JMP CicloBit

ProssimoByte:
		LOOP CicloByte
		
		AND EAX, 1
		MOV Ris, EAX

	}

	printf("Ris = %d\n", Ris);
}