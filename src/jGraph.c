#include "jGraph.h"
#include "jSet.h"
#include "jMem.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "jVertex.h"

/*
 * Metodi per inizializzare e deallocare un vertice
 */
static J_VERTEX *JGraph_VertexInit( J_VERTEX *V )
{
	return V;
}

static void JGraph_VertexDelete( J_VERTEX *V )
{
	JVertex_Destroy(V);
}

/*
 * Metodi per allocare e deallocare un arco
 */
static J_EDGE *InizializzaNodoEdge( J_EDGE *Value )
{
	J_EDGE *E = NULL;
	MemAlloc( sizeof(J_EDGE), (void **)&E);

	*E = *Value;
	return E;
}

static void StampaNodoEdge( J_EDGE *Edge )
{
	printf("\nDestinazione arco: %s\n", Edge->Destination->Label);
	printf("Peso arco %f\n", Edge->Weight );
}

static void DeallocaEdge( J_EDGE *Edge )
{
	MemFree( (void **)&Edge);
}
/**
 * Dato che le liste di adiacenza non sono ordinate, questa funzione
 * ha solo il compito di verificare l'uguaglianza di due archi, dato
 * il vertice di destinazione.
 */
static int EdgeCmp( J_EDGE *Edge1, J_EDGE *Edge2 )
{
	int ReturnValue;

	J_VERTEX *First = Edge1->Destination;
	J_VERTEX *Second = Edge2->Destination;

	if( First == Second )
	{
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = -1;
	}

	return ReturnValue;
}
/**
 * Cancella un arco dalla lista di adiacenza di un vertice
 */
static void RemoveVertexFromAdjList(J_VERTEX **Source, J_VERTEX *Destination)
{
	J_EDGE E;

	E.Destination = Destination;
	if((*Source)->AdjList)
	{
#ifdef DEBUG
		fprintf(stderr, "Cancello l'arco (%s,%s)\n", (*Source)->Label, Destination->Label);
#endif
		JList_DeleteNode((void *)&E, (*Source)->AdjList);
	}

}


/**
 * Alloca ed inizializza un nuovo grafo
 */
J_STATUS JGraph_New(int HintNumVertices, INITIALIZER InitData, DELETER DelData, J_GRAPH **G)
{
	J_STATUS ReturnStatus;
	JSET_METHODS *VerticesOp;

	ReturnStatus = SUCCESS;
	
	ReturnStatus = MemAlloc(sizeof(J_GRAPH), (void **)G);
	if( ReturnStatus == SUCCESS )
	{
		ReturnStatus = MemAlloc(sizeof(JSET_METHODS), (void **)&VerticesOp);
		if( ReturnStatus == SUCCESS )
		{
			/* Inizializzo operazioni per dati */
			(*G)->InitVertexData = InitData;
			(*G)->DeleteVertexData = DelData;

			/* Inizializzare Operazioni Vertici */
			VerticesOp->Compare = (COMPARATOR)JVertex_Compare;
			VerticesOp->Delete = (DELETER)JGraph_VertexDelete;
			VerticesOp->Initialize = (INITIALIZER)JGraph_VertexInit;
			VerticesOp->Hash = (HASH)HashVertex;
			VerticesOp->Print = (PRINTER)JVertex_Print;

			/* Crea il nuovo insieme dei vertici */
			ReturnStatus = JSet_New(HintNumVertices, VerticesOp, &(*G)->Vertices);
			MemFree((void **)&VerticesOp);
			if( ReturnStatus != SUCCESS )
			{
				MemFree((void **)G);
			}
		}
		else
		{
			MemFree((void **)G);
		}

	}

	return ReturnStatus;
}

void JGraph_Destroy(J_GRAPH *G)
{
	JSet_Destroy(G->Vertices);
	MemFree((void **)&G);
}

J_STATUS JGraph_AddVertex(char *Label, void *Data, J_GRAPH *G)
{
	J_VERTEX *V;
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;
	V = NULL;

	V = JVertex_New(Label, Data, G->InitVertexData, G->DeleteVertexData);
	ReturnStatus = JSet_Add((void *)V, G->Vertices);

	return ReturnStatus;
}

J_STATUS JGraph_SearchVertex(char *Label, J_VERTEX **OutValue, J_GRAPH *G)
{
	J_STATUS ReturnStatus;
	J_VERTEX V;

	V.Label = strdup(Label);

	ReturnStatus = W_NOTFOUND;
	ReturnStatus = JSet_Search(&V, (void **)OutValue, G->Vertices);

	free(V.Label);

	return ReturnStatus;
}

J_STATUS JGraph_AddEdge(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_GRAPH *G)
{
	J_STATUS ReturnStatus;
	J_LIST *AdjList;
	JLIST_METHODS Op;
	J_EDGE E;

	ReturnStatus = SUCCESS;

	/* Recupera la lista di adiacenza del vertice sorgente */
	AdjList = JVertex_GetAdjList(Source);

	/* Se la lista è vuota, inizializzala */
	if( AdjList == NULL )
	{
		Op.Compare = (COMPARATOR)EdgeCmp;
		Op.InitNode = (INITIALIZER)InizializzaNodoEdge;
		Op.Delete = (DELETER)DeallocaEdge;
		Op.Print = (PRINTER)StampaNodoEdge;
		Op.GetNodeValue = NULL;
		Op.ManageDuplicate = NULL;

		ReturnStatus = JList_Init(&AdjList, &Op, 1);
		JVertex_SetAdjList(AdjList, Source);
	}

	if( ReturnStatus == SUCCESS )
	{
#ifdef DEBUG
		fprintf(stderr, "Inserisco l'arco (%s,%s)\n", Source->Label, Destination->Label);
#endif
		/* Crea l'arco */
		E.Destination = Destination;
		E.Weight = Weight;
		/* Aggiungi l'arco alla lista */
		ReturnStatus = JList_HeadInsert((void *)&E, JVertex_GetAdjList(Source) );
	}

	return ReturnStatus;
}


J_STATUS JGraph_RemoveVertex( J_VERTEX *V, J_GRAPH *G )
{
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;

	JSet_Map((APPLY)RemoveVertexFromAdjList, V, G->Vertices);
	ReturnStatus = JSet_Delete(V, G->Vertices);

	return ReturnStatus;
}

J_STATUS JGraph_RemoveEdge(J_VERTEX *Source, J_VERTEX *Destination, J_GRAPH *G)
{
	J_STATUS ReturnStatus;
	J_EDGE E;

	E.Destination = Destination;
	ReturnStatus = SUCCESS;

	if(Source->AdjList)
	{
		ReturnStatus = JList_DeleteNode((void *)&E, Source->AdjList);
	}

	return ReturnStatus;
}

int JGraph_GetNumVertices(J_GRAPH *G)
{
	return JSet_GetNumElem(G->Vertices);
}



/*-------------------------------------------------------------------
 * -------------------TRASPOSTO--------------------------------------
 */
/**
 * Inserisce i vertici nel grafo trasposto e gli archi, invertendoli
 */
static void InsertVertexInTranspose(J_VERTEX **Vertex, J_GRAPH *G)
{
	JGraph_AddVertex((*Vertex)->Label, (*Vertex)->Data, G);
}
typedef struct {
	J_VERTEX *Source;
	J_GRAPH *Gt;
} VERTEX_GRAPH;
static void InsertEdges(J_EDGE **Edge, VERTEX_GRAPH *Vg)
{
	J_VERTEX *Source;
	J_VERTEX *Destination;
#ifdef DEBUG
	printf("Arco (%s,%s) - [%f]\n", Vg->Source->Label, (*Edge)->Destination->Label, (*Edge)->Weight);
#endif
	JGraph_SearchVertex(Vg->Source->Label, &Source, Vg->Gt);
    JGraph_SearchVertex((*Edge)->Destination->Label, &Destination, Vg->Gt);
    /* Inserisco l'arco con i ruoli sorgente e destinazione invertiti */
    JGraph_AddEdge(Destination, Source, (*Edge)->Weight, Vg->Gt);

}
static void InsertEdgesInTranspose(J_VERTEX **Vertex, J_GRAPH *Gt)
{
	VERTEX_GRAPH Vg;

	if( JVertex_GetAdjList((*Vertex)) )
	{
		Vg.Source = (*Vertex);
		Vg.Gt = Gt;
		JList_Map((APPLY)InsertEdges, &Vg, JVertex_GetAdjList((*Vertex)));
	}

}

J_STATUS JGraph_Transpose(J_GRAPH *G, J_GRAPH **Gt)
{
	J_STATUS ReturnStatus;

	ReturnStatus = JGraph_New(JSet_GetSize(G->Vertices), G->InitVertexData, G->DeleteVertexData, Gt);
	/* Aggiungi tutti i vertici al grafo trasposto */
	JSet_Map((APPLY)InsertVertexInTranspose, *Gt, G->Vertices);
	JSet_Map((APPLY)InsertEdgesInTranspose, *Gt, G->Vertices);
	return ReturnStatus;
}
/*-------------------TRASPOSTO--------------------------------------
 *------------------------------------------------------------------
 */

/*-------------------------------------------------------------------
 * -------------------STAMPA--------------------------------------
 */
static void PrintEdge(J_EDGE **Edge, J_VERTEX *Source)
{
	printf("Arco (%s,%s) - [%f]\n", Source->Label, (*Edge)->Destination->Label, (*Edge)->Weight);

}

static void PrintVertexAndAdjList(J_VERTEX **Vertex, void *Input)
{
	JVertex_Print(*Vertex);
	if( JVertex_GetAdjList((*Vertex)) )
	{
		JList_Map((APPLY)PrintEdge, *Vertex, JVertex_GetAdjList((*Vertex)));
	}
}

void JGraph_Print( J_GRAPH *G )
{
	JSet_Map((APPLY)PrintVertexAndAdjList, NULL, G->Vertices);
}
/*-------------------STAMPA--------------------------------------
 *------------------------------------------------------------------
 */

typedef struct {
	J_VERTEX *Source;
	J_GRAPH *G;
} DFS_INFO;

static void DfsVisitAdjList(J_EDGE **Edge, DFS_INFO *Vg);
static void InitVerticesForDfsVisit(J_VERTEX **Vertex, void *Input);
static void DfsVisit(J_VERTEX **Vertex, J_GRAPH *G);

void JGraph_Dfs(J_GRAPH *G)
{
	/* Inizializza tutti i vertici per la visita */
	JSet_Map((APPLY)InitVerticesForDfsVisit, NULL, G->Vertices);
	JSet_Map((APPLY)DfsVisit, G, G->Vertices);
}

static void InitVerticesForDfsVisit(J_VERTEX **Vertex, void *Input)
{
	(*Vertex)->Color = WHITE;
	(*Vertex)->Pred = NULL;
	(*Vertex)->DiscoveryTime = -1;
	(*Vertex)->FinishTime = -1;
}

static void DfsVisit(J_VERTEX **Vertex, J_GRAPH *G)
{
	if( (*Vertex)->Color == WHITE )
	{
#ifdef DEBUG
	printf("[DfsVisit: Visito il vertice %s]\n", (*Vertex)->Label);
#endif
		(*Vertex)->Color = GRAY;
		DFS_INFO Vg;

		G->Time++;
		(*Vertex)->DiscoveryTime = G->Time;
		if( JVertex_GetAdjList((*Vertex)) )
		{
			Vg.Source = (*Vertex);
			Vg.G = G;
			JList_Map((APPLY)DfsVisitAdjList, &Vg, JVertex_GetAdjList((*Vertex)));
		}
		(*Vertex)->Color = BLACK;

		G->Time++;
		(*Vertex)->FinishTime = G->Time;

	}

}

static void DfsVisitAdjList(J_EDGE **Edge, DFS_INFO *Vg)
{
#ifdef DEBUG
	printf("[DfsVisit: Visito l'arco (%s,%s)]\n", Vg->Source->Label, (*Edge)->Destination->Label);
#endif
	if( (*Edge)->Destination->Color == WHITE )
	{
		(*Edge)->Destination->Pred = Vg->Source;
		DfsVisit( &((*Edge)->Destination), Vg->G);
	}
}
















