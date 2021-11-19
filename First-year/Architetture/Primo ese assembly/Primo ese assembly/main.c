#include <stdio.h>

void main() {

	int a = 2, b = -5, c = 0;
	
	__asm {

		MOV EAX, a
		MOV EBX, b
		CMP EAX, EBX
		JG etic
		JMP etic
		ADD EAX, EBX
		MOV c, EAX
etic:
	}

	printf("Il risultato e': %d\n", c);
}