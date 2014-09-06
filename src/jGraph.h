#ifndef JGRAPH_H
#define JGRAPH_H

#include "jSet.h"
#include "errors.h"
#include "jVertex.h"
#include "jEdgeSet.h"

typedef struct j_graph_tag
{
	J_SET *Vertices; /**< Insieme dei vertici */
	J_EDGE_SET *Edges;

	int Time; /**< Contatore dei tempi di scoperta e terminazione per DFS */

	INITIALIZER InitVertexData;
	DELETER DeleteVertexData;
} J_GRAPH;

typedef struct j_edge_tag
{
	double Weight; /**< Peso dell'arco */
	J_VERTEX *Destination; /**< Destinazione dell'arco */
} J_EDGE;

/**
 * Alloca un nuovo grafo
 */
J_STATUS JGraph_New(int HintNumVertices, INITIALIZER InitData, DELETER DelData, J_GRAPH **G);
/**
 * Dealloca un grafo e tutte le strutture di supporto
 */
void JGraph_Destroy(J_GRAPH *G);
/**
 * Aggiunge un vertice al grafo
 */
J_STATUS JGraph_AddVertex(char *Label, void *Data, J_GRAPH *G);
/**
 * Rimuove un vertice dal grafo
 */
J_STATUS JGraph_RemoveVertex( J_VERTEX *V, J_GRAPH *G );
/**
 * Cerca un vertice nel grafo
 */
J_STATUS JGraph_SearchVertex(char *Label, J_VERTEX **OutValue, J_GRAPH *G);
/**
 * Aggiunge un arco al grafo
 */
J_STATUS JGraph_AddEdge(J_VERTEX *Source, J_VERTEX *Destination, double Weight, J_GRAPH *G);
/**
 * Rimuove un arco dal grafo
 */
J_STATUS JGraph_RemoveEdge(J_VERTEX *Source, J_VERTEX *Destination, J_GRAPH *G);

/**
 * Recupera il numero di vertici nel grafo
 */
int JGraph_GetNumVertices(J_GRAPH *G);

/**
 * Crea il trasposto del grafo
 */
J_STATUS JGraph_Transpose(J_GRAPH *G, J_GRAPH **Gt);
/**
 * Stampa il grafo: vertici e archi
 */
void JGraph_Print( J_GRAPH *G );

void JGraph_Dfs(J_GRAPH *G);

#endif
