/** 
 * mem.c
 *
 * implementation of the mem interface
 *
 * */

#include <stdlib.h>
#include <stddef.h>
#include "jMem.h"

J_STATUS MemAlloc( size_t Size, void **OutPtr)
{
	void *Ptr;
	J_STATUS Status;

	Status = SUCCESS;

	Ptr = malloc(Size);
	if( Ptr != NULL )
	{
		*OutPtr = Ptr;
	}
	else
	{
		Status = E_NO_MEM;
		*OutPtr = NULL;
	}
	return Status;

}

J_STATUS MemCalloc( size_t Count, size_t Size, void **OutPtr)
{
	void *Ptr;
	J_STATUS Status;

	Status = SUCCESS;

	Ptr = calloc(Count, Size);
	if( Ptr != NULL )
	{
		*OutPtr = Ptr;
	}
	else
	{
		Status = ERROR;
		*OutPtr = NULL;
	}
	return Status;

}

J_STATUS MemFree( void **Ptr )
{
	J_STATUS Status;

	Status = SUCCESS;

	if( *Ptr )
	{
		free(*Ptr);
	}
	else
	{
		Status = ERROR;
	}
	*Ptr = NULL;
	return Status;
}

J_STATUS MemRealloc( long Nbytes, void **Ptr )
{
    J_STATUS Status;
    void *TempPtr;

    Status = SUCCESS;

    TempPtr = realloc( *Ptr, Nbytes );
    if( TempPtr != NULL )
	{
		*Ptr = TempPtr;
	}
	else
	{
		Status = ERROR;
		*Ptr = NULL;
	}

	return Status;
}
