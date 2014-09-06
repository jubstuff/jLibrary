#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include "jList.h"
#include <stdlib.h>

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

void GetElemento(ELEM *InValue, ELEM **Out)
{
    printf("GETTER\n");
    *Out = InValue;
}





static JLIST_METHODS Op;
static J_STATUS ReturnStatus;
static ELEM El;
J_LIST *List;

void setUp(void)
{

	/* Inizializza la struct con le operazioni */
   	Op.Compare = (COMPARATOR)ConfrontaElementi;
   	Op.InitNode = (INITIALIZER)InizializzaElemento;
	Op.Delete = (DELETER)CancellaElemento;
   	Op.Print = (PRINTER)StampaElemento;
    Op.ManageDuplicate = NULL; 
    Op.GetNodeValue = (GETTER)GetElemento;


    JList_Init(&List, &Op, 1);

}

void tearDown(void)
{
    JList_Destroy( List );
}

/**
 * TEST
 * */
void test_ListOrderedInsertInsertsInOrder(void)
{
    TEST_ASSERT_TRUE(JList_isEmpty(List));

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_OrderedInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_FALSE(JList_isEmpty(List));
    TEST_ASSERT_NULL(List->Head->Next);

    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 100, ((ELEM *)(List->Head->Info))->Data ) ;

    El.Key = 0;
    El.Data = 200;
    ReturnStatus = JList_OrderedInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_NOT_NULL(List->Head->Next);
    TEST_ASSERT_NULL(List->Head->Next->Next);
    TEST_ASSERT_EQUAL( 0, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 200, ((ELEM *)(List->Head->Info))->Data ) ;
    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 100, ((ELEM *)(List->Head->Next->Info))->Data ) ;

    El.Key = 2;
    El.Data = 300;
    ReturnStatus = JList_OrderedInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL( 2, ((ELEM *)(List->Head->Next->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 300, ((ELEM *)(List->Head->Next->Next->Info))->Data ) ;
}

void test_ListWarnThatThereIsADuplicate(void)
{
    TEST_ASSERT_TRUE(JList_isEmpty(List));

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_OrderedInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_FALSE(JList_isEmpty(List));

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_OrderedInsert(&El, List);

    TEST_ASSERT_EQUAL(W_LIST_DUP, ReturnStatus);
}

void test_HeadInsertInsertsNodesInHead(void)
{
    TEST_ASSERT_TRUE(JList_isEmpty(List));

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_FALSE(JList_isEmpty(List));
    TEST_ASSERT_NULL(List->Head->Next);

    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 100, ((ELEM *)(List->Head->Info))->Data ) ;

    El.Key = 0;
    El.Data = 200;
    ReturnStatus = JList_HeadInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_NOT_NULL(List->Head->Next);
    TEST_ASSERT_NULL(List->Head->Next->Next);
    TEST_ASSERT_EQUAL( 0, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 200, ((ELEM *)(List->Head->Info))->Data ) ;
    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 100, ((ELEM *)(List->Head->Next->Info))->Data ) ;

    El.Key = 2;
    El.Data = 300;
    ReturnStatus = JList_HeadInsert(&El, List);

    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL( 2, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 300, ((ELEM *)(List->Head->Info))->Data ) ;
    TEST_ASSERT_EQUAL( 0, ((ELEM *)(List->Head->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 200, ((ELEM *)(List->Head->Next->Info))->Data ) ;

}

void test_SearchFindsNodeInListAndReturnsPointer(void)
{
    ELEM *El2;

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 6;
    El.Data = 600;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 2;
    El.Data = 200;
    ReturnStatus = JList_HeadInsert(&El, List);

    El.Key = 6;
    ReturnStatus = JList_Search( &El, (void **)&El2, List );
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(6, El2->Key);
    TEST_ASSERT_EQUAL(600, El2->Data);

    El.Key = 1;
    ReturnStatus = JList_Search( &El, (void **)&El2, List );
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(1, El2->Key);
    TEST_ASSERT_EQUAL(100, El2->Data);


}

void test_SearchDoesntFindNodeInEmptyList(void)
{
    ELEM *El2;

    El.Key = 23;
    ReturnStatus = JList_Search( &El, (void **)&El2, List );
    TEST_ASSERT_EQUAL(W_LIST_NOTFOUND, ReturnStatus);
    TEST_ASSERT_NULL(El2);
}

void test_SearchDoesntFindNodeNotPresentInList(void)
{
    ELEM *El2;

    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 6;
    El.Data = 600;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 2;
    El.Data = 200;
    ReturnStatus = JList_HeadInsert(&El, List);

    El.Key = 23;
    ReturnStatus = JList_Search( &El, (void **)&El2, List );
    TEST_ASSERT_EQUAL(W_LIST_NOTFOUND, ReturnStatus);
    TEST_ASSERT_NULL(El2);
}

void test_SearchWithoutSavingInfo(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 6;
    El.Data = 600;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 2;
    El.Data = 200;
    ReturnStatus = JList_HeadInsert(&El, List);

    ReturnStatus = JList_Search( &El, NULL, List );
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);

    El.Key = 123;
    ReturnStatus = JList_Search( &El, NULL, List );
    TEST_ASSERT_EQUAL(W_LIST_NOTFOUND, ReturnStatus);

}

void test_DeleteNodeDeleteNodeInHead(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);

    ReturnStatus = JList_DeleteNode(&El, List);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_TRUE(JList_isEmpty(List));

}

void test_DeleteNonExistentNodeWarnsNodeNotPresent(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);

    El.Key = 20;
    ReturnStatus = JList_DeleteNode(&El, List);
    TEST_ASSERT_EQUAL(W_LIST_NOTFOUND, ReturnStatus);
    TEST_ASSERT_FALSE(JList_isEmpty(List));
}

void test_DeleteRestoreCorrectLinkBetweenNodes(void)
{
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 6;
    El.Data = 600;
    ReturnStatus = JList_HeadInsert(&El, List);

    El.Key = 4;
    ReturnStatus = JList_DeleteNode(&El, List);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_FALSE(JList_isEmpty(List));
    TEST_ASSERT_EQUAL( 6, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 600, ((ELEM *)(List->Head->Info))->Data ) ;
    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 100, ((ELEM *)(List->Head->Next->Info))->Data ) ;
    TEST_ASSERT_NULL(List->Head->Next->Next);

}

void test_HeadDeleteDeletesNodeInHead(void)
{
    ELEM *OutElem;


    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 6;
    El.Data = 600;
    ReturnStatus = JList_HeadInsert(&El, List);

    TEST_ASSERT_EQUAL( 6, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 600, ((ELEM *)(List->Head->Info))->Data ) ;
    ReturnStatus = JList_HeadDelete((void **)&OutElem, List);
    TEST_ASSERT_EQUAL(SUCCESS, ReturnStatus);
    TEST_ASSERT_EQUAL(6, OutElem->Key);
    TEST_ASSERT_EQUAL(600, OutElem->Data);
    MemFree((void **)&OutElem);
}

//void test_DeleteRange(void)
//{
//    TEST_IGNORE();
//}

void addTwoToElem(ELEM **El, int *Input)
{
    if( (*El)->Data == *Input )
    {
        printf("Trovato uguale!\n");
    }

    (*El)->Data += 2;
}

void test_jListMap(void)
{
    int num = 100;
    El.Key = 1;
    El.Data = 100;
    ReturnStatus = JList_HeadInsert(&El, List);
    El.Key = 4;
    El.Data = 400;
    ReturnStatus = JList_HeadInsert(&El, List);

    JList_Map(addTwoToElem, &num, List);

    TEST_ASSERT_EQUAL( 4, ((ELEM *)(List->Head->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 402, ((ELEM *)(List->Head->Info))->Data ) ;
    TEST_ASSERT_EQUAL( 1, ((ELEM *)(List->Head->Next->Info))->Key ) ;
    TEST_ASSERT_EQUAL( 102, ((ELEM *)(List->Head->Next->Info))->Data ) ;
}
