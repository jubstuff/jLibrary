/**
 * @file   jArray.c
 * @brief  Implementazione di un array dinamico.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Aprile, 2012
 *
 * Implementazione per array dinamico.
 */

#include "jArray.h"
#include "jMem.h"

#define DEFAULT_SIZE 10 /**< Dimensione predefinita dell'array */

J_STATUS JArray_New(int StartSize, INITIALIZER Init, DELETER Delete, J_ARRAY **A)
{
	J_STATUS ReturnStatus; /* Valore di ritorno */
	int i; /* Indice per cicli */

	ReturnStatus = SUCCESS;

	if( Init && Delete )
	{
		/* Alloco la struttura contenente l'array */
		ReturnStatus = MemAlloc(sizeof(J_ARRAY), (void **)A);
		if( ReturnStatus == SUCCESS )
		{
			if( StartSize <= 0 )
			{
				/* Se non viene passato un valore valido, imposto una
				 * dimensione iniziale di default */
				StartSize = DEFAULT_SIZE;
			}

			ReturnStatus = MemAlloc(StartSize * sizeof(void *), (void **)(&(*A)->Array));
			if( ReturnStatus == SUCCESS )
			{
				/* se l'array è stato allocato correttamente */


					/* Imposta la dimensione */
					(*A)->Size = StartSize;
					/* Imposta il numero iniziale di elementi a zero */
					(*A)->ElemCount = 0;
					/* Inizializza tutti gli elementi a null */
					for(i=0; i<(*A)->Size; i++)
					{
						(*A)->Array[i] = NULL;
					}
					/* Inizializza le funzioni di init e delete */
					(*A)->InitData = Init;
					(*A)->DeleteData = Delete;
			}
		}
		else
		{
			/* Se l'array non è stato allocato, dealloca la struttura */
			MemFree((void **)A);
		}

	}
	else
	{
		/* non sono state passate le funzioni di init e delete */
		ReturnStatus = ERROR;
	}

	return ReturnStatus;
}

int JArray_GetSize(J_ARRAY *A)
{
	int Size; /* Dimensione dell'array */
	Size = -1;
	if(A)
	{
		/* Se l'array esiste, recupera la dimensione */
		Size = A->Size;
	}
	return Size;
}

int JArray_GetElemCount(J_ARRAY *A)
{
	int ElemCount; /* Numero di elementi nell'array */
	ElemCount = -1;
	if(A)
	{
		/* Se l'array esiste, recupera il numero di elementi */
		ElemCount = A->ElemCount;
	}
	return ElemCount;
}

void JArray_Destroy( J_ARRAY *A )
{
	int i; /* Indice per cicli */
	for(i = 0; i < A->Size; i++)
	{
		if(A->Array[i])
		{
			/* Se l'elemento esiste, deallocalo */
			A->DeleteData(A->Array[i]);
		}
	}
	/* Dealloca array */
	MemFree((void **)&(A->Array));
	/* Dealloca struttura */
	MemFree((void **)&A);
}

J_STATUS JArray_Set(void *Data, int Pos, J_ARRAY *A)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */

	ReturnStatus = SUCCESS;
	if( (Pos >= 0) && (Pos < A->Size) )
	{
		/* Se l'indice è minore della dimensione dell'array */
		if(A->Array[Pos])
		{
			/* se già esiste un elemento in quella posizione bisogna deallocarlo */
			JArray_Delete(Pos, A);
		}
		/* inizializza un elemento e associalo a quella posizione */
		A->Array[Pos] = A->InitData(Data);
		/* Incrementa il numero di elementi */
		A->ElemCount++;
	}
	else
	{
		ReturnStatus = W_OUTOFBOUND;
	}

	return ReturnStatus;
}

J_STATUS JArray_Get(int Pos, void **Out, J_ARRAY *A)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */

	ReturnStatus = SUCCESS;
	if(Pos < A->Size)
	{
		/* Se l'indice cercato è compreso nei limiti dell'array,
		 * restituisci l'elemento
		 */
		*Out = A->Array[Pos];
	}
	else
	{
		/* Altrimenti segnala superamento dei limiti */
		*Out = NULL;
		ReturnStatus = W_OUTOFBOUND;
	}
	return ReturnStatus;
}

J_STATUS JArray_Delete(int Pos, J_ARRAY *A)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */

	ReturnStatus = SUCCESS;
	if( Pos < A->Size)
	{
		if(A->Array[Pos])
		{
			/* se esiste un elemento in quella posizione bisogna deallocarlo */
			A->DeleteData(A->Array[Pos]);
			A->Array[Pos] = NULL;
			/* Decrementa il numero di elementi */
			A->ElemCount--;
		}
	}
	else
	{
		ReturnStatus = W_OUTOFBOUND;
	}

	return ReturnStatus;
}

J_STATUS JArray_Resize(int Size, J_ARRAY *A)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	int i; /* Variabile per cicli */

	ReturnStatus = SUCCESS;
	if( Size > 0 )
	{
		if(Size < A->Size)
		{
			/*
			 * Se la dimensione è minore di quella attuale, dealloca gli
			 * elementi che si trovano oltre il limite
			 */
			for(i = Size; i < A->Size; i++)
			{
				if(A->Array[i])
				{
					A->DeleteData(A->Array[i]);
					A->ElemCount--;
				}
			}
			/* Realloca l'array */
			ReturnStatus = MemRealloc(Size * sizeof(void *), (void **)&(A->Array));
			if(ReturnStatus == SUCCESS )
			{
				/* Reimposta la dimensione dell'array */
				A->Size = Size;
			}
		}
		else
		{
			/*
			 * Dimensione maggiore di quella attuale. Allocare altro spazio
			 */
			ReturnStatus = MemRealloc(Size * sizeof(void *), (void **)&(A->Array));
			for(i = A->Size; i < Size; i++)
			{
				A->Array[i] = NULL;
			}
			A->Size = Size;
		}
	}
	else
	{
		/* Nuova dimensione negativa: segnalare errore */
		ReturnStatus = ERROR;
	}

	return ReturnStatus;
}
/**
 * Richiama una funzione su tutti gli elementi dell'array
 */
void JArray_Map(APPLY_ARRAY Apply, void *Input, J_ARRAY *A)
{
	int i; /* Indice per cicli */

	for(i = 0; i < A->Size; i++)
	{
		if(A->Array[i])
		{
			/* Se l'elemento i-esimo esiste, richiama la funzione con l'input */
			Apply(&(A->Array[i]), i, Input);
		}
	}
}
