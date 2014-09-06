/**
 * @file   errors.h
 * @brief  Messaggi di stato restituiti dalle funzioni della libreria.
 * @author Giustino Borzacchiello (giustinob@gmail.com)
 * @date   Ottobre, 2011
 *
 */
#ifndef J_ERRORS
#define J_ERRORS
/**
 * Memorizza i messaggi di stato restituiti dalle varie funzioni facenti
 * parte della libreria.
 * I messaggi si suddividono in:
 *  - notifiche (SUCCESS)
 *  - warning (Iniziano con W_), per avvertimenti non fatali
 *  - errori  (Iniziano per E_), per errori fatali
 */
typedef enum
{
	SUCCESS,         /**< Esito OK */
	E_NO_MEM,        /**< Errore in malloc, memoria insufficiente */
	ERROR,           /**< Errore generico */
	W_LIST_NOTFOUND, /**< Nodo non trovato nella lista */
	W_LIST_DUP,      /**< Trovato un duplicato nella lista */
	W_NOTFOUND,      /**< Elemento non trovato */
	W_OUTOFBOUND,    /**< Superamento dei limiti dell'array */

} J_STATUS;

#endif
