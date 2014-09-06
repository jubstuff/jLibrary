/**
 * @file   common.h
 * @brief  Definizioni comuni di tipi di puntatori a funzione.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Ottobre, 2011
 *
 */

#ifndef JCOMMON_H
#define JCOMMON_H

/**
 * Una funzione che confronta due elementi
 *
 * @param Arg1 Riferimento al primo elemento da confrontare
 * @param Arg2 Riferimento al secondo elemento da confrontare
 *
 * @return {< 0, 0, > 0} se (Arg1 < Arg2), (Arg1 == Arg2), (Arg1 > Arg2), rispettivamente
 *
 * @note È responsabilità della funzione gestire eventuali valori NULL
 * */
typedef int (*COMPARATOR)(const void *Arg1, const void *Arg2); 

/**
 * Una funzione che inizializza il campo del nodo
 *
 * @param Value Riferimento al valore da copiare o associare al campo del nodo
 *
 * @return Il riferimento al nuovo valore allocato
 * */
typedef void *(*INITIALIZER)( void *Value );

/**
 * @brief Una funzione che dealloca il valore memorizzato nel campo di un nodo.
 *
 * @param Value Riferimento al valore da deallocare
 * */
typedef void (*DELETER)(void *NodeInfo);
/**
 * Una funzione che stampa a video il valore memorizzato nel campo di un nodo
 *
 * @param Value Riferimento al valore da stampare
 * */
typedef void (*PRINTER)(void *Value);

/**
 * Una funzione che gestisce l'occorrenza di nodi duplicati della lista all'atto
 * dell'inserimento
 *
 * @param Value Riferimento al valore passato in input
 * @param CurrentNode Riferimento al nodo in cui è stato trovato il duplicato
 *
 * @todo Ci puo stare NODE come tipo qui? No, perché in ogni caso l'utente non
 * saprebbe come gestirlo. Quindi si dovrebbe rimuovere. O bisognerebbe creare
 * dei metodi per gestire il nodo a cui l'utente può avere accesso.
 * */
typedef void (*DUPLICATE)( void *Value, void *CurrentNode );

/**
 * Una funzione che copia il valore della chiave del nodo passato in input
 *
 * */
typedef void (*GETTER)( const void *NodeValue, void **OutputValue );

/**
 * Funzione di hashing
 */
typedef unsigned int (*HASH)(void *Key);
/**
 * Funzioni di mapping
 */
typedef void (*APPLY)(void **Value, void *Input);
typedef void (*APPLY_ARRAY)(void **Value, int Index, void *Input);

#endif /* JCOMMON_H */
