#include <stdio.h>

void main() {
	unsigned int Num = 16;
	unsigned int Radice;

	__asm {
		XOR EAX, EAX	//Radice
		MOV EBX, 1	//S
		MOV ECX, Num
Ciclo:
		SUB ECX, EBX
		JL Fine
		INC EAX
		ADD EBX, 2
		JMP Ciclo
Fine: 
		MOV Radice, EAX

	}

	printf("Radice= %d\n", Radice);
}