/**
 * @file   jSet.h
 * @brief
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Aprile, 2012
 *
 *
 * jSet è
 */
/*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.

* Nome File : jSet.h

* Scopo : Definizioni per il modulo Set

* Data di creazione : 13 Gennaio 2012

* Autore : Giustino Borzacchiello - giustinob@gmail.com

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#ifndef J_SET_H
#define J_SET_H

#include "errors.h"
#include "common.h"
#include "jList.h"

typedef struct j_set_methods_tag
{
	COMPARATOR Compare;
	DELETER Delete;
	INITIALIZER Initialize;
	PRINTER Print;
	HASH Hash;
} JSET_METHODS;


/*
 * J_SET implementato come hash
 */
typedef struct j_set_tag {
	int Size;      /**< Numero di buckets */
	int ElemCount; /**< Numero di elementi inseriti nell'insieme */

	J_LIST **Buckets; /**< Array di liste */
	JLIST_METHODS *ListOp; /**< Metodi per operare sulle liste */

	HASH Hash; /**< Funzione di hashing */
} J_SET;



J_STATUS JSet_New(int Hint, JSET_METHODS *Op, J_SET **Set);

J_STATUS JSet_Add(void *Data, J_SET *Set);

void JSet_Print( J_SET *Set );

void  JSet_Destroy(J_SET *Set);

J_STATUS JSet_Delete(void *Key, J_SET *Set);

J_STATUS JSet_Search(void *Key, void **OutValue, J_SET *Set);

int   JSet_GetNumElem(J_SET *Set);

int JSet_GetSize(J_SET *Set);

void JSet_Map(APPLY apply, void *Input, J_SET *Set);





#endif
