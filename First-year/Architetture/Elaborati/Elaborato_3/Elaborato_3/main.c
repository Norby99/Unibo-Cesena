#include <stdio.h>

void main()
{
	// Variabili
#define MaxLen 1024
	/*int Len = 30;							// Lunghezza della sequenza da comprimere
	unsigned char Buffer[MaxLen] = { 1,2,3,3,3,3,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,	// I dati da
									4,5,99,99,99,255,255,255,255 };				// comprimere
	*/
	int Len = 258;
	unsigned char Buffer[MaxLen] = { 97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,255 };

#define MaxLenComp (MaxLen*2+1)
	unsigned char BufComp[MaxLenComp];		// Vettore in cui mettere i dati compressi

	// Blocco assembler
	__asm
	{
		// Allocazione delle variabili
		MOV ECX, Len	// Lunghezza dell'array
		MOV EBX, 1		// k
		XOR EDX, EDX	// i (ovver il registro che scorre l'array Buffer)
		JECXZ ArrayVuoto
Ciclo:	// Ciclo l'array fino ad arrivare in fondo
		XOR EAX, EAX
		MOV AL, Buffer[EDX]		//val
		MOV BufComp[EBX], AL
		MOV BufComp[EBX - 1], 0
ValRipetuto:	// Controllo se il valore e' ripetuto e in quel caso incremento il contatore del numero. Inoltre controllo che il valore non sia ripetuto piu' di 255, in questo caso spezzo il contatore
		INC BufComp[EBX - 1]
		INC EDX
		CMP EDX, ECX
		JNL FineArray	// Se sono arrivato a fine array, esco dal ciclo
		CMP BufComp[EBX - 1], 255
		JE ValNuovo
		CMP Buffer[EDX], AL
		JE ValRipetuto
ValNuovo:
		ADD EBX, 2
		JMP Ciclo
ArrayVuoto:	// Se l'array e' vuoto e' necessario decrementare l'indice di partenza
		SUB EBX, 2
FineArray:	// Inserimento del numero terminatore all'interno dell'array
		MOV BufComp[EBX+1], 0
	}

	// Stampa su video
	{
		int i;
		for (i = 0; i < Len; i++)
		{
			printf("%02X ", Buffer[i]);
		}
		printf("\n\n");
		for (i = 0; BufComp[i];)
		{
			printf("%3d ", BufComp[i++]);
			printf("%2X ", BufComp[i++]);
		}
		printf("\n");
	}
}

/*
* Temp

		// Allocazione delle variabili
		MOV ECX, Len	// i
		MOV EBX, 1		// k
		JECXZ ArrayVuoto
Ciclo:	// Ciclo l'array fino ad arrivare in fondo
		XOR EAX, EAX
		MOV AL, Buffer[ECX - 1]		//val
		MOV BufComp[EBX], AL
		MOV BufComp[EBX - 1], 0
ValRipetuto:	// Controllo se il valore e' ripetuto e in quel caso incremento il contatore del numero
		INC BufComp[EBX - 1]
		DEC ECX
		CMP ECX, 0
		JLE FineArray
		CMP Buffer[ECX - 1], AL
		JE ValRipetuto
		ADD EBX, 2
		JMP Ciclo
ArrayVuoto:	// Se l'array e' vuoto e' necessario decrementare l'indice di partenza
		SUB EBX, 2
FineArray:	// Inserimento del numero terminatore all'interno dell'array
		MOV BufComp[EBX+1], 0


*/