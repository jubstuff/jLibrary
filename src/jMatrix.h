/**
 * @file   jMatrix.h
 * @brief  Implementazione di una matrice dinamica.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Aprile, 2012
 *
 *
 * jMatrix è una matrice di puntatori a void dinamica.
 */
#ifndef JMATRIX_H
#define JMATRIX_H

#include "errors.h"
#include "common.h"

typedef struct jmatrix_tag {
	int Rows;             /**< Numero di righe attive */
	int Cols;             /**< Numero di colonne attive */
	int MaxRows;          /**< Massimo numero di righe (buffer) */
	int MaxCols;          /**< Massimo numero di colonne (buffer) */
	INITIALIZER InitData; /**< Funzione di inizializzazione dei dati */
	DELETER DeleteData;   /**< Funzione di deallocazione dei dati */
	void ***Matrix;       /**< Matrice di puntatori */
} J_MATRIX;

/**
 * Crea una nuova matrice
 *
 * Alloca una nuova matrice di dimensioni massime Rows x Cols.
 * La matrice inizialmente è vuota. Prima di poter inserire degli elementi è
 * necessario aggiungere righe e/o colonne tramite i metodi
 * JMatrix_AddRow/JMatrix_AddRowCol/JMatrix_AddColumn
 *
 * @param [in]  Rows   Numero di righe iniziali della matrice
 * @param [in]  Cols   Numero di colonne iniziali della matrice
 * @param [in]  Init   Funzione di callback da richiamare all'inserimento di un nuovo elemento
 * @param [in]  Delete Funzione di callback da richiamare alla rimozione di un elemento
 * @param [out] M      Indirizzo del puntatore in cui salvare la matrice
 *
 * @retval SUCCESS  Matrice allocata correttamente
 * @retval E_NO_MEM Allocazione fallita, memoria esaurita
 * @retval ERROR    Metodi Init e Delete mancanti
 */
J_STATUS JMatrix_New(int Rows, int Cols, INITIALIZER Init, DELETER Delete, J_MATRIX **M);

/**
 * Dealloca una matrice esistente
 *
 * Questa funzione dealloca una matrice e tutto il suo contenuto.
 *
 * @param [in] Puntatore alla matrice da deallocare
 */
void JMatrix_Destroy( J_MATRIX *M );

/**
 * Inserisce un elemento nella matrice
 *
 * Inserisce un elemento nella matrice, nella posizione (Row, Col).
 * Se gli indici eccedono le dimensioni della matrice, viene restituito un
 * warning.
 * Se nella posizione (Row, Col) esiste già un elemento, quest'ultimo viene
 * sovrascritto.
 *
 * @param [in] Data Puntatore all'elemento da inserire nella matrice
 * @param [in] Row  Riga in cui inserire l'elemento
 * @param [in] Col  Colonna in cui inserire l'elemento
 * @param [in] M    Puntatore alla matrice
 *
 * @retval SUCCESS      Elemento inserito correttamente.
 * @retval W_OUTOFBOUND Gli indici eccedono le dimensioni della matrice
 */
J_STATUS JMatrix_Set(void *Data, int Row, int Col, J_MATRIX *M);

/**
 * Recupera un elemento dalla matrice
 *
 * Recupera l'elemento in posizione (Row,Col) della matrice M, 
 * passandone l'indirizzo nel parametro Data passato in input.
 *
 * @param [out] Data Puntatore all'elemento da recuperare.
 * @param [in]  Row  Riga da cui recuperare l'elemento.
 * @param [in]  Col  Colonna da cui recuperare l'elemento.
 * @param [in]  M    Puntatore alla matrice.
 *
 * @retval SUCCESS      Elemento recuperato correttamente.
 * @retval W_OUTOFBOUND Gli indici eccedono le dimensioni della matrice.
 */
J_STATUS JMatrix_Get(void **Data, int Row, int Col, J_MATRIX *M);


/**
 * Recupera il numero di righe della matrice.
 *
 * @param [in] M    Puntatore alla matrice
 */
int JMatrix_GetRows(J_MATRIX *M);

/**
 * Recupera il numero di colonne della matrice.
 *
 * @param [in] M    Puntatore alla matrice
 */
int JMatrix_GetCols(J_MATRIX *M);

/**
 * Aggiunge una riga alla matrice
 *
 * Aggiunge una riga alla matrice, incrementando il numero di righe attive.
 *
 * @param [in] M    Puntatore alla matrice
 */
J_STATUS JMatrix_AddRow(J_MATRIX *M);

/**
 * Aggiunge una colonna alla matrice.
 *
 * Aggiunge una colonna alla matrice, incrementando il numero di colonne attive.
 *
 * @param [in] M    Puntatore alla matrice
 */
J_STATUS JMatrix_AddColumn(J_MATRIX *M);
/**
 * Aggiunge una riga ed una colonna alla matrice.
 *
 * Aggiunge una riga ed una colonna alla matrice incrementando il numero
 * di righe e colonne attive.
 *
 * @param [in] M    Puntatore alla matrice
 */
void JMatrix_AddRowCol(J_MATRIX *M);
/**
 * Imposta un valore di default a tutti gli elementi della matrice
 *
 * @param [in] DefaultValue Valore predefinito da assegnare agli elementi
 * @param [in] M            Puntatore alla matrice
 *
 */
void JMatrix_SetDefaultValues(void *DefaultValue, J_MATRIX *M);

#endif /* JMATRIX_H */


