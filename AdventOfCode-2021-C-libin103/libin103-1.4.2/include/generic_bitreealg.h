/*! \file generic_bitreealg.h
 *
 * \brief Fonctions de parcours d'arbres binaires en version générique
 * (contenant des valeurs de type <b>void*</b>)
 *
 */

/*****************************************************************************
*                                                                            *
*  ------------------------------ traverse.h ------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef GENERIC_BITREEALG_H
#define GENERIC_BITREEALG_H

#include "generic_bitree.h"
#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

/*! \brief Parcours d'arbre binaire suivant l'ordre préfixe
 *
 * Fonction récursive qui parcours l'arbre binaire dans l'ordre préfixe
 * 1. Stock la valeur du noeud dans la liste
 * 2. Appel récursif sur fils gauche si non NULL
 * 3. Appel récursif sur fils droit si non NULL
 *
 * @param node pointeur vers la racine de l'arbre
 *
 * @param list liste chaînée des valeurs des noeuds de l'arbre dans
 * l'ordre du parcours
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 pour erreur)
 */
int generic_preorder(generic_bitreenode_t *node, generic_list_t *list);

/*! \brief Parcours d'arbre binaire suivant l'ordre infixe
 *
 * Fonction récursive qui parcours l'arbre binaire dans l'ordre infixe
 * 1. Appel récursif sur fils gauche si non NULL
 * 2. Stock la valeur du noeud dans la liste
 * 3. Appel récursif sur fils droit si non NULL
 *
 * @param node pointeur vers la racine de l'arbre
 *
 * @param list liste chaînée des valeurs des noeuds de l'arbre dans
 * l'ordre du parcours
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 pour erreur)
 */
int generic_inorder(generic_bitreenode_t *node, generic_list_t *list);

/*! \brief Parcours d'arbre binaire suivant l'ordre postfixe
 *
 * Fonction récursive qui parcours l'arbre binaire dans l'ordre postfixe
 * 1. Appel récursif sur fils gauche si non NULL
 * 2. Appel récursif sur fils droit si non NULL
 * 3. Stock la valeur du noeud dans la liste
 *
 * @param node pointeur vers la racine de l'arbre
 *
 * @param list liste chaînée des valeurs des noeuds de l'arbre dans
 * l'ordre du parcours
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 pour erreur)
 */
int generic_postorder(generic_bitreenode_t *node, generic_list_t *list);

#endif
