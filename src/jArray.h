/**
 * @file   jArray.h
 * @brief  Implementazione di un array dinamico.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Aprile, 2012
 *
 *
 * jArray è un array che può essere ridimensionato dinamicamente.
 */

#ifndef J_ARRAY_H
#define J_ARRAY_H

#include "errors.h"
#include "common.h"

/**
 * Struct che rappresenta un array dinamico.
 *
 * Questa struct tiene traccia della dimensione dell'array e del numero di
 * elementi contenuti in esso.
 * L'allocazione e la deallocazione degli elementi viene eseguita tramite
 * le funzioni InitData e DeleteData memorizzate all'interno della struct.
 * L'array è realizzato mediante un array di puntatori void.
 */
typedef struct j_array_tag
{
	int Size;             /**< Numero massimo di elementi */
	int ElemCount;        /**< Numero di elementi inseriti nell'array */
	INITIALIZER InitData; /**< Funzione di inizializzazione degli elementi dell'array */
	DELETER DeleteData;   /**< Funzione di deallocazione degli elementi dell'array */
	void **Array;         /**< Array */
} J_ARRAY;

/**
 * @brief   Alloca un nuovo array dinamico e lo inizializza.
 *
 * @ingroup jArray
 *
 * Questo metodo alloca un nuovo array dinamico di dimensione StartSize.
 *
 * Ogni elemento di questo array viene inizializzato tramite la funzione
 * Init passata in ingresso e deallocato con la funzione Delete.
 *
 * @param [in]  StartSize Dimensione iniziale dell'array
 * @param [in]  Init      Funzione di inizializzazione di un elemento
 * @param [in]  Delete    Funzione di deallocazione di un elemento
 * @param [out] A         Indirizzo del puntatore in cui salvare l'array allocato
 *
 * @retval SUCCESS  Array allocato correttamente
 * @retval E_NO_MEM Allocazione fallita, memoria esaurita
 */
J_STATUS JArray_New(int StartSize, INITIALIZER Init, DELETER Delete, J_ARRAY **A);
/**
 * @brief   Dealloca l'array e tutti i dati contenuti.
 * @ingroup jArray
 *
 * Questa funzione dealloca tutti gli elementi esistenti nell'array, quindi
 * dealloca l'array stesso e la struttura che lo contiene.
 *
 * @param [in] A Array da deallocare.
 *
 */
void JArray_Destroy( J_ARRAY *A );

/**
 * @brief Recupera la dimensione dell'array.
 *
 * Questa funzione recupera la dimensione attuale dell'array.
 *
 * @param [in] A Array di cui recuperare la dimensione
 *
 * @return Dimensione attuale dell'array.
 */
int JArray_GetSize(J_ARRAY *A);

/**
 * @brief Recupera il numero di elementi nell'array.
 *
 * Questa funzione recupera il numero di elementi attualmente inseriti
 * nell'array.
 *
 * @param [in] A Array di cui recuperare il numero di elementi
 *
 * @return Numero di elementi memorizzati nell'array.
 */
int JArray_GetElemCount( J_ARRAY *A);

/**
 * @brief Aggiunge un elemento nell'array, data la posizione
 *
 * Questa funzione aggiunge l'elemento Data nella posizione Pos, se questa è
 * compresa nei limiti dell'array.
 *
 * Il valore da inserire nell'array viene allocato dinamicamente tramite la
 * funzione Init passata in ingresso a JArray_New.
 *
 * Se alla posizione Pos è già presente un elemento, quest'ultimo viene deallocato
 * prima di procedere all'inserimento dei nuovi dati.
 *
 * @param [in] Data Dati da inserire nell'array
 * @param [in] Pos  Indice in cui inserire i dati
 * @param [in] A    Array in cui inserire i dati
 *
 * @retval SUCCESS      Inserimento eseguito correttamente
 * @retval W_OUTOFBOUND L'indice passato in input eccede i limiti dell'array
 */
J_STATUS JArray_Set(void *Data, int Pos, J_ARRAY *A);

/**
 * @brief Recupera l'elemento memorizzato alla posizione Pos dell'array.
 *
 * Questa funzione recupera l'elemento memorizzato alla posizione Pos dell'array, se
 * è compresa nei limiti dell'array, e lo salva nel parametro Out.
 *
 * @param [in]  Pos Indice da cui recuperare i dati.
 * @param [out] Out Puntatore in cui salvare l'indirizzo dei dati.
 * @param [in]  A   Array da cui recuperare i dati.
 *
 * @retval SUCCESS      Dati recuperati correttamente
 * @retval W_OUTOFBOUND L'indice passato in input eccede i limiti dell'array
 */
J_STATUS JArray_Get(int Pos, void **Out, J_ARRAY *A);

/**
 * @brief Dealloca l'elemento alla posizione Pos.
 *
 * Questa funzione dealloca l'elemento presente alla posizione Pos dell'array,
 * se esistente, e diminuisce il numero di elementi presenti nell'array.
 *
 * @param [in] Pos Indice dell'array da cui eliminare i dati.
 * @param [in] A   Array da cui eliminare i dati.
 *
 * @retval SUCCESS Dati deallocati correttamente.
 * @retval W_OUTOFBOUND L'indice passato in input eccede i limiti dell'array.
 */
J_STATUS JArray_Delete(int Pos, J_ARRAY *A);

/**
 * @brief Ridimensiona un array.
 *
 * Questa funzione ridimensiona un array già allocato.
 *
 * Se la dimensione è minore di quella attuale, tutti gli elementi che eccedono
 * la nuova dimensione vengono deallocati.
 * Altrimenti l'array viene espanso per poter contenere nuovi elementi.
 *
 * @param [in] Size Nuova dimensione dell'array.
 * @param [in] A    Array da ridimensionare.
 *
 * @retval SUCCESS  Array ridimensionato correttamente.
 * @retval ERROR    Passata in input dimensione negativa.
 * @retval E_NO_MEM Reallocazione fallita.
 *
 */
J_STATUS JArray_Resize(int Size, J_ARRAY *A);
/**
 * @brief Richiama una funzione su tutti gli elementi dell'array.
 *
 * Questa funzione richiama la funzione Apply su tutti gli elementi
 * dell'array passandogli come parametri l'indirizzo dell'elemento e
 * il valore Input passato in ingresso a questa funzione.
 *
 * @param [in] Apply Funzione da richiamare su ogni elemento.
 * @param [in] Input Valore da passare ad ogni chiamata della funzione Apply.
 * @param [in] A     Array da esaminare.
 */
void JArray_Map(APPLY_ARRAY Apply, void *Input, J_ARRAY *A);

#endif /* J_ARRAY_H */
