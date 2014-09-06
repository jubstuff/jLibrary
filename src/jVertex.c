#include "jVertex.h"
#include <stdlib.h>
#include <stdio.h>
#include "jMem.h"
#include <string.h>

J_VERTEX *JVertex_New( char *Label, void *Data, INITIALIZER Init, DELETER Del )
{
	J_VERTEX *Out = NULL;
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;

	ReturnStatus = MemAlloc(sizeof(J_VERTEX), (void **)&Out);
	if( ReturnStatus == SUCCESS )
	{
		Out->Label = strdup(Label);
		Out->Data = NULL;
		Out->Delete = Del;
		Out->Id = -1;
		if( Init && Data )
		{
			Out->Data = Init(Data);
		}

	}
	return Out;
}

void JVertex_Destroy( J_VERTEX *V )
{
    if( V != NULL )
    {
        /* Se il vertice esiste, dealloca tutte le info ad esso associate */
        MemFree( (void **)&V->Label );

        if( V->Delete ) {
        	V->Delete(V->Data);
        }


        MemFree( (void **)&V );
    }
}

/**
 * Confronta due vertici per ID
 *
 * */
int JVertex_Compare( J_VERTEX *Vertex1, J_VERTEX *Vertex2 )
{
	return strcmp(Vertex1->Label, Vertex2->Label);
}

unsigned int HashVertex(J_VERTEX *V)
{
    unsigned int Id;
    char *Label = V->Label;

    for(Id = 0; *Label != '\0'; Label++)
    {
        Id = *Label + 31 * Id;
    }
    return Id;
}

void JVertex_Print( J_VERTEX *V )
{
	printf("Etichetta: %s\n", V->Label);
}

char *JVertex_GetLabel( J_VERTEX *V )
{
	return V->Label;
}

void *JVertex_GetData( J_VERTEX *V )
{
	return V->Data;
}

void JVertex_SetId( int Id, J_VERTEX *V )
{
	V->Id = Id;
}
int JVertex_GetId( J_VERTEX *V )
{
	return V->Id;
}
