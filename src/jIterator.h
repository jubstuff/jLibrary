/*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.

* Nome File : jIterator.h

* Scopo : Definizioni per il modulo Iterator

* Data di creazione : 13 Gennaio 2012

* Autore : Giustino Borzacchiello - giustinob@gmail.com

_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#ifndef J_ITERATOR_H
#define J_ITERATOR_H

#include "errors.h"

typedef void *(*GET_FN)(void *);
typedef void  (*DEL_FN)(void *);

typedef struct j_iterator_tag J_ITERATOR;

J_STATUS JIterator_New(void *Data, GET_FN Get, DEL_FN Delete, J_ITERATOR **It);
void   *JIterator_Get(J_ITERATOR *It);
J_STATUS JIterator_Delete(J_ITERATOR *It);

#endif
