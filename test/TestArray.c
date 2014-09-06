#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "jArray.h"
#include <stdio.h>

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

void PrintElemento( ELEM **El, int i, void *Input)
{
    printf("\nChiave: %d\n", (*El)->Key);
    printf("Valore: %d\n", (*El)->Data);
    printf("Indice: %d\n", i);
}

static ELEM El;
J_STATUS ReturnStatus;
J_ARRAY *A;

void setUp(void)
{
	/* Inizializza la struct con le operazioni */
    ReturnStatus = JArray_New(5, (INITIALIZER)InizializzaElemento, (DELETER)CancellaElemento, &A);
}

void tearDown(void)
{
    JArray_Destroy(A);
}

void test_InitArrayWithGoodInitAndDeleteFunctions(void)
{
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(5, JArray_GetSize(A));
    TEST_ASSERT_EQUAL(0, JArray_GetElemCount(A));
}

void test_InitArrayWithoutInitAndDeleteFunctionsShouldReturnError(void)
{
    J_ARRAY *A2;
    ReturnStatus = JArray_New(5, NULL, NULL, &A2);
    TEST_ASSERT_EQUAL(ERROR, ReturnStatus);
}
void test_GetArrayElementInPosition0AfterInitializationShouldReturnNull(void)
{
    ELEM *OutElem;
    ReturnStatus = JArray_Get(0, (void **)&OutElem, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_NULL(OutElem);
}
void test_GetArrayElementOutOfBoundShouldReturnWarning(void)
{
    ELEM *OutElem;
    ReturnStatus = JArray_Get(234, (void **)&OutElem, A);
    TEST_ASSERT_EQUAL(W_OUTOFBOUND, ReturnStatus);
    TEST_ASSERT_NULL(OutElem);
}

void test_SetElemInPosition0ShouldSaveElementInArrayAndIncrementElemCount(void)
{
    ELEM *OutElem;
    El.Key = 1;
    El.Data = 100;

    ReturnStatus = JArray_Set(&El, 0, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(1, JArray_GetElemCount(A));
    ReturnStatus = JArray_Get(0, (void **)&OutElem, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_NOT_NULL(OutElem);
    TEST_ASSERT_EQUAL(1, OutElem->Key);
    TEST_ASSERT_EQUAL(100, OutElem->Data);

}

void test_SetElemOutOfBoundShouldReturnWarningAndLeaveElemCountZero(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JArray_Set(&El, 123, A);
    TEST_ASSERT_EQUAL(W_OUTOFBOUND, ReturnStatus);
    TEST_ASSERT_EQUAL(0, JArray_GetElemCount(A));
}

void test_SetElemWithNegativeIndexShouldReturnWarningAndLeaveElemCountZero(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JArray_Set(&El, -1, A);
    TEST_ASSERT_EQUAL(W_OUTOFBOUND, ReturnStatus);
    TEST_ASSERT_EQUAL(0, JArray_GetElemCount(A));
}

void test_SetElemInPositionAlreadyOccupiedFreeOldElem(void)
{
    ELEM *OutElem;
    El.Key = 1;
    El.Data = 100;
    JArray_Set(&El, 0, A);
    El.Key = 2;
    El.Data = 200;
    JArray_Set(&El, 0, A);
    ReturnStatus = JArray_Get(0, (void **)&OutElem, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_NOT_NULL(OutElem);
    TEST_ASSERT_EQUAL(2, OutElem->Key);
    TEST_ASSERT_EQUAL(200, OutElem->Data);
}

void test_DeleteItemInPositionSetThatItemToNull(void)
{
    ELEM *OutElem;
    El.Key = 1;
    El.Data = 100;
    JArray_Set(&El, 0, A);
    TEST_ASSERT_EQUAL(1, JArray_GetElemCount(A));
    ReturnStatus = JArray_Delete(0, A);
    ReturnStatus = JArray_Get(0, (void **)&OutElem, A);
    TEST_ASSERT_EQUAL(NULL, OutElem);
    TEST_ASSERT_EQUAL(0, JArray_GetElemCount(A));
}

void test_DeleteItemOutOfArrayBoundShouldReturnWarning(void)
{
    ReturnStatus = JArray_Delete(123, A);
    TEST_ASSERT_EQUAL(W_OUTOFBOUND, ReturnStatus);
}

void test_ResizeArrayToSmallerSizeShouldDeallocateUnusedElements(void)
{
    int i;
    for(i=1; i<=5; i++)
    {
        El.Key = i;
        El.Data = i * 100;
        JArray_Set(&El, i-1, A);
    }
    JArray_Delete(1, A);
    ReturnStatus = JArray_Resize(2, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(1, JArray_GetElemCount(A));
    TEST_ASSERT_EQUAL(2, JArray_GetSize(A));
}

void test_ResizeArrayToBiggerSizeShouldAllocateNewNullPointersAndIncrementArraySize(void)
{
    int i;
    ELEM *OutElem;

    for(i=1; i<=5; i++)
    {
        El.Key = i;
        El.Data = i * 100;
        JArray_Set(&El, i-1, A);
    }

    ReturnStatus = JArray_Resize(10, A);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(10, JArray_GetSize(A));
    TEST_ASSERT_EQUAL(5, JArray_GetElemCount(A));
    for(i=1; i<=5; i++)
    {
        JArray_Get(i-1, (void **)&OutElem, A);
        TEST_ASSERT_EQUAL(i, OutElem->Key);
        TEST_ASSERT_EQUAL(i*100, OutElem->Data);
    }
    for(i=5; i<10; i++)
    {
        JArray_Get(i, (void **)&OutElem, A);
        TEST_ASSERT_NULL(OutElem);
    }

}

void test_ResizeWithNegativeNewSizeShouldReturnError(void)
{
    ReturnStatus = JArray_Resize(-4, A);
    TEST_ASSERT_EQUAL(ERROR, ReturnStatus);
}

void test_MapElementsWithPrintFunctionPrintElements(void)
{
    int i;
    for(i=1; i<=5; i++)
    {
        El.Key = i;
        El.Data = i * 100;
        JArray_Set(&El, i-1, A);
    }
    JArray_Map(PrintElemento, NULL, A);
}
