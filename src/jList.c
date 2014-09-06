/*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.

* Nome File : jList.c

* Scopo : Implementazioni delle funzioni per la gestione di una lista a
 			concatenazione singola

* Data di creazione : 27 Ottobre 2011

* Autore : Giustino Borzacchiello - giustinob@gmail.com

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/
#include <stdlib.h>
#include "jList.h"
#include "jMem.h"

/**
 * DEFINIZIONE METODI PRIVATI
 * */
static NODE *List_RecursiveOrderedInsert ( void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op );
static NODE *List_LinkNewNode( NODE *NewNode, NODE *Current, J_STATUS *ReturnStatus );
static NODE *ListCreateNewNode(void *Value, JLIST_METHODS *Op);
static NODE *List_RecursiveDelete(void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op);
static void  List_RecursivePrint( NODE *Current, JLIST_METHODS *Op );
static NODE *List_RecursiveDestroy(NODE *Current, JLIST_METHODS *Op);
static NODE *List_RecursiveDeleteRange( NODE *Current, void *Inf, void *Sup, JLIST_METHODS *Op );
static NODE *List_RecursiveSearch( void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op);

/**
 * Inizializza una lista con i relativi metodi.
 *
 * */
J_STATUS JList_Init( J_LIST **L, JLIST_METHODS *Op, int AllocateMethods )
{
    J_STATUS ReturnStatus;

    ReturnStatus = SUCCESS;

    ReturnStatus = MemAlloc(sizeof(J_LIST), (void **)L);

    if( ReturnStatus == SUCCESS )
    {
        /* Inizializzo la testa della lista e i relativi metodi */
        (*L)->Head = NULL;

        if( AllocateMethods == 1 )
        {
        	(*L)->AllocaMetodi = 1;
			/* Alloca la struct per i metodi della lista */
			ReturnStatus = MemAlloc(sizeof(JLIST_METHODS), (void **)&((*L)->Op) );
			if( ReturnStatus == SUCCESS )
			{
				/* Copia i membri della struct Op passata in input
				 * nella struct Op appena allocata */
				*((*L)->Op) = *Op;
			}
			else
			{
				/* Allocazione delle operazioni fallita: deallocare lista */
				MemFree((void **)L);
			}
        }
        else
        {
        	(*L)->AllocaMetodi = 0;
        	/* Copia l'indirizzo della struct passata in ingresso */
        	(*L)->Op = Op;
        }


    }

    return ReturnStatus;
}
/**
 * Verifica se una lista è vuota
 *
 * @param L Puntatore alla lista
 *
 * @return 1 se la lista è vuota, 0 se ha almeno un elemento
 */
int JList_isEmpty( J_LIST *L )
{
    return L->Head == NULL;
}

/**
 * Inserisce Value in lista, rispettando la relazione d'ordine definito nel COMPARATOR
 *
 * Necessita dei metodi
 * COMPARATOR
 * INITIALIZER
 * DUPLICATE
 * */
J_STATUS JList_OrderedInsert( void *Value, J_LIST *L )
{
    J_STATUS ReturnStatus;

    L->Head = List_RecursiveOrderedInsert( Value, L->Head, &ReturnStatus, L->Op );
    return ReturnStatus;
}

/**
 * Inserisce un nodo in testa alla lista
 *
 * Necessita dei metodi:
 * INITIALIZER
 * */
J_STATUS JList_HeadInsert( void *Value, J_LIST *L )
{
    J_STATUS ReturnStatus;
    NODE *NewNode;

    ReturnStatus = SUCCESS;

    /*alloca e inizializza il nuovo nodo*/
    NewNode = ListCreateNewNode( Value, L->Op );
    L->Head = List_LinkNewNode(NewNode, L->Head, &ReturnStatus);

    return ReturnStatus;
}

/**
 * Rimuove un nodo dalla testa della lista e copia i dati del nodo in Value
 *
 * Necessita dei metodi:
 *
 * GETTER
 * DELETER
 *
 * */
J_STATUS JList_HeadDelete( void **Value, J_LIST *L )
{
    J_STATUS ReturnStatus;
    NODE *Temp;

    ReturnStatus = SUCCESS;

    if( L->Head != NULL )
    {
        /* Se la lista non è vuota, elimina il nodo in testa */

        /* Recupera i dati memorizzati nel nodo */
    	*Value = L->Head->Info;
        /* Aggiorno la testa della lista al secondo elemento */
        Temp = L->Head;
        L->Head = L->Head->Next;

        /* Libera memoria per il nodo */
        free( Temp );
        ReturnStatus = SUCCESS; /* Nodo trovato e rimosso */
    }
    else
    {
        /* Lista vuota */
        ReturnStatus = ERROR;
    }

    return ReturnStatus;
}

/**
 * Elimina un nodo contenente Value dalla lista
 *
 * Necessita dei metodi:
 * COMPARATOR
 * DELETER
 * */
J_STATUS JList_DeleteNode( void *Value, J_LIST *L )
{
    J_STATUS ReturnStatus;

    L->Head = List_RecursiveDelete( Value, L->Head, &ReturnStatus, L->Op);

    return ReturnStatus;
}

/**
 * Elimina i nodi compresi nell'intervallo [Inf, Sup] da una lista ordinata
 *
 * Necessita dei metodi:
 * COMPARATOR
 * DELETER
 *
 * =NOTA=
 * Il comportamento di questa funzione è indefinito, se applicato a liste non ordinate
 *
 * */
void JList_DeleteRange( void *Inf, void  *Sup, J_LIST *L )
{
    L->Head = List_RecursiveDeleteRange( L->Head, Inf, Sup, L->Op );
}

/* 
 * Stampa la lista
 *
 * Necessita dei metodi:
 * PRINTER
 *
 * */
void JList_Print( J_LIST *L )
{
    List_RecursivePrint( L->Head, L->Op );
}

/**
 * Richiama una funzione su tutti gli elementi della lista
 *
 * @param apply funzione da richiamare
 * @param L     Lista
 */
void JList_Map(void apply(void **Value, void *Input), void *Input, J_LIST *L)
{
	NODE *Current;

	Current = L->Head;
	while( Current != NULL )
	{
		apply(&Current->Info, Input);
		Current = Current->Next;
	}
}

/**
 * Dealloca l'intera lista
 *
 * Necessita dei metodi:
 * DELETER
 * */
void JList_Destroy( J_LIST *L )
{
	if( L )
	{
		L->Head = List_RecursiveDestroy(L->Head, L->Op);
		if( L->AllocaMetodi )
		{
			MemFree( (void **)&(L->Op) ); //FIXME Se i metodi sono condivisi da più liste, non è possibile deallocarli sempre
		}
		MemFree( (void **)&L );
	}

}
/**
 * Cerca un nodo nella lista
 *
 * Cerca un nodo all'interno della lista, data la chiave di ricerca e
 * restituisce il puntatore al valore memorizzato nel nodo.
 *
 * @param Key Puntatore al valore da ricercare nella lista
 * @param L   Puntatore alla lista
 * @param OutValue Indirizzo del puntatore in cui salvare il campo Info del nodo
 *
 * @return Esito dell'operazione
 * 			- SUCCESS se il nodo è trovato
 * 			- W_LIST_NOTFOUND se il nodo non è stato trovato
 */
J_STATUS JList_Search( void *Key, void **OutValue, J_LIST *L )
{
    J_STATUS ReturnStatus;
    NODE *Found;
    NODE *Temp;

    /* Inizializzazione variabili */
    ReturnStatus = W_LIST_NOTFOUND; /* Default: nodo non trovato */
    Found = NULL;
    Temp = L->Head;
    /* Cerca il nodo nella lista */
    Found = List_RecursiveSearch( Key, Temp, &ReturnStatus, L->Op);

    if( Found )
    {
    	if( OutValue )
    	{
    		/* Restituisci il campo info del nodo */
			*OutValue = Found->Info;
    	}

    }
    else
    {
    	/* Imposta campo info di ritorno a NULL */
    	if( OutValue )
    	{
    		*OutValue = NULL;
    	}
    }

    return ReturnStatus;
}

void *JNode_GetData( NODE *N )
{
    return N->Info;
}

/*********************************************************************************
 * IMPLEMENTAZIONE METODI PRIVATI                                                *
 *                                                                               *
 *********************************************************************************/

static NODE *List_RecursiveSearch( void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op)
{
    NODE *Result;

    Result = NULL;

	/* cerca il nodo solo se la lista non è vuota */
	if( Current != NULL )
	{
		/* se il nodo corrente è quello cercato */
		if( Op->Compare( Current->Info, Value ) == 0 )
		{
			/* Esegui operazioni sul nodo */
			*ReturnStatus = SUCCESS;
			Result = Current;
		}
		/* altrimenti prosegui la ricerca */
		else
		{
			Result = List_RecursiveSearch( Value, Current->Next, ReturnStatus, Op );
		}
	}
	return Result;

}



/**
 * Inserisce un nodo all'interno della lista
 *
 * Questa funzione utilizza un approccio ricorsivo per inserire un nodo
 * con campo <Value> all'interno della lista <Current>.
 * 
 * @param Value        Valore da inserire nel nodo.
 * @param Current      Testa della lista in cui inserire il nodo.
 * @param ReturnStatus Esito dell'operazione. Può assumere valore:
 *	                   - 0, in caso di inserimento corretto
 *                     - E_MALLOC, in caso di errore nella creazione del nodo
 *                     - W_DUPLICATE, in caso di valore già presente in lista
 * @param Op           Riferimento al record contenente le operazioni di manipolazione
 *                     dei nodi.
 *
 * @return Il puntatore alla testa della lista, eventualmente modificato
 */ 
static NODE *List_RecursiveOrderedInsert ( void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op ) 
{
	NODE *NewNode;
	*ReturnStatus = SUCCESS; 
	
	/*se la lista è vuota oppure il nodo è maggiore, inserisci un nuovo nodo */
	if( Current == NULL || ( Op->Compare( Current->Info, Value ) > 0 ) )
	{
		/*alloca e inizializza il nuovo nodo*/
		NewNode = ListCreateNewNode( Value, Op );
		Current = List_LinkNewNode(NewNode, Current, ReturnStatus);
	}   
	/* se il valore del nodo corrente è uguale a quello in ingresso
	 * notifica la condizione tramite ReturnStatus */
	else if( ( Op->Compare( (void *)Current->Info, (void *)Value ) == 0 )  )
	{
		/* Esiste già un nodo con pari campo nella lista */
		if( Op->ManageDuplicate ) {
			Op->ManageDuplicate( Value, Current );
		}
		*ReturnStatus = W_LIST_DUP;
	}
	else
	{
		/* vai avanti nella ricerca, aggiornando il campo Next del nodo corrente */
		Current->Next = List_RecursiveOrderedInsert(Value, Current->Next, ReturnStatus, Op);
	}
	return Current;
}

/**
 * Alloca un nuovo nodo, con relativo campo
 *
 * @param Value Valore da inserire nel nodo
 * @param Op    Riferimento al record contenente le operazioni di manipolazione
 *              dei nodi.
 *
 * @return Il riferimento al nuovo nodo creato, oppure NULL in caso di fallimento
 *         nell'allocazione
 * */ 
static NODE *ListCreateNewNode(void *Value, JLIST_METHODS *Op)
{
	NODE * NewNode;
	J_STATUS Status;

	/* Alloca spazio per il nuovo nodo */
	Status = MemAlloc(sizeof(NODE), (void **)&NewNode);
	if( Status == SUCCESS )
    {
		/* Associa il valore passato dall'utente al campo del nodo */
		NewNode->Info = Op->InitNode(Value);
		NewNode->Next = NULL;
    }

	return NewNode;
}
/**
 * Collega il nuovo nodo a quello corrente
 *
 * @param NewNode nuovo nodo da inserire
 * @param Current nodo che precede il nuovo nodo
 * @param ReturnStatus Esito dell'operazione
 *
 * @return Il nodo corrente, eventualmente modificato
 */
static NODE *List_LinkNewNode( NODE *NewNode, NODE *Current, J_STATUS *ReturnStatus )
{
	if ( NewNode != NULL )
	{
		/* Posiziona il nuovo nodo prima del nodo corrente */
		NewNode->Next = Current;
		Current = NewNode;
	}
	else
	{
		*ReturnStatus = E_NO_MEM;
	}

	return Current;
}


/**
 * Rimuove un nodo dalla lista
 *
 * Questa funzione utilizza un approccio ricorsivo per rimuovere un nodo
 * con campo <Value> dalla lista <Current>.
 * 
 * NOTE Se un nodo con campo pari a <Value> non esiste, la lista
 * non viene modificata
 * 
 * @param Value        Valore associata al nodo da rimuovere.
 * @param Current      Testa della lista da cui rimuovere il nodo.
 * @param ReturnStatus Esito dell'operazione. Può assumere valore:
 *                     - SUCCESS, in caso di nodo trovato e rimosso
 *                     - W_LIST_NOTFOUND, in caso di nodo non trovato
 * @param Op           Riferimento al record contenente le operazioni di manipolazione
 *                     dei nodi.
 *
 * @return Il puntatore alla testa della lista, eventualmente modificato
 */
static NODE *List_RecursiveDelete(void *Value, NODE *Current, J_STATUS *ReturnStatus, JLIST_METHODS *Op) 
{
    NODE *Temp; /**< Nodo di appoggio per cancellazione */
    
    *ReturnStatus = W_LIST_NOTFOUND;

	/* cerca il nodo solo se la lista non è vuota */
	if( Current != NULL )
    {
        /* se il nodo corrente è quello cercato, cancellalo */
		if( Op->Compare( Current->Info, Value ) == 0 )
		{
			Temp = Current->Next;
			/* Dealloca il campo chiave del nodo */
			Op->Delete( Current->Info );
			
			/* Libera memoria per il nodo */
   			free( Current );
			Current = Temp;
			*ReturnStatus = SUCCESS; /* Nodo trovato e rimosso */
		}
		/* altrimenti prosegui la ricerca */
		else
		{
			Current->Next = List_RecursiveDelete( Value, Current->Next, ReturnStatus, Op );
		}
	} 
	return Current;
}

/**
 * Cancella i nodi compresi in un intervallo
 *
 * Permette di cancellare tutti i nodi i cui campi sono compresi tra Inf e Sup,
 * estremi compresi.
 * Si assume Inf < Sup (secondo la relazione di ordinamento applicabile). In caso
 * contrario, la funzione non modifica la lista.
 *
 * @param Current Testa della lista da cui rimuovere i nodi.
 * @param Inf     Estremo inferiore dell'intervallo.
 * @param Sup     Estremo superiore dell'intervallo.
 * @param Op      Riferimento al record contenente le operazioni di manipolazione
 *                dei nodi.
 *
 * @return Il puntatore alla testa della lista, eventualmente modificato
 * */
static NODE *List_RecursiveDeleteRange( NODE *Current, void *Inf, void *Sup, JLIST_METHODS *Op )
{
	NODE *Temp;

    /* Se la lista non è vuota oppure il limite inferiore è maggiore del limite superiore
	 * non fare nulla */
	if( (Current != NULL) || (Op->Compare(Inf, Sup) > 0)  )
	{
		/* Scorri la lista fino al nodo che supera il limite superiore */
		if( Op->Compare(Current->Info, Sup) <= 0 )
		{
			Current->Next = List_RecursiveDeleteRange( Current->Next, Inf, Sup, Op );
			/* Cancella i nodi finché sono maggiori del limite inferiore */
			if( Op->Compare(Current->Info, Inf) >= 0 )
			{
				Temp = Current->Next;
				Op->Delete( Current->Info);
				free(Current);
				Current = Temp;
			}
		}
	}
	return Current;
}


/**
 * Dealloca tutti i nodi della lista
 *
 * @param Current Testa della lista da deallocare.
 * @param Op      Riferimento al record contenente le operazioni di manipolazione
 *                dei nodi.
 *
 * @return Il puntatore alla testa eventualmente modificato.
 */
static NODE *List_RecursiveDestroy(NODE *Current, JLIST_METHODS *Op)
{
    if( Current != NULL )
	{
		/* scorre la lista fino all'ultimo ed effettua la cancellazione
		 * in ordine inverso */
		Current->Next = List_RecursiveDestroy(Current->Next, Op);
		Op->Delete( Current->Info);
		free(Current);
		Current = NULL;
	}
	return Current;
}

/**
 * Stampa i campi della lista, in ordine
 *
 * Questa funzione utilizza un approccio ricorsivo per scorrere la lista e
 * stampare a video tutti i campi dei nodi, in ordine.
 * La procedura per stampare i nodi è memorizzata in <Op>.
 *
 * @param Current Testa della lista da stampare a video.
 * @param Op      Riferimento al record contenente le operazioni di manipolazione
 *                dei nodi.
 *
 */
static void List_RecursivePrint( NODE *Current, JLIST_METHODS *Op )
{
    /* stampa la lista, se non vuota */
	if( Current != NULL )
	{
		Op->Print(Current->Info);
		List_RecursivePrint(Current->Next, Op);
	}
}

