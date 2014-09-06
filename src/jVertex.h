/**
 * @file   jVertex.h
 * @brief  Definizione di tipi e prototipi di funzioni per il tipo Vertice
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Febbraio, 2012
 *
 * Il tipo J_VERTEX rappresenta un vertice del grafo. Ha associato un'etichetta
 * e dei dati.
 */
#include "common.h"

#ifndef JVERTEX_H
#define JVERTEX_H

typedef enum {
	WHITE,
	GRAY,
	BLACK
} COLOR;

typedef struct jvertex_tag J_VERTEX;

struct jvertex_tag
{
	int Id;          /**< ID univoco del vertice */
	char *Label;     /**< Etichetta del vertice */
	void *Data;      /**< Dati associati al vertice */

	/* costanti per BFS, DFS */
	COLOR Color;
	J_VERTEX *Pred;
	int DiscoveryTime;
	int FinishTime;
	int Distance;

	DELETER Delete; /**< Funzione di deallocazione dei dati associati al vertice */
};

J_VERTEX *JVertex_New( char *Label, void *Data, INITIALIZER Init, DELETER Del );

void JVertex_Destroy( J_VERTEX *V );

int JVertex_Compare( J_VERTEX *Vertex1, J_VERTEX *Vertex2 );

unsigned int HashVertex(J_VERTEX *V);

void JVertex_Print( J_VERTEX *V );

char *JVertex_GetLabel( J_VERTEX *V );

void *JVertex_GetData( J_VERTEX *V );

void JVertex_SetId( int Id, J_VERTEX *V );
int JVertex_GetId( J_VERTEX *V );

#endif /* JVERTEX_H */
