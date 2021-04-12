#include <stdio.h>

void main(){
	char Stringa[] = "Questa è una stringa";
	int Lunghezza = sizeof(Stringa) - 1;
	char Carattere = 'a';
	int Posizione;
	
	__asm {
		MOV CH, Carattere
		MOV EBX, Lunghezza	// len
		XOR EDX, EDX // i

Ciclo:
		CMP EDX, EBX
		JGE NonTrovato // >=
		MOV CL, Stringa[EDX]
		CMP CL, CH
		JE Fine // ==
		INC EDX
		JMP Ciclo // ripeti finche'

NonTrovato :
		MOV Posizione, -1
		JMP True
Fine:
		MOV Posizione, EDX

True:


	}

	printf("Posizione = %d\n", Posizione);
}