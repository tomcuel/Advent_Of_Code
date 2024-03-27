/*! \file integer_uf.h
 *
 * \brief structure de données union-find contenant des valeurs de
 * type <b> int </b>.
 *
 * Une structure d'union-find est un ensemble de valeurs entières
 * organisé en classe d'équivalence. La structure est représentée par
 * un arbre avec plusieurs racines. Chaque racine est le représentant
 * de la classe d'équivalence.
 */

#ifndef INTEGER_UNION_FIND_H
#define INTEGER_UNION_FIND_H

#include <stdbool.h>

/*! \brief Type de données pour représenter un élément de la structure
 * union-find contenant des valeurs de type <b> int </b>
 *
 * Une structure union-find est structure arborescente avec
 * possiblement plusieurs racines. Chaque élément de l'arbre contient
 * une valeur, un pointeur vers son parent et une valeur indiquant sa
 * profondeur dans l'arbre.
 */
typedef struct integer_uf_elm {
  struct integer_uf_elm *parent; /**< pointeur vers le parent du noeud */
  int value; /**< valeur stockée dans le noeud */
  int depth; /**< profondeur dans l'arbre */
} integer_uf_elm_t;

/*! \brief Type de données pour représenter la structure union-find
 * contenant des valeurs de type <b> int </b>
 *
 * Une structure union-find est structure arborescente représentée par
 * un tableau dont les éléments sont de type <b>integer_uf_elm_t</b>,
 * une valeur représentant la taille du tableau et une valeur
 * représentant le nombre de classes d'équivalence.
 */
typedef struct integer_uf {
  integer_uf_elm_t** forest; /**< tableau des éléments de l'ensemble */
  int size; /**< nombre d'éléments maximal dans l'ensemble */
  int components; /**< nombre de classes d'équivalence */
} integer_uf_t;

/*****************************************************************************
 *                                                                            *
 *  --------------------------- Public Interface ---------------------------  *
 *                                                                            *
 *****************************************************************************/

/*! \brief Initialisation d'une nouvelle structure union-find
 *
 * Alloue la mémoire du tableau en fonction du nombres d'éléments
 * maximal de l'ensemble considéré. Chaque élément du tableau est
 * initialisé à NULL. Le nombre de classe d'équivalence est fixée à
 * 0. Initiallement, on considère qu'aucun élément de l'ensemble n'a
 * été ajouté.
 *
 * @param dset est un pointeur vers la structure de données union-find
 * integer_uf_t
 *
 * @param size nombre maximal d'éléments dans l'ensemble
 */
void integer_uf_init (integer_uf_t *dset, int size);

/*! \brief Destruction d'une structure union-find
 *
 * Parcours du tableau pour désallouer chaque élément, désalloue le
 * tableau, et fixe à zéro la taille et le nombre de classes
 * d'équivalence.
 *
 * @param dset est un pointeur vers la structure de données union-find
 * integer_uf_t
 */
void integer_uf_destroy(integer_uf_t *dset);

/*! \brief Ajout d'un nouvel élément dans une structure union-find
 *
 * La valeur d'un élément de l'ensemble est associé à l'indice du
 * tableau. Alloue en position value alloue la mémoire d'une structure
 * de données <b>integer_uf_elm_t</b>. Considère cet élément comme une
 * nouvelle classe d'équivalence, fixe la valeur stockée à la valeur
 * value, considère que cet élément n'a pas de parent et une
 * profondeur de zéro.
 *
 * @param dset est un pointeur vers la structure de données union-find
 * integer_uf_t
 *
 * @param value est un entier représentant la nouvelle valeur à
 * ajouter dans l'ensemble.
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 pour erreur mémoire)
 */
int integer_uf_add_element(integer_uf_t *dset,
			   int value);

/*! \brief Cherche le représentant d'une classe d'équivalence.
 *
 * Etant donné une valeur de l'ensemble, retourne le représentant de
 * la classe d'équivalence * à laquelle value appartient. Durant la
 * recherche du représentant, l'algorithme applique la technique de la
 * compression de chemin.
 *
 * @param dset est un pointeur vers la structure de données union-find
 * integer_uf_t
 *
 * @param value est un élément de l'ensemble
 *
 * @param result renvoie la structure de données de type
 * <b>integer_uf_elm_t*</b> associée au représentant de la classe
 * d'équivalence. result vaut NULL si value n'est pas dans l'ensemble.
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 si value n'est pas dans l'ensemble)
 */
int integer_uf_find(integer_uf_t *dset,
		    int value,
		    integer_uf_elm_t** result);

/*! \brief Associe deux éléments dans une même classe d'équivalence.
 *
 * Etant donné deux valeurs de l'ensemble, considère que ces deux
 * éléments sont dans une même classe d'équivalence en désigant l'une
 * d'entre elles comme parent de l'autre. L'algorithme applique la
 * méthode fusion par rang ou par profondeur pour limité
 * l'accroissement de l'arbre associé à une même classe d'équivalence.
 *
 * @param dset est un pointeur vers la structure de données union-find
 * integer_uf_t
 *
 * @param value1 est un élément de l'ensemble
 *
 * @param value2 est un élément de l'ensemble
 *
 * @return valeur entière indiquant si tout s'est bien passé (0 pour
 * ok, -1 si value1 ou value2 n'est pas dans l'ensemble)
 */
int integer_uf_union(integer_uf_t *dset,
		     int value1,
		     int value2);

/*! \brief Prédicat pour tester si deux éléments sont dans la même
 * classe d'équivalence
 *
 * Etant données deux valeurs de l'ensemble, teste si les
 * représentants des classes d'équivalence sont les mêmes
 *
 * @param dset est un pointeur vers la structure de données
 * integer_uf_t
 *
 * @param value1 un élément de l'ensemble
 *
 * @param value2 un élément de l'ensemble
 *
 * @return un booléen indiquant vrai si value1 et value2 sont dans la
 * même classe d'équivalence
 */
bool integer_uf_are_connected (integer_uf_t *dset,
			       int value1,
			       int value2);

/*! \brief Accesseur sur la taille maximale de l'ensemble
 *
 * Etant donné un pointeur vers un structure d'union-find, retourne la
 * valeur du champ size
 *
 * @param dset est un pointeur vers la structure de données
 * integer_uf_t
 *
 * @return un entier indiquant la taille maximale de l'ensemble
 */
int integer_uf_size (integer_uf_t *dset);

/*! \brief Accesseur sur le nombre de classes d'équivalence
 *
 * Etant donné un pointeur vers un structure d'union-find, retourne la
 * valeur du champ components
 *
 * @param dset est un pointeur vers la structure de données
 * integer_uf_t
 *
 * @return un entier indiquant le nombre de classes d'équivalence
 */
int integer_uf_components (integer_uf_t *dset);


#endif
