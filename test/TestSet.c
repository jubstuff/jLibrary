#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include "jList.h"
#include <stdlib.h>
#include "jSet.h"

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

int ConfrontaElementi( const ELEM *E1, const ELEM *E2 )
{
	int ReturnValue;

	if( E1->Key < E2->Key )
	{
		ReturnValue = -1;
	}
	else if( E1->Key == E2->Key )
	{
		ReturnValue = 0;
	}
	else
	{
		ReturnValue = 1;
	}

	return ReturnValue;
}

void StampaElemento( const ELEM *El )
{
	printf("\nKey: %d\n", El->Key);
	printf("Data: %d\n", El->Data);
}

int HashElem(ELEM *El)
{
    return El->Key * 10 - 20;
}


static JSET_METHODS Op;
static J_STATUS ReturnStatus;
static ELEM El;
static int Dim = 343;
J_SET *Set;

void setUp(void)
{
   	Op.Compare = (COMPARATOR)ConfrontaElementi;
   	Op.Initialize = (INITIALIZER)InizializzaElemento;
	Op.Delete = (DELETER)CancellaElemento;
   	Op.Print = (PRINTER)StampaElemento;
   	Op.Hash = (HASH)HashElem;

   	JSet_New( Dim , &Op, &Set);
}

void tearDown(void)
{
    JSet_Destroy( Set );
}

/*==============================================================================
 * TEST
 *============================================================================*/
void test_CheckInitialization(void)
{
    TEST_ASSERT_EQUAL(0, Set->ElemCount);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(Dim, Set->Size);
}

void test_AddElementToSet(void)
{
    int HashKey;

    El.Key = 20;
    El.Data = 200;
    ReturnStatus = JSet_Add(&El, Set);

    HashKey = HashElem(&El) % Dim;

    TEST_ASSERT_EQUAL(SUCCESS, JList_Search(&El, NULL, Set->Buckets[HashKey]));
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(1, Set->ElemCount);

    El.Key = 45;
    El.Data = 200;
    ReturnStatus = JSet_Add(&El, Set);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(2, Set->ElemCount);

}

void test_DeleteElement(void)
{
    int ElemCount;
    int HashKey;

    El.Key = 20;
    El.Data = 200;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 462;
    El.Data = 67963;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 5682;
    El.Data = 294846;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 12353;
    El.Data = 293048;
    ReturnStatus = JSet_Add(&El, Set);
    ElemCount = JSet_GetNumElem(Set);
    HashKey = HashElem(&El) % Dim;

    ReturnStatus = JSet_Delete(&El, Set);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(ElemCount-1, JSet_GetNumElem(Set));
    TEST_ASSERT_EQUAL(W_LIST_NOTFOUND, JList_Search(&El, NULL, Set->Buckets[HashKey]));
}

void test_SearchElementWithoutSavingInfo(void)
{
    El.Key = 20;
    El.Data = 200;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 462;
    El.Data = 67963;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 5682;
    El.Data = 294846;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 12353;
    El.Data = 293048;
    ReturnStatus = JSet_Add(&El, Set);

    ReturnStatus = JSet_Search(&El, NULL, Set);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);

    El.Key = 1;
    ReturnStatus = JSet_Search(&El, NULL, Set);
    TEST_ASSERT_EQUAL(W_NOTFOUND, ReturnStatus);
}

void test_SearchElementSavingInfo(void)
{
    ELEM *El2;

    El.Key = 20;
    El.Data = 200;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 462;
    El.Data = 67963;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 5682;
    El.Data = 294846;
    ReturnStatus = JSet_Add(&El, Set);
    El.Key = 12353;
    El.Data = 293048;
    ReturnStatus = JSet_Add(&El, Set);

    ReturnStatus = JSet_Search(&El, &El2, Set);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(293048, El2->Data);
}
