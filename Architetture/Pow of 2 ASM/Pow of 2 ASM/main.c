#include <stdio.h>

void main() {

	unsigned int Num = 17;
	unsigned int Ris;

	__asm{

		XOR EAX, EAX // Ris
		MOV ECX, Num // Num

		JECXz Fine // Solo se ECX == 0

Ciclo:
		TEST ECX, 1	// fa la AND
		JNZ TrovatoBitUno	// Solo se != 0

		SHR ECX, 1
		JMP Ciclo

TrovatoBitUno:

		CMP ECX, 1
		JE PotenzaDue
		JMP Fine

PotenzaDue:
		MOV EAX, 1 // INC EAX

Fine:
		MOV Ris, EAX


	}

	printf("Ris = %d\n", Ris);
}