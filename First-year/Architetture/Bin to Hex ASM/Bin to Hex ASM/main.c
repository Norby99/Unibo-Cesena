#include <stdio.h>

void main() {

	unsigned int Num = 1254154;
	char Ris[8];

	__asm {

		MOV ECX, 8	// i

Ciclo:
		MOV EAX, Num	// temp
		AND EAX, 15
		CMP EAX, 9
		JG Lettera
		MOV EBX, 48		// val
Continua:
		ADD EBX, EAX
		MOV Ris[ECX-1], BL
		SHR Num, 4
		LOOP Ciclo
		JMP Fine

Lettera:
		MOV EBX, 55		// val
		JMP Continua
	
Fine:

	}

	{
		unsigned int i;
		for (i = 0; i < 8; i++) {
			printf("%c", Ris[i]);
		}
		printf("\n");
	}


}