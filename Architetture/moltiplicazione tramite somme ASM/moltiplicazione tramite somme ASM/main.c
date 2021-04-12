#include <stdio.h>

void main() {
	unsigned short Num1 = 6;
	unsigned short Num2 = 3;
	unsigned int Prodotto;

	__asm {
		MOV AX, Num1
		MOV BX, Num2
		XOR EDX, EDX	//sara' il mio prodotto

LOOP1:	
		MOV CX, BX
		ADD EDX, EAX
		DEC BX
		LOOP LOOP1

		MOV Prodotto, EDX
	}

	printf("Prodotto = %u\n", Prodotto);
}