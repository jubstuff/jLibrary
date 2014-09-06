/*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.

* Nome File : jIterator.c

* Scopo : Implementazione dei metodi per il modulo Iterator

* Data di creazione : 13 Gennaio 2012

* Autore : Giustino Borzacchiello - giustinob@gmail.com

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#include "jIterator.h"
#include "jMem.h"
#include "errors.h"

/*
 * Definizione struttura
 */
struct j_iterator_tag
{
    void *Data;   /* Oggetto dell'iteratore */
    GET_FN Get;    /* Funzione per recuperare il prossimo elemento */
    DEL_FN Delete; /* Funzione per deallocare l'oggetto dell'iteratore */
};

/**
 * Crea un nuovo iteratore
 *
 * Questa funzione crea un nuovo iteratore per l'oggetto Data.
 *
 * @param Data Oggetto su cui opera l'iteratore
 * @param Get  Puntatore ad una funzione per recuperare il prossimo elemento
 * @param Del  Puntatore ad una funzione per deallocare l'oggetto Data
 * @param It   Indirizzo del puntatore in cui salvare l'iteratore
 *
 * @return Esito dell'operazione
 * 			 - SUCCESS, se OK
 * 			 - E_NO_MEM, se l'allocazione è fallita
 */
J_STATUS JIterator_New(void *Data, GET_FN Get, DEL_FN Delete, J_ITERATOR **It)
{
    J_STATUS ReturnStatus;

    ReturnStatus = SUCCESS;

    /* alloca il nuovo iteratore */
    ReturnStatus = MemAlloc(sizeof(J_ITERATOR), (void **)It);
    if( ReturnStatus == SUCCESS )
    {
    	/* Inizializza i campi dell'iteratore */
        (*It)->Data = Data;
        (*It)->Get = Get;
        (*It)->Delete = Delete;
    }

    return ReturnStatus;
}

/**
 * Recupera il prossimo elemento
 *
 * Questa funzione recupera il prossimo elemento dall'oggetto, richiamando
 * la funzione passata in input al costruttore
 *
 * @param It Iteratore generato da JIterator_New
 *
 * @return  Puntatore al prossimo elemento dell'oggetto, se esiste,
 * 			NULL altrimenti
 */
void *JIterator_Get(J_ITERATOR *It)
{
    return It->Get(It->Data);
}

/**
 * Dealloca l'iteratore
 *
 * Dealloca l'iteratore e l'oggetto
 *
 * @param It Iteratore generato da JIterator_New
 */
J_STATUS JIterator_Delete(J_ITERATOR *It)
{
	J_STATUS ReturnStatus;

	ReturnStatus = SUCCESS;

	if(It)
	{
		if(It->Data)
		{
			It->Delete(It->Data);
		}
		ReturnStatus = MemFree((void **)&It);
	}

	return ReturnStatus;
}
