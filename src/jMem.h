/**
 * mem.h
 *
 * Abstraction for C's standard library memory management functions
 *
 * Questo modulo crea delle interfacce consistenti per tutte le funzioni di
 * gestione della memoria della libreria standard del C.
 *
 * Tutte le funzioni restituiscono lo stato dell'operazione e accettano in input
 * l'indirizzo del puntatore da modificare
 *
 * */
#include "errors.h"
#include <stdlib.h>

#ifndef MEM_H
#define MEM_H

/**
 * Alloca memoria dinamica - interfaccia per malloc
 *
 * @param Size Numero di byte da allocare
 * @param OutPtr Indirizzo del puntatore alla memoria
 *
 * @return esito dell'operazione
 * 			 - SUCCESS, se l'allocazione è riuscita
 * 			 - E_NO_MEM, se la memoria è esaurita
 */
extern J_STATUS MemAlloc( size_t Size, void **OutPtr);
/**
 * Alloca memoria dinamica - interfaccia per calloc
 *
 * @param Count
 * @param Size
 * @param OutPtr
 *
 * @return esito dell'operazione
 */
extern J_STATUS MemCalloc( size_t Count, size_t Size, void **OutPtr);
extern J_STATUS MemFree( void **Ptr );
extern J_STATUS MemRealloc( long Nbytes, void **Ptr );

#endif
