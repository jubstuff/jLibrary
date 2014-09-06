#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "jVertex.h"
#include "jGraph.h"

typedef struct elem_tag
{
    int Key;
    int Data;
} ELEM;

ELEM *InizializzaElemento( ELEM *El )
{
	ELEM *Out = NULL;
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;

	ReturnStatus = MemAlloc(sizeof(ELEM), (void **)&Out);
	if( ReturnStatus == SUCCESS )
	{
		Out->Key = El->Key;
		Out->Data = El->Data;
	}
	return Out;
}

void CancellaElemento( ELEM *El )
{
	MemFree((void **)&El);
}         


J_GRAPH *G;
J_STATUS ReturnStatus;

 
void setUp(void)
{
    JGraph_New(20, (INITIALIZER)InizializzaElemento, (DELETER)CancellaElemento, &G);
}

void tearDown(void)
{
    JGraph_Destroy( G );
}

void CreaGrafo(J_GRAPH *G)
{
    J_VERTEX *A;
    J_VERTEX *B;
    J_VERTEX *C;
    J_VERTEX *D;
    J_VERTEX *E;
    JGraph_AddVertex("A", NULL, G);
    JGraph_AddVertex("B", NULL, G);
    JGraph_AddVertex("C", NULL, G);
    JGraph_AddVertex("D", NULL, G);
    JGraph_AddVertex("E", NULL, G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);
    JGraph_SearchVertex("C", &C, G);
    JGraph_SearchVertex("D", &D, G);
    JGraph_SearchVertex("E", &E, G);
    
    JGraph_AddEdge(A, B, 1, G);
    JGraph_AddEdge(A, E, 1, G);
    JGraph_AddEdge(B, D, 1, G);
    JGraph_AddEdge(C, D, 1, G);
    JGraph_AddEdge(D, E, 1, G);
    JGraph_AddEdge(E, B, 1, G);
}

/*==============================================================================
 * TEST
 *============================================================================*/
void test_CheckInitialization(void)
{
    TEST_ASSERT_TRUE(1);
}

void test_AddingVertexWithDataToGraph(void)
{
    ELEM El;
    El.Key = 10;
    El.Data = 100;

    ReturnStatus = JGraph_AddVertex("Primo Vertice", &El, G);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
}

void test_AddVertexWithoutDataToGraph(void)
{
    ReturnStatus = JGraph_AddVertex("Primo Vertice", NULL, G);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);

}

void test_SearchVertexWithoutSavingDataAssociated(void)
{
    ReturnStatus = JGraph_AddVertex("Primo Vertice", NULL, G);
    ReturnStatus = JGraph_SearchVertex("Primo Vertice", NULL, G);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
}

void test_SearchVertexSavingItsData(void)
{
    ELEM El;
    ELEM *OutData;
    J_VERTEX *Out;
    El.Key = 10;
    El.Data = 100;

    ReturnStatus = JGraph_AddVertex("Primo Vertice", &El, G);
    ReturnStatus = JGraph_SearchVertex("Primo Vertice", &Out, G);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL_STRING("Primo Vertice", Out->Label);

    OutData = (ELEM *)JVertex_GetData(Out);
    TEST_ASSERT_EQUAL(10, OutData->Key);
    TEST_ASSERT_EQUAL(100, OutData->Data);


}

void test_addEdgeToVertexAdjList(void)
{
    J_VERTEX *A;
    J_VERTEX *B;
    J_VERTEX *C;
    J_VERTEX *D;
    J_VERTEX *E;
    J_EDGE E1;
    J_EDGE *E2;

    JGraph_AddVertex("A", NULL, G);
    JGraph_AddVertex("B", NULL, G);
    JGraph_AddVertex("C", NULL, G);
    JGraph_AddVertex("D", NULL, G);
    JGraph_AddVertex("E", NULL, G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);
    JGraph_SearchVertex("C", &C, G);
    JGraph_SearchVertex("D", &D, G);
    JGraph_SearchVertex("E", &E, G);
    
    JGraph_AddEdge(A, B, 2, G);
    JGraph_AddEdge(B, A, 2, G);
    JGraph_AddEdge(C, D, 2, G);
    ReturnStatus = JGraph_AddEdge(A, B, 2, G);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);

    E1.Destination = B;
    ReturnStatus = JList_Search(&E1, (void **)&E2, JVertex_GetAdjList(A));
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL_FLOAT(2, E2->Weight);
    TEST_ASSERT_EQUAL_MEMORY(B, E2->Destination, sizeof(J_VERTEX *));

}

void test_removeVertex(void)
{
    J_VERTEX *A;
    J_VERTEX *B;
    J_VERTEX *C;
    J_VERTEX *D;
    J_VERTEX *E;

    JGraph_AddVertex("A", NULL, G);
    JGraph_AddVertex("B", NULL, G);
    JGraph_AddVertex("C", NULL, G);
    JGraph_AddVertex("D", NULL, G);
    JGraph_AddVertex("E", NULL, G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);
    JGraph_SearchVertex("C", &C, G);
    JGraph_SearchVertex("D", &D, G);
    JGraph_SearchVertex("E", &E, G);
    
    JGraph_AddEdge(B, A, 2, G);
    JGraph_AddEdge(B, C, 2, G);
    JGraph_AddEdge(C, A, 2, G);
    JGraph_AddEdge(C, D, 2, G);
    JGraph_AddEdge(D, A, 2, G);
    JGraph_AddEdge(D, E, 2, G);
    JGraph_AddEdge(E, A, 2, G);
    JGraph_AddEdge(E, B, 2, G);

    ReturnStatus = JGraph_RemoveVertex(A, G);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(4, JGraph_GetNumVertices(G));

}

void test_removeEdge(void)
{
    J_VERTEX *A;
    J_VERTEX *B;

    JGraph_AddVertex("A", NULL, G);
    JGraph_AddVertex("B", NULL, G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);

    JGraph_AddEdge(B, A, 2, G);
    TEST_ASSERT_NOT_NULL(B->AdjList->Head);

    JGraph_RemoveEdge(B, A, G);
    TEST_ASSERT_NULL(B->AdjList->Head);

}

void test_TransposeGraph(void)
{
    J_GRAPH *Gt;
    J_VERTEX *A;
    J_VERTEX *B;
    J_VERTEX *C;
    J_VERTEX *D;
    J_VERTEX *E;

    CreaGrafo(G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);
    JGraph_SearchVertex("C", &C, G);
    JGraph_SearchVertex("D", &D, G);
    JGraph_SearchVertex("E", &E, G);
    
    ReturnStatus = JGraph_Transpose(G, &Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);

    ReturnStatus = JGraph_SearchVertex("A", &A, Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    ReturnStatus = JGraph_SearchVertex("B", &B, Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    ReturnStatus = JGraph_SearchVertex("C", &C, Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    ReturnStatus = JGraph_SearchVertex("D", &D, Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    ReturnStatus = JGraph_SearchVertex("E", &E, Gt);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    ReturnStatus = JGraph_SearchVertex("F", NULL, Gt);
    TEST_ASSERT_EQUAL(W_NOTFOUND, ReturnStatus);
    //JGraph_Print(Gt);

    JGraph_Destroy(Gt);

}

void test_PrintGraph(void)
{
    J_VERTEX *A;
    J_VERTEX *B;
    J_VERTEX *C;
    J_VERTEX *D;
    J_VERTEX *E;
    JGraph_AddVertex("A", NULL, G);
    JGraph_AddVertex("B", NULL, G);
    JGraph_AddVertex("C", NULL, G);
    JGraph_AddVertex("D", NULL, G);
    JGraph_AddVertex("E", NULL, G);

    JGraph_SearchVertex("A", &A, G);
    JGraph_SearchVertex("B", &B, G);
    JGraph_SearchVertex("C", &C, G);
    JGraph_SearchVertex("D", &D, G);
    JGraph_SearchVertex("E", &E, G);
    
    JGraph_AddEdge(A, B, 1, G);
    JGraph_AddEdge(A, E, 1, G);
    JGraph_AddEdge(B, D, 1, G);
    JGraph_AddEdge(C, D, 1, G);
    JGraph_AddEdge(D, E, 1, G);
    JGraph_AddEdge(E, B, 1, G);
    
    //JGraph_Print(G);
}

void test_Dfs(void)
{
    CreaGrafo(G);
    JGraph_Dfs(G);
}
