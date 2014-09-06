/*
 * =====================================================================================
 *
 *       Filename:  jMatrix.c
 *
 *    Description:  Implementazione di una matrice dinamica
 *
 *        Version:  1.0
 *        Created:  08/07/2012 19:33:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jMatrix.h"
#include "jMem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* Per memmove, memset */

#define DEFAULT 10

/*-----------------------------------------------------------------------------
 *  DEFINIZIONE METODI PRIVATI
 *-----------------------------------------------------------------------------*/
static int IsIndexInMatrix(int Row, int Col, J_MATRIX *M);
/**
 * Alloca una quantità fissata di righe per la matrice
 */
static J_STATUS JMatrix_AllocateRows(J_MATRIX *M);
/**
 * Alloca una quantità fissata di colonne per la matrice
 */
static J_STATUS JMatrix_AllocateColumns(J_MATRIX *M);


/*-----------------------------------------------------------------------------
 *  IMPLEMENTAZIONE METODI PUBBLICI
 *-----------------------------------------------------------------------------*/
J_STATUS JMatrix_New(int StartRows, int StartCols, INITIALIZER Init, DELETER Delete, J_MATRIX **M)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	int i, j;              /* Indici per cicli */
	ReturnStatus = SUCCESS;
	if(Init && Delete)
	{
		/* Sono state passate le funzioni di inizializzazione e deallocazione */
		/* Alloco la matrice */
		ReturnStatus = MemAlloc(sizeof(J_MATRIX), (void **)M);
		if(ReturnStatus == SUCCESS)
		{
			/* Se il numero di righe passato in input è negativo, imposto un default */
			(*M)->MaxRows = StartRows > 0 ? StartRows : DEFAULT;
			/* Struttura allocata correttamente, alloca puntatori alle righe */
			ReturnStatus = MemAlloc((*M)->MaxRows * sizeof(void **), (void **)&(*M)->Matrix);
			if(ReturnStatus == SUCCESS)
			{
				/* Puntatori alle righe allocati correttamente */

				/* Se il numero di colonne passato in input è negativo, imposto un default */
				(*M)->MaxCols = StartCols > 0 ? StartCols : DEFAULT;
				/* Alloca righe */
				for(i = 0; i < (*M)->MaxRows && (ReturnStatus==SUCCESS); i++)
				{
					ReturnStatus = MemCalloc((*M)->MaxCols, sizeof(void *), (void**)&((*M)->Matrix[i]));
				}
				/* Inizializza i campi della struttura */
				(*M)->InitData = Init;
				(*M)->DeleteData = Delete;
				(*M)->Rows = 0;
				(*M)->Cols = 0;

				if(ReturnStatus != SUCCESS)
				{
					/* Se l'allocazione delle righe non è andata a buon termine,
					 * dealloca quelle esistenti
					 */
					for(j = 0; j < i; j++)
					{
						MemFree((void**)&((*M)->Matrix[j]));
					}
					MemFree((void **)&(*M)->Matrix);
					MemFree((void **)&M);
				}
			}
			else
			{
				MemFree((void **)&M);
			}
		}
	}
	else
	{
		/* Mancano le funzioni di inizializzazione e deallocazione */
		ReturnStatus = ERROR;
	}

	return ReturnStatus;
}

void JMatrix_Destroy( J_MATRIX *M )
{
    int i,j; /* Indici per cicli */

    if( M )
    {
    	/* Dealloca gli elementi esistenti */
    	for(i = 0; i < M->MaxRows; i++)
    	{
    		for(j = 0; j < M->MaxCols; j++)
    		{
    			if(M->Matrix[i][j])
    			{
    				M->DeleteData(M->Matrix[i][j]);
    			}
    		}
    	}


    	/* Dealloca le righe */
        for(i = 0; i < M->MaxRows; i++)
        {
            MemFree((void **)&(M->Matrix[i]));
        }
        /* Dealloca il vettore di puntatori alle righe */
        MemFree((void **)&M->Matrix);
        /* Dealloca la matrice */
        MemFree((void **)&M);
    }
}

J_STATUS JMatrix_Set(void *Data, int Row, int Col, J_MATRIX *M)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */

	ReturnStatus = SUCCESS;
	if(IsIndexInMatrix(Row, Col, M))
	{
		if(M->Matrix[Row][Col])
		{
			/* Se c'è già un elemento, cancellalo */
			M->DeleteData(M->Matrix[Row][Col]);
		}
		/* Imposta il valore alla posizione Row,Col */
		M->Matrix[Row][Col] = M->InitData(Data);
	}
	else
	{
		ReturnStatus = W_OUTOFBOUND;
	}
	return ReturnStatus;
}

J_STATUS JMatrix_Get(void **Data, int Row, int Col, J_MATRIX *M)
{
	J_STATUS ReturnStatus; /* Esito dell'operazione */
	if(IsIndexInMatrix(Row, Col, M))
	{
		*Data = M->Matrix[Row][Col];
	}
	else
	{
		ReturnStatus = W_OUTOFBOUND;
	}

	ReturnStatus = SUCCESS;
	return ReturnStatus;
}

void JMatrix_AddRowCol(J_MATRIX *M)
{
	JMatrix_AddRow(M);
	JMatrix_AddColumn(M);
}

int JMatrix_GetRows(J_MATRIX *M)
{
	return M->Rows;
}

int JMatrix_GetCols(J_MATRIX *M)
{
	return M->Cols;
}

J_STATUS JMatrix_AddRow(J_MATRIX *M)
{
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;
	if(M->Rows == M->MaxRows)
	{
		/* Alloca altre righe */
		ReturnStatus = JMatrix_AllocateRows(M);
	}
	if(ReturnStatus == SUCCESS)
	{
		M->Rows++;
	}

	return ReturnStatus;
}

J_STATUS JMatrix_AddColumn(J_MATRIX *M)
{
	J_STATUS ReturnStatus;
	int i;

	ReturnStatus = SUCCESS;
	if(M->Cols == M->MaxCols)
	{
		ReturnStatus = JMatrix_AllocateColumns(M);
	}
	if(ReturnStatus == SUCCESS)
	{
		for(i = 0; i < M->MaxRows; i++)
		{
			/* Inizializza la nuova colonna */
			M->Matrix[i][M->Cols] = NULL;
		}
		M->Cols++;
	}

	return ReturnStatus;
}

void JMatrix_SetDefaultValues(void *DefaultValue, J_MATRIX *M)
{
    int i;
    int j;

    i = 0;
    j = 0;

    for( i = 0; i < M->Rows; i++ )
    {
        for( j = 0; j < M->Cols; j++ )
        {
            JMatrix_Set(DefaultValue, i, j, M);
        }
    }

}

/*-----------------------------------------------------------------------------
 *  IMPLEMENTAZIONE METODI PRIVATI
 *-----------------------------------------------------------------------------*/

/**
 * Verifica se gli indici (Row,Col) sono entro i limiti della matrice
 */
static int IsIndexInMatrix(int Row, int Col, J_MATRIX *M)
{
	return (Row >= 0) && (Row < M->Rows) && (Col >= 0) && (Col < M->Cols);
}
/**
 * Alloca un numero predefinito di righe aggiuntive
 */
static J_STATUS JMatrix_AllocateRows(J_MATRIX *M)
{
	J_STATUS ReturnStatus;
	int i;
	/* Aumento il numero di righe disponibili */
	M->MaxRows += DEFAULT;
	/* Realloco tutte le righe */
	ReturnStatus = MemRealloc(M->MaxRows * sizeof(void **), (void **)&M->Matrix );
	/* Inizializzo le nuove colonne */
	for(i = M->Rows; i < M->MaxRows && (ReturnStatus==SUCCESS); i++)
	{
		ReturnStatus = MemCalloc(M->MaxCols, sizeof(void *), (void**)&(M->Matrix[i]));
	}

	return ReturnStatus;
}
/**
 * Alloca un numero predefinito di colonne aggiuntive
 */
static J_STATUS JMatrix_AllocateColumns(J_MATRIX *M)
{
	J_STATUS ReturnStatus;
	int i;

	ReturnStatus = SUCCESS;

	/* Aggiungo altre colonne */
	M->MaxCols += DEFAULT;
	for(i = 0; i < M->MaxRows && (ReturnStatus == SUCCESS); i++)
	{
		ReturnStatus = MemRealloc(M->MaxCols * sizeof(void *), (void **)&(M->Matrix[i]));
	}

	return ReturnStatus;
}
