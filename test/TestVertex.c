#include "jMem.h"
#include "unity.h"
#include "errors.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include "jVertex.h"

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

J_VERTEX *V;
 
void setUp(void)
{
   	V = JVertex_New("Vertice A", NULL, NULL, NULL);
}

void tearDown(void)
{
    JVertex_Destroy( V );
}

/*==============================================================================
 * TEST
 *============================================================================*/
void test_CheckInitialization(void)
{
    TEST_ASSERT_EQUAL_STRING("Vertice A", JVertex_GetLabel(V));
    TEST_ASSERT_NULL(JVertex_GetData(V));
}

void test_CheckHashingLabel(void)
{
    unsigned int Id;

    Id = HashVertex(V);

    TEST_ASSERT_EQUAL(625126715, Id);
}

void test_VertexWithElem(void)
{
    J_VERTEX *V2;
    ELEM El;

    El.Key = 10;
    El.Data = 100;
    V2 = JVertex_New("Vertice B", &El, (INITIALIZER)InizializzaElemento, (DELETER)CancellaElemento);
    

    TEST_ASSERT_EQUAL_STRING("Vertice B", JVertex_GetLabel(V2));
    TEST_ASSERT_EQUAL_MEMORY(&El, JVertex_GetData(V2), sizeof(ELEM));

    JVertex_Destroy( V2 );
    
}

void test_VertexWithoutElem(void)
{
    J_VERTEX *V2;

    V2 = JVertex_New("Vertice B", NULL, NULL, (DELETER)CancellaElemento);
    TEST_ASSERT_NULL(JVertex_GetData(V2));
    JVertex_Destroy( V2 );
}

