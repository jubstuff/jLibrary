/*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.

* Nome File : jSet.c

* Scopo : Implementazione di un insieme come Hash

* Data di creazione : 17 Novembre 2011

* Autore : Giustino Borzacchiello - giustinob@gmail.com

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#include "jSet.h"
#include "jList.h"
#include "jMem.h"
#include <stdio.h>

/**
 * Alloca ed inizializza un nuovo Set.
 *
 * @param Hint Numero previsto di elementi
 * @param Op   Metodi per operare sugli elementi dell'insieme
 * @param Set  Puntatore all'insieme
 *
 * @return Esito della creazione.
 */
J_STATUS JSet_New(int Hint, JSET_METHODS *Op, J_SET **Set)
{
	J_STATUS ReturnStatus;

	int i;

	ReturnStatus = SUCCESS;
	i = 0;

	/* Alloca Set */
	ReturnStatus = MemAlloc(sizeof(J_SET), (void **)Set);
	if( ReturnStatus == SUCCESS )
	{
		/* Inizializza Set */
		(*Set)->Size = Hint;
		(*Set)->ElemCount = 0;
		(*Set)->Hash = Op->Hash;

		/* Alloca i puntatori a lista */
		ReturnStatus = MemAlloc(sizeof(J_LIST *) * Hint,
				(void **)&(*Set)->Buckets);
		if( ReturnStatus == SUCCESS )
		{
			/* Alloca metodi per liste */
			ReturnStatus = MemAlloc(sizeof(JLIST_METHODS),
					(void **)&(*Set)->ListOp);
			if( ReturnStatus == SUCCESS )
			{
				(*Set)->ListOp->InitNode = (INITIALIZER)Op->Initialize;
				(*Set)->ListOp->Delete = (DELETER)Op->Delete;
				(*Set)->ListOp->Compare = (COMPARATOR)Op->Compare;
				(*Set)->ListOp->Print = (PRINTER)Op->Print;
				(*Set)->ListOp->ManageDuplicate = NULL;
				(*Set)->ListOp->GetNodeValue = NULL;

				for( i=0; i<Hint; i++)
				{
					(*Set)->Buckets[i] = NULL;
				}
			}
			else
			{
				/* Dealloca Set e Buckets */
				MemFree((void **)(*Set)->Buckets);
				MemFree((void **)Set);
			}

		}
		else
		{
			/* dealloca Set */
			MemFree((void **)Set);
		}

	}

	return ReturnStatus;
}

/**
 * Aggiunge un elemento all'insieme.
 *
 * @param Data Dati da inserire nell'insieme
 * @param Set  Puntatore all'insieme
 *
 * @return Esito dell'operazione
 */
J_STATUS JSet_Add(void *Data, J_SET *Set)
{
	J_STATUS ReturnStatus;
	int HashKey;


	ReturnStatus = SUCCESS;

	HashKey = Set->Hash(Data) % Set->Size;
#ifdef DEBUG
	fprintf(stderr, "[JSet_Add: HashKey: %d]\n", HashKey);
#endif
	if( Set->Buckets[HashKey] == NULL )
	{
		ReturnStatus = JList_Init(&Set->Buckets[HashKey], Set->ListOp, 0);
	}

	ReturnStatus = JList_HeadInsert(Data, Set->Buckets[HashKey]);
	Set->ElemCount++;

	return ReturnStatus;
}

/**
 * Stampa a video tutti gli elementi dell'insieme.
 *
 * @param Set Puntatore all'insieme
 */
void JSet_Print( J_SET *Set )
{
	int i;

	for( i=0; i<Set->Size; i++)
	{
		if( Set->Buckets[i] )
		{
			JList_Print( Set->Buckets[i] );
		}
	}

}

/**
 * Dealloca l'insieme.
 *
 * Dealloca tutti gli elementi e le strutture collegate all'insieme.
 *
 * @param Set Puntatore all'insieme
 */
void  JSet_Destroy(J_SET *Set)
{
	int i;


	for(i=0; i<Set->Size; i++)
	{
		if( Set->Buckets[i] )
		{
			JList_Destroy( Set->Buckets[i] );
		}
	}

	MemFree( (void **)&Set->ListOp );
	MemFree( (void **)&Set->Buckets );
	MemFree( (void **)&Set );


}

/**
 * Rimuove un elemento dall'insieme.
 *
 * Deve avere in ingresso un nodo dell'insieme. Di questo elemento basta che sia
 * definito il campo che viene utilizzato come chiave nel confronto
 *
 * @param Key Dato contenente la chiave dell'elemento da cercare e rimuovere dall'insieme
 * @param Set Puntatore all'insieme
 *
 * @return Esito dell'operazione
 */
J_STATUS JSet_Delete(void *Key, J_SET *Set)
{
	J_STATUS ReturnStatus;
	int HashKey;

	ReturnStatus = SUCCESS;
	HashKey = Set->Hash(Key) % Set->Size;

	if( Set->Buckets[HashKey] != NULL )
	{
		ReturnStatus = JList_DeleteNode(Key, Set->Buckets[HashKey]);
		Set->ElemCount--;
	}
	else
	{
		ReturnStatus = ERROR;
	}

	return ReturnStatus;

}

/**
 * Cerca un elemento nell'insieme.
 *
 * Cerca un elemento nell'insieme, data la chiave, e ne restituisce il dato
 * associato
 *
 * @param Key      Dato contenente la chiave dell'elemento da cercare
 * @param OutValue TODO aggiustare. Dove salvare l'informazione associata all'elemento
 * @param Set      Puntatore all'insieme
 *
 * @return Esito della ricerca
 */
J_STATUS JSet_Search(void *Key, void **OutValue, J_SET *Set)
{
	J_STATUS ReturnStatus;
	int HashKey;

	ReturnStatus = W_NOTFOUND;
	HashKey = Set->Hash(Key) % Set->Size;
#ifdef DEBUG
	fprintf(stderr, "[JSet_Search: HashKey: %d]\n", HashKey);
#endif
	if( Set->Buckets[HashKey] )
	{
		ReturnStatus = JList_Search(Key, OutValue, Set->Buckets[HashKey]);
	}

	return ReturnStatus;
}

/**
 * Recupera il numero di elementi dell'insieme
 *
 * @param Set Puntatore all'insieme
 *
 * @return Il numero di elementi presenti nell'insieme
 */
int JSet_GetNumElem(J_SET *Set)
{
	return Set->ElemCount;
}

int JSet_GetSize(J_SET *Set)
{
	return Set->Size;
}
/**
 * Effettua un'operazione su tutti gli elementi dell'insieme
 */
void JSet_Map(APPLY apply, void *Input, J_SET *Set)
{
	int i;
	J_LIST *Bucket;

	for(i=0; i < Set->Size; i++)
	{
		/* recupera il bucket */
		Bucket = Set->Buckets[i];
		if(Bucket)
		{
			JList_Map(apply, Input, Bucket);
		}

	}

}


























