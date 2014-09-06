/**
 * Implementazioni per insieme degli archi
 */

#include "jEdgeSet.h"
#include "jMem.h"
#include <stdlib.h>
#include <stdio.h>
/*-----------------------------------------------------------------------------------*/
/*------------------------------Definizioni metodi privati---------------------------*/
/* Freelist */
int *InizializzaNodoInt( int *Value );
void DeallocaInt( int *Value );
int NumCmp( int *Num1, int *Num2 );
/* Liste di adiacenza */
static J_EDGE *InizializzaNodoEdge(J_EDGE *Value);
static void StampaNodoEdge(J_EDGE *Edge);
static void DeallocaEdge(J_EDGE *Edge);
static int EdgeCmp(J_EDGE *Edge1, J_EDGE *Edge2);
J_LIST *InizializzaAdjList(void *Input);
void DeallocaAdjList(J_LIST *L);
static J_STATUS AddEdgeAdjList(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_EDGE_SET *Eset);
static void RemoveEdgeAdjList(J_VERTEX *Source, J_VERTEX *Destination, J_EDGE_SET *Eset);
static void PrintAdjList(J_LIST **L);
/*------------------------------Definizioni metodi privati---------------------------*/
/*-----------------------------------------------------------------------------------*/


J_STATUS JEdgeSet_New(int Size, J_EDGE_SET **Eset) {
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	JLIST_METHODS Op;      /* Metodi necessari per FreeList */

	ReturnStatus = SUCCESS;

	/* Alloco la struttura insieme degli archi */
	ReturnStatus = MemAlloc(sizeof(J_EDGE_SET), (void **) Eset);
	if (ReturnStatus == SUCCESS)
	{
		/* Alloco l'array di liste di adiacenza */
		ReturnStatus = JArray_New(Size, (INITIALIZER)InizializzaAdjList, (DELETER)DeallocaAdjList,
				&((*Eset)->AdjLists));
		if (ReturnStatus == SUCCESS)
		{
			/* Inizializza la struct con le operazioni per la FreeList */
			Op.Compare = (COMPARATOR)NumCmp;
			Op.InitNode = (INITIALIZER)InizializzaNodoInt;
			Op.Delete = (DELETER)DeallocaInt;
			Op.GetNodeValue = NULL;
			Op.ManageDuplicate = NULL;
			Op.Print = NULL;
			/* Inizializza la FreeList */
			ReturnStatus = JList_Init(&(*Eset)->FreeList, &Op, 1);
			if( ReturnStatus == SUCCESS )
			{
				/* se la Freelist è stata allocata correttamente */
				/* imposta le liste di adiacenza come rappresentazione predefinita */
				(*Eset)->ActiveRappr = ADJ_LIST;
				(*Eset)->Size = Size;
				(*Eset)->VertexCount = 0;
				(*Eset)->AdjMatrix = NULL;
				(*Eset)->ActiveVertices = NULL;
			}
			else
			{
			    /* Se la Freelist non è stata allocata correttamente, dealloca
			     * le strutture dati già allocate. */
				JArray_Destroy((*Eset)->AdjLists);
				MemFree((void **) Eset);
			}
		}
		else
		{
			MemFree((void **) Eset);
		}
	}
	return ReturnStatus;
}

/**
 * Dealloca l'insieme degli archi
 */
void JEdgeSet_Destroy(J_EDGE_SET *Eset) {
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		/* Dealloca tutte le liste di adiacenza */
		JArray_Destroy(Eset->AdjLists);
		break;
	case ADJ_MATRIX:
		/* TODO Dealloca la matrice di adiacenza */
		break;
	default:
		break;
	}
	/* Dealloca la freelist */
	JList_Destroy(Eset->FreeList);
	/* Dealloca la struttura */
	MemFree((void **)&Eset);
}

J_STATUS JEdgeSet_AddEdge(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	ReturnStatus = SUCCESS;
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		AddEdgeAdjList(Source, Destination, Weight, Eset);
		break;
	case ADJ_MATRIX:
		/* TODO Aggiungere un arco alla matrice di adiacenza */
		break;
	default:
		break;
	}
	return ReturnStatus;
}

J_STATUS JEdgeSet_RemoveEdge(J_VERTEX *Source, J_VERTEX *Destination, J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	ReturnStatus = SUCCESS;
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		/* TODO rimuovere un arco dalla lista di adiacenza */
		RemoveEdgeAdjList(Source, Destination, Eset);
		break;
	case ADJ_MATRIX:
		/* TODO Rimuovere un arco alla matrice di adiacenza */
		break;
	default:
		break;
	}
	return ReturnStatus;
}

int JEdgeSet_GetNextId(J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito delle operazioni */
	int VertexId;          /* ID da associare al vertice */
	int *TempIdPtr;

	VertexId = -1;

	if( JList_isEmpty(Eset->FreeList) )
	{
		if(Eset->VertexCount == Eset->Size)
		{
			/* Raggiunta dimensione massima: realloca struttura dati */
			ReturnStatus = JArray_Resize(Eset->Size * 2, Eset->AdjLists);
			/* TODO Reallocare la AdjMatrix */
			Eset->Size *= 2;
		}
		/* La freelist è vuota: genera un nuovo ID */
		VertexId = Eset->VertexCount;
	}
	else
	{
		/* Ci sono elementi nella freelist: recupera il primo */
		JList_HeadDelete((void **)&TempIdPtr, Eset->FreeList);
		VertexId = *TempIdPtr;
		MemFree((void **)&TempIdPtr);
	}
	/* Incrementa numero di vertici */
	Eset->VertexCount++;


	return VertexId;
}

/**
 * Associa una lista di adiacenza o una riga/colonna nella matrice ad un vertice.
 *
 * Precondizione: la posizione puntata dal vertex Id esiste ed è libera sia nel
 * vettore delle liste che nella matrice
 */
J_STATUS JEdgeSet_AddVertex(J_VERTEX *V, J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	ReturnStatus = SUCCESS;
	/* Associa ID al vertice */
	JVertex_SetId(JEdgeSet_GetNextId(Eset), V);
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		/* Inizializza lista di adiacenza nella posizione corrispondente all'id del vertice.
		 * Passo NULL perché non vi sono dati da associare */
		ReturnStatus = JArray_Set(NULL, JVertex_GetId(V), Eset->AdjLists);
		break;
	case ADJ_MATRIX:
		/* TODO associare un vertice alla matrice di adiacenza */
		break;
	default:
		break;
	}
	return ReturnStatus;
}
/**
 * Rimuove un vertice dall'insieme.
 */
J_STATUS JEdgeSet_RemoveVertex(J_VERTEX *V, J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	int VertexId;          /* Id del vertice da eliminare */

	ReturnStatus = SUCCESS;
	VertexId = JVertex_GetId(V);

	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		ReturnStatus = JArray_Delete(VertexId, Eset->AdjLists);
		break;
	case ADJ_MATRIX:
		/* TODO Cancellare riga/colonna del vertice dalla AdjMatrix */
		break;
	default:
		break;
	}

	if(ReturnStatus == SUCCESS)
	{
		/* Aggiungi l'id del vertice eliminato alla freelist */
		ReturnStatus = JList_HeadInsert((void *)&VertexId, Eset->FreeList);
		if(ReturnStatus == SUCCESS)
		{
			/* Diminuisci il numero di vertici */
			Eset->VertexCount--;
		}
	}


	return ReturnStatus;
}

void JEdgeSet_Print(J_EDGE_SET *Eset)
{
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		/* Stampare insieme con liste di adiacenza */
		JArray_Map((APPLY_ARRAY)PrintAdjList, NULL, Eset->AdjLists);
		break;
	case ADJ_MATRIX:
		/* TODO Stampare insieme con matrice di adiacenza */
		break;
	default:
		break;
	}
}

int JEdgeSet_GetVertexCount(J_EDGE_SET *Eset)
{
	return Eset->VertexCount;
}
int JEdgeSet_GetSize(J_EDGE_SET *Eset)
{
	return Eset->Size;
}
/*
 * Questa struct funge da contenitore per i dati da passare alla 
 * funzione map che scorre le singole liste di adiacenza
 */
typedef struct {
	int SourceId; /**< Id numerico del vertice sorgente dell'arco */
	J_MATRIX *AdjMatrix; /**< Matrice di adiacenza in cui inserire l'arco */
} EDGE_IT;
/*
 * Funzione che viene richiamata su ogni elemento di una lista di adiacenza.
 * Inserisce l'arco nella matrice di adiacenza.
 */
static void InsertEdgeInMatrix(J_EDGE **E, EDGE_IT *SourceIdAndMatrix)
{
	JMatrix_Set((*E)->Weight, SourceIdAndMatrix->SourceId, JVertex_GetId((*E)->Destination),
			SourceIdAndMatrix->AdjMatrix );
}
/*
 * Funzione che visita una data lista di adiacenza 
 */
static void VisitAdjList( J_LIST *L, int SourceId, J_MATRIX *AdjMatrix )
{
	EDGE_IT SourceIdAndMatrix;
	SourceIdAndMatrix.AdjMatrix = AdjMatrix;
	SourceIdAndMatrix.SourceId = SourceId;
	JList_Map(InsertEdgeInMatrix, &SourceIdAndMatrix, L);
}

J_STATUS JEdgeSet_SwitchRepresentation(J_EDGE_SET *Eset)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	int i; /* Indice per cicli */
	switch (Eset->ActiveRappr)
	{
	case ADJ_LIST:
		/* Allocare una matrice di Eset->SizexEset->Size */
		ReturnStatus = JMatrix_New(Eset->Size, Eset->Size, &Eset->AdjMatrix);
		for(i=0; i<Eset->Size; i++)
		{
			JMatrix_AddRowCol(Eset->AdjMatrix);
		}
		/* Inizializzare questa matrice a infinito */
		JMatrix_SetDefaultValues( INFINITE, Eset->AdjMatrix); 
		/* Scorrere il vettore delle AdjLists e, per ogni vertice, inserire nella
		 * riga associata i pesi degli archi incontrati
		 */
		JArray_Map((APPLY_ARRAY)VisitAdjList, Eset->AdjMatrix, Eset->AdjLists);
		/* Deallocare le liste di adiacenza */
		JArray_Destroy(Eset->AdjLists);
		/* Modificare la rappresentazione attiva */
		Eset->ActiveRappr = ADJ_MATRIX;
		break;
	case ADJ_MATRIX:

		break;
	default:
		break;
	}


	return ReturnStatus;
}

/*-----------------------------------------------------------------------------------*/
/*------------------------------Implementazione metodi privati-----------------------*/

static J_EDGE *InizializzaNodoEdge(J_EDGE *Value) {
	J_EDGE *E = NULL;
	MemAlloc(sizeof(J_EDGE), (void **) &E);

	*E = *Value;
	return E;
}

static void StampaNodoEdge(J_EDGE *Edge) {
	printf("\nDestinazione arco: %s\n", Edge->Destination->Label);
	printf("Peso arco %f\n", Edge->Weight);
}

static void DeallocaEdge(J_EDGE *Edge) {
	MemFree((void **) &Edge);
}
/**
 * Dato che le liste di adiacenza non sono ordinate, questa funzione
 * ha solo il compito di verificare l'uguaglianza di due archi, dato
 * il vertice di destinazione.
 */
static int EdgeCmp(J_EDGE *Edge1, J_EDGE *Edge2) {
	int ReturnValue;

	J_VERTEX *First = Edge1->Destination;
	J_VERTEX *Second = Edge2->Destination;

	if (First == Second) {
		ReturnValue = 0;
	} else {
		ReturnValue = -1;
	}

	return ReturnValue;
}



J_LIST *InizializzaAdjList(void *Input) {
	J_LIST *L = NULL;
	JLIST_METHODS Op;

	Op.Compare = (COMPARATOR)EdgeCmp;
	Op.InitNode = (INITIALIZER)InizializzaNodoEdge;
	Op.Delete = (DELETER)DeallocaEdge;
	Op.Print = (PRINTER)StampaNodoEdge;
	Op.GetNodeValue = NULL;
	Op.ManageDuplicate = NULL;

	JList_Init(&L, &Op, 1);
	return L;
}

void DeallocaAdjList(J_LIST *L) {
	JList_Destroy(L);
}

static J_LIST *GetVertexAdjList(J_VERTEX *V, J_EDGE_SET *Eset)
{
	J_LIST *AdjList; /* Lista di adiacenza */

	AdjList = NULL;
	/* Recupera lista di adiacenza del vertice dal vettore delle liste */
	JArray_Get(JVertex_GetId(V), (void **)&AdjList, Eset->AdjLists);

	return AdjList;
}

/**
 * Aggiunge un arco alla lista di adiacenza di un vertice.
 */
static J_STATUS AddEdgeAdjList(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_EDGE_SET *Eset)
{
	J_EDGE E;
	J_LIST *AdjList;
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;
	AdjList = NULL;
	/* Recupera lista di adiacenza della sorgente */
	AdjList = GetVertexAdjList(Source, Eset);
	if( ReturnStatus == SUCCESS )
	{
		/* Crea l'arco */
		E.Destination = Destination;
		E.Weight = Weight;
		/* Aggiungi l'arco alla lista */
		ReturnStatus = JList_HeadInsert((void *)&E, AdjList );
	}
	return ReturnStatus;
}

/**
 * Rimuove un arco dalla lista di adiacenza di un vertice.
 */
static void RemoveEdgeAdjList(J_VERTEX *Source, J_VERTEX *Destination, J_EDGE_SET *Eset)
{
	int SourceId;
	J_LIST *AdjList;
	J_EDGE E;

	AdjList = NULL;
	/* Recupera id della sorgente */
	SourceId = JVertex_GetId(Source);
	/* Recupera AdjList associata al vertice */
	JArray_Get(SourceId, (void **)&AdjList, Eset->AdjLists);

	/* Crea l'arco per la ricerca */
	E.Destination = Destination;
	/* Cancello l'arco dalla AdjList */
	JList_DeleteNode(&E, AdjList);

}

static void PrintAdjList(J_LIST **L)
{
	JList_Print(*L);
}


/* Freelist */
int NumCmp( int *Num1, int *Num2 )
{
	int ReturnValue;

	if ( *Num1 < *Num2 )
	{
		ReturnValue = -1;
	}
	else if ( *Num1 == *Num2 )
	{
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = 1;
	}

	return ReturnValue;

}
void DeallocaInt( int *Value )
{
	MemFree((void **)&Value);
}
int *InizializzaNodoInt( int *Value )
{
	int *Num = NULL;
	MemAlloc( sizeof(int), (void **)&Num );

	*Num = *Value;
	return Num;
}
