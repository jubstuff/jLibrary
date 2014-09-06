/**
 * @file   jList.h
 * @brief  Strutture e definizione di funzioni per una lista a concatenazione singola.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Aprile, 2011
 *
 *
 * Realizza alcune semplici funzioni per una lista, quali:
 *  - inserimento di un nodo
 *  - cancellazione di un nodo
 *  - stampa della lista
 *  - cancellazione di tutti i nodi della lista
 *  - cancellazione di nodi compresi in un intervallo
 *
 * La lista è di tipo a singola concatenazione.
 */
#ifndef J_LIST_H
#define J_LIST_H
#include "errors.h"
#include "common.h"

/*=============================================================================*
 * Definizioni struttura nodo
 =============================================================================*/
typedef struct node_tag NODE;
/**
 * Nodo di una lista.
 *
 * Questa struct contiene il puntatore al nodo successivo ed un puntatore ai
 * dati satellite che gli sono stati associati.
 */
struct node_tag
{
	NODE *Next; /**< Puntatore al nodo successivo */
	void *Info; /**< Campo del nodo */
};

/**
 * Questa struct contiene tutte le operazioni che permettono la gestione del 
 * particolare tipo di nodo associato ad una lista.
 *
 * La libreria si aspetta che queste funzioni seguano le linee guida presentate
 * nella descrizione di ogni tipo di puntatore a funzione.
 *
 * @see common.h
 *
 * */
typedef struct list_op_tag {
	COMPARATOR Compare;        /**< Confronta due nodi */
	INITIALIZER InitNode;      /**< Inizializza un nodo */
	DELETER Delete;            /**< Elimina un nodo */
	PRINTER Print;             /**< Stampa un nodo */
	DUPLICATE ManageDuplicate; /**< Gestisce nodi duplicati */
	GETTER GetNodeValue;       /**< Recupera la chiave del nodo */
} JLIST_METHODS;

/**
 * Una lista a concatenazione singola.
 *
 * Questa struttura contiene i metodi per operare su una lista, la lista stessa ed
 * un flag per indicare se la struct contenente i metodi deve essere allocata o meno.
 * Tale flag è stato introdotto per consentire, nel caso molte liste condividano gli
 * stessi metodi, di allocare un'unica struttura con i metodi.
 */
typedef struct list_tag
{
	NODE *Head;        /**< Testa della lista */
	int AllocaMetodi;  /**< Flag che indica l'allocazione della struct dei metodi */
	JLIST_METHODS *Op; /**<	 Operazioni relative alla lista */
} J_LIST;

/*==============================================================================
 * Funzioni per la gestione della lista
 *============================================================================*/

int      JList_isEmpty( J_LIST *L );
/**
 * @param AllocateMethods int Se 0, la struttura dei metodi viene copiata anziché allocata
 * 							  Se diversa da 0, viene allocata
 */
J_STATUS JList_Init( J_LIST **L, JLIST_METHODS *Op, int AllocateMethods );
/**
 * Inserisce un nodo nella lista, mantenendo l'ordine
 *
 * @param Value Valore da inserire nella lista
 * @param L		Lista in cui inserire
 *
 * @return Esito dell'operazione
 */
J_STATUS JList_OrderedInsert( void *Value, J_LIST *L );
/**
 * Inserisce un nodo in testa ad una lista
 *
 * @param Value Valore da inserire nella lista
 * @param L		Lista in cui inserire
 *
 * @return Esito dell'operazione
 *
 */
J_STATUS JList_HeadInsert( void *Value, J_LIST *L );
/**
 * @brief Rimuove un nodo dalla testa della lista.
 *
 * Questa funzione rimuove il nodo che si trova in testa alla lista e
 * restituisce un puntatore ai dati associati a quel nodo.
 *
 * È compito dell'utilizzatore deallocare i dati recuperati tramite questa
 * funzione.
 *
 * @param Value Indirizzo del puntatore in cui salvare i dati del nodo.
 * @param L		Lista da cui rimuovere il nodo in testa
 *
 * @return Esito dell'operazione
 */
J_STATUS JList_HeadDelete( void **Value, J_LIST *L );

J_STATUS JList_DeleteNode( void *Value, J_LIST *L );

J_STATUS JList_Search( void *Key, void **OutValue, J_LIST *L );

void     JList_DeleteRange( void *Inf, void  *Sup, J_LIST *L );

void     JList_Destroy( J_LIST *L );

void     JList_Print( J_LIST *L );

void JList_Map(void apply(void **Value, void *Input), void *Input, J_LIST *L);

/*==============================================================================
 * Funzioni per la gestione dei nodi
 *============================================================================*/

void *JNode_GetData( NODE *N );


#endif /* J_LIST_H */
