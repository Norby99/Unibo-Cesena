#include <stdio.h>


void main()
{
	// Variabili
	int N = 3;	// numero di interi (provare con valori <=6)	dimensione = 4
	int Perm[4326];	// array permutazioni: la dimensione è sufficiente per N<=6
	int Num; 	// Alla fine deve contenere il numero di permutazioni generate

	// Blocco assembler
	__asm
	{
		JMP Main
Fattoriale:		// Caloca il fattoriale e lo inserisce in ECX
		IMUL ECX, EAX
		DEC EAX
		CMP EAX, 0
		JG Fattoriale
		RET
Scambio:	// Scambio tra Perm[EBX*4] - Perm[EAX*4]
		PUSH EDI	// Salvo i valori dei due registri EDI ed ECX, in quanto i loro valori non verrano utilizzati in questa parte di codice, per poter usufruire di altri 2 registri
		PUSH ECX
		MOV EDI, Perm[EBX*4]
		MOV ECX, Perm[EAX*4]
		MOV Perm[EBX*4], ECX
		MOV Perm[EAX*4], EDI
		POP ECX
		POP EDI
		RET
Minore:
		MOV EAX, ECX
		IMUL EAX, N		// Primo indice da scambiare
		MOV EBX, EAX
		ADD EBX, EDI	// Secondo indice da scambiare
		TEST EDI, 1		// Controllo se il numero e' pari e scambio i due valori 
		JZ Pari
		JMP Dispari
FineMinore:
		INC ECX		// Incremento la riga da modificare
		INC Perm[EDI*4]	// incremento il pivot
		MOV EDI, 1	// Setto a 1 l'indice con cui ciclo l'array
		JMP FineConfronto
Pari:
		CALL Scambio
		JMP FineMinore
Dispari:
		ADD EAX, Perm[EDI*4]
		CALL Scambio
		JMP FineMinore
NonUltimaRiga:	// copio la riga precedente in quella successiva
		PUSH ESI	// Libero ESI, cosi ho un altro registro a disposizione
		XOR ESI, ESI	// k
Copia:
		MOV EAX, ECX
		IMUL EAX, N
		ADD EAX, ESI	// Primo indice da considerrare
		MOV EDX, ECX
		DEC EDX
		IMUL EDX, N
		ADD EDX, ESI	// Secondo indice da considerrare
		MOV EBX, Perm[EDX*4]
		MOV Perm[EAX*4], EBX	// La formula e' Perm[ riga*n + k ] = Perm[(riga-1)*n + k]
		INC ESI
		CMP ESI, N
		JL Copia
		POP ESI	// Rialloco il vecchio valore di ESI
		JMP FineCiclo
Main:
		MOV ECX, 1	// Trovo il numero di permutazioni
		MOV EAX, N
		MOV BH, AL	// Trovo il numero di volte da inserire
		CALL Fattoriale
		MOV Num, ECX	// Assegno Num
		MOV ESI, ECX	// fact
		MOV ECX, N
Preparazione:
		MOV EAX, N
		ADD EAX, ECX
		MOV Perm[ECX*4 - 4], 0	// Azzero la prima riga, che verra' utilizzata in seguito come pivot per eseguire le permutazioni
		MOV Perm[EAX * 4 - 4], ECX	// Imposto la seconda riga con la prima permutazione
		LOOP Preparazione
		MOV EDI, 1	// i
		MOV ECX, 1	// riga
Ciclo:
		CMP Perm[EDI*4], EDI	// Controllo che il valore del pivot sia minore del mio indice
		JL Minore
		MOV Perm[EDI*4], 0	// Maggiore
		INC EDI
FineConfronto:
		CMP ECX, ESI	// Controllo di non trovarmi nell'ultima riga
		JL NonUltimaRiga
FineCiclo:
		CMP EDI, N
		JL Ciclo	// Ripeto finche EDI e' minore di N

		MOV ECX, N
PrimaRiga:	// Setto la prima riga precedentemente usato come pivot con i valori di default
		MOV Perm[ECX * 4 - 4], ECX
		LOOP PrimaRiga
	}

	// Stampa su video
	{
		printf("--%d\n", Num);
		int i, j, k;
		printf("Permutazioni dei primi %d numeri naturali\n", N);
		for (i = k = 0; i < Num; i++)
		{
			for (j = 0; j < N; j++)
			{
				printf("%3d", Perm[k++]);
			}
			printf("\n");
		}
	}
}


/*
* Primo test (finito male)
JMP Main

Fattoriale:
		IMUL ECX, EAX
		DEC EAX
		CMP EAX, 0
		JG Fattoriale
		RET

RiempiColonna:
		IMUL BL, 4
		IMUL ECX, 4
		XOR EDX, EDX

		ADD EBX, ECX
		MOV Perm[EBX], EAX


		LOOP RiempiColonna
		RET




Main:
		MOV ECX, 1	// Trovo il numero di permutazioni
		MOV EAX, N
		MOV BH, AL	// Trovo il numero di volte da inserire
		CALL Fattoriale
		MOV Num, ECX
		MOV BL, 0	// Colonna da inserire
Ciclo:
		MOV ECX, 1
		XOR EAX, EAX
		MOV AL, BH
		DEC EAX
		CALL Fattoriale	// Numero di volte da inserire
		MOV EAX, 1	// Numero da inserire
		CALL RiempiColonna	// ECX=quante volte lo inserisco, AL=numero da inserire, BL=colonna dove inserire
		DEC EAX
		INC BL		// Colonna dove inserire

*/