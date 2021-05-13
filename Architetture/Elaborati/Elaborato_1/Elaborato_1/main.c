#include <stdio.h>

void main()
{
	char frase[] = "SoNo da cOnSiderARsi EnerGie riNnovaBiLi queLlE FOrMe di energIA []genERate dA fONti chE peR loRo caRatterIsticA IntrinseCa sI rigEnErANo O Non SoNo 'ESAuribIli' nELla Scala DeI Tempi 'Umani' e, per esTenSioNE, il Cui uTilIzZo NON pRegiudIcA le riSorse NaturAlI per lE geneRazioNi future"; //Frase da valutare

	// Output
	unsigned short occorrenze[26]; //Occorrenze delle 26 lettere dell'alfabeto

	__asm
	{

		MOV ECX, 26		// Questo blocco azzera l'array che conterra' il conteggio delle lettere
AzzeraArray:
		MOV occorrenze[ECX*2-2], 0
		LOOP AzzeraArray

		MOV ECX, 1		// Questo blocco scorre la stringa e conta i caratteri presenti
		LEA ESI, [frase]
		CMP[ESI + ECX - 1], 0	// Controllo se la stringa sia vuota
		JE Fine
Ciclo :
		XOR EBX, EBX
		MOV BL, frase[ECX - 1]	// Leggo il carattere in input
		LEA EDX, [EBX - ('A')]	// Trovo la differenza tra il carattere e la prima lettera maiuscola
		MOV EAX, EBX			// Creo una copia del carattere, che verra' sostituita nel caso il carattere fosse una maiuscola
		OR EBX, 0x20
		CMP EDX, 91-65		// Controllo se maiuscola, se si, sostituisco il valore di EAX
		CMOVB EAX, EBX

		INC occorrenze[EAX*2 - 97*2]	// Incremento la lettera trovata

		INC ECX
		CMP[ESI + ECX-1], 0
		JNE Ciclo
Fine:

	}

	// Stampa su video
	{
		int i;
		for (i = 0; i < 26; i++)
		{
			printf("%c: %d\n", 97 + i, occorrenze[i]);
		}
	}
}


/*
* trova la lunghezza della stringa
		MOV EBX, 0
		LEA ESI, [frase]
strLeng:
		CMP[ESI + EBX], 0
			JE breakLength
			INC EBX
			JMP strLeng
breakLength :
		MOV lettera, BL
*/

/*
* trova l'ultimo elemento della stringa
 		XOR EAX, EAX
		LEA ESI, [frase]
strLeng:
		CMP[ESI + ECX], 0
		JE breakLength
		INC ECX
		JMP strLeng

breakLength :

		MOV AL, frase[ECX-1]
		MOV lettera, AL

*/

/*
* converte upper to lower case
XOR EBX, EBX
MOV BL, frase[ECX-1]
LEA EDX, [EBX - ('A')]
MOV EAX, EBX
OR EBX, 0x20
CMP EDX, 'Z' - 'A'
CMOVB EAX, EBX
*/

/*
* temp
CMP[ESI + ECX], 0
JE TrovatoFineArray
INC ECX
JMP Ciclo

TrovatoFineArray :
*/