/*! \file generic_stack.h
 *
 * \brief File (FIFO) générique contenant des valeurs de type <b>
 * void* </b>
 *
 */

/*****************************************************************************
*                                                                            *
*  ----------------------- generic_stack.h --------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  Implement stacks as linked lists.                                         *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour représenter une pile générique
    contenant des valeurs de type <b> void* </b>
 *
 * Une pile est une liste chaînée gérée de manière adéquate: insertion
 * en tête et suppression en tête.
 */
typedef generic_list_t generic_stack_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

/*! \brief Initialisation d'une nouvelle pile contenant des valeurs de
    type <b> void* </b>
 *
 * Utilise la fonction d'initalisation des listes chaînée pour mettre
 * à zéro la taille et à NULL les pointeurs vers le premier et le
 * dernier élément de la liste, i.e., de la pile. Cette fonction
 * nécessite une fonction de comparaison des éléments de la pile, une
 * fonction de construction d'un élément de la pile et une fonction de
 * destruction des éléments de la pile.
 *
 * @param queue est un pointeur vers la structure de données des piles
 * generic_queue_t
 *
 * @param compare fonction permettant la comparaison de deux éléments
 * de la pile
 *
 * @param build fonction permettant la construction d'une valeur de la
 * pile
 *
 * @param destroy fonction permettant la destruction d'une valeur de
 * la pile
 */
void generic_stack_init (generic_stack_t *stack,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build) (const void *data),
			 void (*destroy) (void *data));

/*! \brief Destruction d'une pile
 *
 * Utilise la fonction de destruction des listes chaînées pour
 * parcourir la liste et détruire tous les éléments de la pile.
 *
 * @param stack est un pointeur vers la structure de données des piles
 * generic_stack_t
 */
void generic_stack_destroy (generic_stack_t *stack);

/*! \brief Ajouter un élément de type <b>void*</b> sur la pile
 *
 * Etant donné un pointeur vers une pile (représentée par une liste
 * chaînée) et une valeur de type <b>void*</b>, ajoute un nouvel
 * élément à la pile (en "haut" de la pile) avec cette valeur
 *
 * @param stack est un pointeur vers la structure de données
 * generic_stack_t
 *
 * @pram data une valeur de type <b>void*</b> à ajouter à la pile
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour OK, et
 * -1 en cas de problème mémoire)
 */
int generic_stack_push(generic_stack_t *stack, const void *data);

/*! \brief Supprime un élément de type <b>void*</b> du haut de la pile
 *
 * Etant donné un pointeur vers une pile (représentée par une liste
 * chaînée), supprime l'élément en haut de la pile (le dernier à avoir
 * été inséré)
 *
 * @param stack est un pointeur vers la structure de données generic_stack_t
 *
 * @pram data un pointeur sur type <b>void*</b> qui permet de
 * récupérer la valeur de l'élément supprimer de la pile
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour OK, et
 * -1 en cas de problème mémoire)
 */
int generic_stack_pop(generic_stack_t *stack, void **data);

/*! \brief Récupère la valeur de type <b>void*</b> du haut de la pile
 *
 * Etant donné un pointeur vers une pile (représentée par une liste
 * chaînée), récupère la valeur de l'élément au dessus de la pile sans
 * le supprimer.
 *
 * @param stack est un pointeur vers la structure de données
 * generic_stack_t
 *
 * @return une valeur de <b>void*</b> de l'élément en haut de la
 * pile.
 */
void* generic_stack_peek (generic_stack_t *stack);

/*! \brief Accesseur sur la taille d'une pile
 *
 * Etant donné un pointeur vers une pile (représentée par une liste
 * chaînée), retourne la valeur du champ size
 *
 * @param stack est un pointeur vers la structure de données generic_stack_t
 *
 * @return un entier indiquant la taille de la pile
 */
int generic_stack_size (generic_stack_t *stack);

#endif
