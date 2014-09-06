#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "jArray.h"
#include <stdio.h>
#include "jList.h"
#include "jEdgeSet.h"

J_EDGE_SET *Eset;
J_STATUS ReturnStatus;
J_VERTEX *V;

void setUp(void)
{
    ReturnStatus = JEdgeSet_New(5, &Eset);
}

void tearDown(void)
{
    JEdgeSet_Destroy(Eset);
}

void test_AddingVertexShouldSetVertexIdAndIncrementElemCount(void)
{
    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    TEST_ASSERT_EQUAL(-1, JVertex_GetId(V));
    ReturnStatus = JEdgeSet_AddVertex(V, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(0, JVertex_GetId(V));
    TEST_ASSERT_EQUAL(1, JEdgeSet_GetVertexCount(Eset));
    JVertex_Destroy( V );
}

void test_AddingMoreThanSizeVerticesShouldReallocateAdjList(void)
{
    J_VERTEX *V2, *V3, *V4, *V5, *V6;
    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V, Eset);
    V2 = JVertex_New("Vertice B", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V2, Eset);
    V3 = JVertex_New("Vertice C", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V3, Eset);
    V4 = JVertex_New("Vertice D", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V4, Eset);
    V5 = JVertex_New("Vertice E", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V5, Eset);

    TEST_ASSERT_EQUAL(4, JVertex_GetId(V5));
    TEST_ASSERT_EQUAL(5, JEdgeSet_GetVertexCount(Eset));

    V6 = JVertex_New("Vertice F", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V6, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(5, JVertex_GetId(V6));
    TEST_ASSERT_EQUAL(6, JEdgeSet_GetVertexCount(Eset));
    TEST_ASSERT_EQUAL(10, JEdgeSet_GetSize(Eset));
    
    JVertex_Destroy( V );
    JVertex_Destroy( V2 );
    JVertex_Destroy( V3 );
    JVertex_Destroy( V4 );
    JVertex_Destroy( V5 );
    JVertex_Destroy( V6 );
}

void test_RemoveVertexShouldPutIndexInFreeListAndDecrementVertexCount(void)
{
    J_VERTEX *V2, *V3, *V4, *V5;
    int *Id;
    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V, Eset);
    V2 = JVertex_New("Vertice B", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V2, Eset);
    V3 = JVertex_New("Vertice C", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V3, Eset);
    V4 = JVertex_New("Vertice D", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V4, Eset);
    V5 = JVertex_New("Vertice E", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V5, Eset);
    TEST_ASSERT_EQUAL(5, JEdgeSet_GetVertexCount(Eset));

    ReturnStatus = JEdgeSet_RemoveVertex(V3, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(4, JEdgeSet_GetVertexCount(Eset));
    TEST_ASSERT_FALSE(JList_isEmpty(Eset->FreeList));

    JList_HeadDelete((void **)&Id, Eset->FreeList);
    TEST_ASSERT_EQUAL(2, *Id);

    JVertex_Destroy( V );
    JVertex_Destroy( V2 );
    JVertex_Destroy( V3 );
    JVertex_Destroy( V4 );
    JVertex_Destroy( V5 );
    MemFree((void **)&Id);
}

void test_VertexInsertedAfterRemovalShouldHaveOldVertexId(void)
{
    J_VERTEX *V2, *V3, *V4, *V5;
    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    V2 = JVertex_New("Vertice B", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V2, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    V3 = JVertex_New("Vertice C", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V3, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    V4 = JVertex_New("Vertice D", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V4, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(4, JEdgeSet_GetVertexCount(Eset));


    ReturnStatus = JEdgeSet_RemoveVertex(V3, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(3, JEdgeSet_GetVertexCount(Eset));

    V5 = JVertex_New("Vertice E", NULL, NULL, NULL);
    ReturnStatus = JEdgeSet_AddVertex(V5, Eset);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(4, JEdgeSet_GetVertexCount(Eset));
    TEST_ASSERT_EQUAL(2, JVertex_GetId(V5));


    JVertex_Destroy( V );
    JVertex_Destroy( V2 );
    JVertex_Destroy( V3 );
    JVertex_Destroy( V4 );
    JVertex_Destroy( V5 );
}

void test_InsertEdgeWithAdjLists(void)
{

    J_VERTEX *V2, *V3, *V4;
    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    JEdgeSet_AddVertex(V, Eset);
    V2 = JVertex_New("Vertice B", NULL, NULL, NULL);
    JEdgeSet_AddVertex(V2, Eset);
    V3 = JVertex_New("Vertice C", NULL, NULL, NULL);
    JEdgeSet_AddVertex(V3, Eset);
    V4 = JVertex_New("Vertice D", NULL, NULL, NULL);
    JEdgeSet_AddVertex(V4, Eset);

    JEdgeSet_AddEdge(V2, V, 5, Eset);
    JEdgeSet_AddEdge(V3, V, 5, Eset);
    JEdgeSet_AddEdge(V3, V2, 20, Eset);
    JEdgeSet_AddEdge(V3, V4, 15, Eset);
    JEdgeSet_AddEdge(V4, V, 10, Eset);
    JEdgeSet_Print(Eset);

    JVertex_Destroy( V );
    JVertex_Destroy( V2 );
    JVertex_Destroy( V3 );
    JVertex_Destroy( V4 );
}
