/*! \file generic_queue.h
 *
 * \brief File (FIFO) générique contenant des valeurs de type <b>
 * void* </b>
 *
 */

/*****************************************************************************
*                                                                            *
*  ------------------------------- queue.h --------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  Implement queues as linked lists.                                         *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour représenter une file générique
    contenant des valeurs de type <b> void* </b>
 *
 * Une file est une liste chaînée gérée de manière adéquate: insertion
 * en tête et suppression en queue.
 */
typedef generic_list_t generic_queue_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

/*! \brief Initialisation d'une nouvelle file générique contenant des
 *  valeurs de type <b> void* </b>
 *
 * Utilise la fonction d'initalisation des listes chaînée pour mettre
 * à zéro la taille et à NULL les pointeurs vers le premier et le
 * dernier élément de la liste, i.e., de la file. Cette fonction
 * nécessite une fonction de comparaison des éléments de la file, une
 * fonction de construction d'un élément de la file et une fonction de
 * destruction des éléments de la file.
 *
 * @param queue est un pointeur vers la structure de données des files
 * generic_queue_t
 *
 * @param compare fonction permettant la comparaison de deux éléments
 * de la file
 *
 * @param build fonction permettant la construction d'une valeur de la
 * file
 *
 * @param destroy fonction permettant la destruction d'une valeur de
 * la file
 */
void generic_queue_init (generic_queue_t *queue,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build) (const void *data),
			 void (*destroy) (void *data));

/*! \brief Destruction d'une file
 *
 * Utilise la fonction de destruction des listes chaînées pour
 * parcourir la liste et détruire tous les éléments de la file.
 *
 * @param queue est un pointeur vers la structure de données des piles
 * generic_queue_t
 */
void generic_queue_destroy (generic_queue_t *queue);

/*! \brief Ajouter un élément de type <b>void*</b> dans la file
 *
 * Etant donné un pointeur vers une file (représentée par une liste
 * chaînée) et une valeur de type <b>void*</b>, ajoute un nouvel
 * élément à la file avec cette valeur.
 *
 * @param queue est un pointeur vers la structure de données
 * generic_queue_t
 *
 * @pram data une valeur de type <b>void*</b> à ajouter à la file
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour OK, et
 * -1 en cas de problème mémoire)
 */
int generic_queue_enqueue(generic_queue_t *queue, const void *data);

/*! \brief Supprime un élément de type <b>void*</b> dans la file
 *
 * Etant donné un pointeur vers une file (représentée par une liste
 * chaînée), supprime l'élément le plus ancien de la file.
 *
 * @param queue est un pointeur vers la structure de données generic_queue_t
 *
 * @pram data un pointeur sur type <b>void*</b> qui permet de
 * récupérer la valeur de l'élément supprimer de la file
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour OK, et
 * -1 en cas de problème mémoire)
 */
int generic_queue_dequeue(generic_queue_t *queue, void **data);

/*! \brief Récupère la valeur de type <b>void*</b> le plus ancien de la file
 *
 * Etant donné un pointeur vers une file (représentée par une liste
 * chaînée), récupère la valeur de l'élément le plus ancien de la file
 * sans le supprimer.
 *
 * @param queue est un pointeur vers la structure de données generic_queue_t
 * @return une valeur de <b>void*</b> de l'élément le plus ancien de la file
 */
void* generic_queue_peek(generic_queue_t *queue);

/*! \brief Accesseur sur la taille d'une file
 *
 * Etant donné un pointeur vers une file (représentée par une liste
 * chaînée), retourne la valeur du champ size
 *
 * @param queue est un pointeur vers la structure de données generic_queue_t
 * @return un entier indiquant la taille de la file
 */
int generic_queue_size(generic_queue_t *queue);

#endif
