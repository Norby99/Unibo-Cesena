#include <stdio.h>

// 1 = primo, 0 = non primo

void main() {
	unsigned int Num = 100003;
	int Ris;
	
	__asm {

		MOV ESI, Num	// Num
		CMP ESI, 2
		JE Primo
		JL NonPrimo
		MOV EAX, ESI
		SHR EAX, 1
		MOV EBX, EAX	// MaxDiv
		MOV ECX, 2		// D
Ciclo:
		XOR EDX, EDX
		MOV EAX, ESI	// Resto
		DIV ECX
		CMP EDX, 0
		JE NonPrimo
		INC ECX
		CMP ECX, EBX
		JLE Ciclo
Primo:
		MOV Ris, 1
		JMP Fine
NonPrimo:
		MOV Ris, 0
Fine:

	}

	printf("Ris = %d\n", Ris);
}