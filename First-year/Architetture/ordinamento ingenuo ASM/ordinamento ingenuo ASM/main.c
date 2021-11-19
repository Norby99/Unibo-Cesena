#include <stdio.h>

void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void selectionSort(int arr[], int n)
{
    int i, j, min_idx;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}

void main() {

	int Vettore[] = { 3, 7, 3, 7, 5, 1, 4, -3, -7, -9, 2, 6 };
	int vett2[100];
	unsigned int Lung = sizeof(Vettore) / sizeof(Vettore[0]);
	memcpy(vett2, Vettore, sizeof(Vettore));
	selectionSort(vett2, Lung);

	__asm {
		MOV ECX, Lung
		DEC ECX	// Pos

Ciclo:
		MOV EAX, Vettore[4 * ECX]	// Max 
		MOV EBX, ECX	// MaxPos
		MOV EDX, ECX	// I

ControlloFineArray:
		DEC EDX
		CMP EDX, -1
		JE FineArray
		CMP EAX, Vettore[4 * EDX]
		JG ControlloFineArray
		MOV EAX, Vettore[4 * EDX]
		MOV EBX, EDX
		JMP ControlloFineArray

FineArray:
		MOV ESI, Vettore[4 * ECX]
		MOV Vettore[4 * ECX], EAX
		MOV Vettore[4 * EBX], ESI
		DEC ECX
		JNS Ciclo	// Finchè ECX maggiore uguale a zero
					/*	Si potrebbe fare anche:
						CMP ECX, 0
						JGE Ciclo*/
	}

	{
		for (int i = 0; i < Lung; i++) {
			printf("%d ", Vettore[i]);
		}

	}
}