#include "jMatrix.h"
#include "errors.h"
#include "jMem.h"
#include <stdlib.h>
#include <stdio.h>
#include <unity.h>
#include "common.h"
#include "jVertex.h"

#define ROWS 4
#define COLS 3

J_MATRIX *M;
J_STATUS ReturnStatus;

typedef struct j_edge_tag
{
	double Weight;         /**< Peso dell'arco */
	J_VERTEX *Destination; /**< Destinazione dell'arco */
} J_EDGE;

J_EDGE *InizializzaNodoEdge(J_EDGE *Value) {
	J_EDGE *E = NULL;
	MemAlloc(sizeof(J_EDGE), (void **) &E);

	*E = *Value;
	return E;
}

void StampaNodoEdge(J_EDGE *Edge) {
	printf("\nDestinazione arco: %s\n", Edge->Destination->Label);
	printf("Peso arco %f\n", Edge->Weight);
}

void DeallocaEdge(J_EDGE *Edge) {
	MemFree((void **) &Edge);
}

void setUp(void)
{
    ReturnStatus = JMatrix_New(ROWS, COLS, (INITIALIZER)InizializzaNodoEdge, (DELETER)DeallocaEdge, &M);
}

void tearDown(void)
{
    JMatrix_Destroy( M );
}

void test_Initialization(void)
{
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(ROWS, M->MaxRows);
    TEST_ASSERT_EQUAL(COLS, M->MaxCols);
    TEST_ASSERT_EQUAL(0, M->Cols);
    TEST_ASSERT_EQUAL(0, M->Rows);
}


void test_SetElementsInMatrixShouldGetThemInTheRightIndices(void)
{
    J_VERTEX *V;
    J_EDGE *E;
    int i,j;

    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    MemAlloc(sizeof(J_EDGE), (void **)&E);

    E->Destination = V;
    E->Weight = 123;
    //Aggiungere righe e colonne
    JMatrix_AddRowCol(M);
    JMatrix_AddRowCol(M);
    JMatrix_AddRowCol(M);

    // Inserisci lo stesso elemento in tutte le locazioni
    for(i = 0; i < JMatrix_GetRows(M); i++)
    {
        for(j = 0; j < JMatrix_GetCols(M); j++)
        {
            JMatrix_Set((void *)E, i, j, M);
        }
    }
    // Verifica che in tutte le locazioni sia correttamente inserito l'elemento
    for(i = 0; i < JMatrix_GetRows(M); i++)
    {
        for(j = 0; j < JMatrix_GetCols(M); j++)
        {
            TEST_ASSERT_EQUAL_MEMORY(E->Destination, ((J_EDGE *)(M->Matrix[i][j]))->Destination, sizeof(J_VERTEX *));
            TEST_ASSERT_EQUAL(E->Weight,((J_EDGE *)(M->Matrix[i][j]))->Weight);
        }
    }
    JVertex_Destroy(V);
    MemFree((void **)&E);
}

void test_SetElementOutsideBoundaryShouldReturnWarning(void)
{
    J_VERTEX *V;
    J_EDGE *E;

    V = JVertex_New("Vertice A", NULL, NULL, NULL);
    MemAlloc(sizeof(J_EDGE), (void **)&E);

    TEST_ASSERT_EQUAL( W_OUTOFBOUND, JMatrix_Set((void *)E, 2, 3, M) );

    JVertex_Destroy(V);
    MemFree((void **)&E);

}

void test_AddingRowsReallocateMatrix(void)
{
    TEST_ASSERT_EQUAL( 0, JMatrix_GetRows(M) );
    JMatrix_AddRow(M);
    JMatrix_AddRow(M);
    JMatrix_AddRow(M);
    TEST_ASSERT_EQUAL( 3, JMatrix_GetRows(M) );
    JMatrix_AddRow(M);
    ReturnStatus = JMatrix_AddRow(M);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL( 5, JMatrix_GetRows(M) );
    TEST_ASSERT_EQUAL( 14, M->MaxRows ); /* 4 + DEFAULT */
}

void test_AddingColsReallocateMatrix(void)
{
    TEST_ASSERT_EQUAL( 0, JMatrix_GetCols(M) );
    JMatrix_AddColumn(M);
    JMatrix_AddColumn(M);
    JMatrix_AddColumn(M);
    JMatrix_AddColumn(M);
    ReturnStatus = JMatrix_AddColumn(M);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(13, M->MaxCols); /* 3 + DEFAULT */
}

void test_SetDefaultValuesShouldInitializeTheMatrixWithTheSameValue(void)
{
    J_EDGE *E, *Out;

    JMatrix_AddRowCol(M);
    JMatrix_AddRowCol(M);
    JMatrix_AddRowCol(M);
    JMatrix_AddRowCol(M);

    MemAlloc(sizeof(J_EDGE), (void **)&E);

    E->Destination = NULL;
    E->Weight = 123;

    JMatrix_SetDefaultValues(E, M);
    JMatrix_Get(&Out, 1, 1, M);
    TEST_ASSERT_EQUAL(123, Out->Weight);

    MemFree((void **)&E);

}


