/**
 * @file   jEdgeSet.h
 * @brief  Definizioni e strutture per l'insieme degli archi di un grafo
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Marzo, 2012
 *
 *
 * jEdgeSet rappresenta l'insieme degli archi per un grafo. Astrae le
 * reali implementazioni dell'insieme (liste di adiacenza o matrice di adiacenza)
 * dietro un'interfaccia comune, in modo che sia possibile aggiungere
 * altre rappresentazioni in futuro.
 */

#ifndef J_EDGE_SET_H
#define J_EDGE_SET_H

#include "errors.h"
#include "jArray.h"
#include "jMatrix.h"
#include "jVertex.h"
#include "jList.h"

#define INFINITE __builtin_inf()

/**
 * Tipo di rappresentazione dell'insieme degli archi
 */
typedef enum
{
	ADJ_LIST,  /**< Lista di adiacenza */
	ADJ_MATRIX /**< Matrice di adiacenza */
} ESET_TYPE;

/**
 * Insieme degli archi
 *
 * Questa struttura contiene tutti gli elementi per memorizzare
 * l'insieme degli archi di un grafo.
 */
typedef struct j_edge_set
{
	int Size;                /**< Numero massimo di vertici */
	int VertexCount;         /**< Numero di vertici attuali */

	J_ARRAY *AdjLists;       /**< Vettore delle liste di adiacenza */

	J_MATRIX *AdjMatrix;     /**< Matrice di adiacenza */
	J_ARRAY *ActiveVertices; /**< Vertici attivi nella matrice */

	J_LIST *FreeList;        /**< Lista delle locazioni libere */

   ESET_TYPE ActiveRappr;   /**< Rappresentazione attualmente attiva dell'insieme */

} J_EDGE_SET;
/**
 * Arco di un grafo.
 *
 * Questa struct rappresenta un arco del grafo.
 */
typedef struct j_edge_tag
{
	double Weight;         /**< Peso dell'arco */
	J_VERTEX *Destination; /**< Destinazione dell'arco */
} J_EDGE;


/**
 * Alloca l'insieme degli archi
 */
J_STATUS JEdgeSet_New(int Size, J_EDGE_SET **Eset);
/**
 * Dealloca l'insieme degli archi
 */
void JEdgeSet_Destroy( J_EDGE_SET *Eset);
/**
 * Aggiunge un vertice all'insieme.
 */
J_STATUS JEdgeSet_AddVertex(J_VERTEX *V, J_EDGE_SET *Eset);
/**
 * Rimuove un vertice dall'insieme.
 */
J_STATUS JEdgeSet_RemoveVertex(J_VERTEX *V, J_EDGE_SET *Eset);
/**
 * Aggiunge un arco all'insieme.
 */
J_STATUS JEdgeSet_AddEdge(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_EDGE_SET *Eset);
/**
 * Rimuove un arco dall'insieme
 */
J_STATUS JEdgeSet_RemoveEdge(J_VERTEX *Source, J_VERTEX *Destination, J_EDGE_SET *Eset);
/**
 * Stampa l'insieme dei vertici
 */
void JEdgeSet_Print(J_EDGE_SET *Eset);

int JEdgeSet_GetVertexCount(J_EDGE_SET *Eset);
int JEdgeSet_GetSize(J_EDGE_SET *Eset);
int JEdgeSet_GetNextId(J_EDGE_SET *Eset);


#endif /* J_EDGE_SET_H */
