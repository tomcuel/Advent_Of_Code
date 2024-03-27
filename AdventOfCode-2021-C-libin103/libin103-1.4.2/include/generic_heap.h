/*! \file generic_heap.h
 *
 * \brief structure de données tas contenant des valeurs de type <b>
 * void* </b>. Le tas peut être configuré pour être tas-min ou
 * tas-max.
 *
 */

/*****************************************************************************
*                                                                            *
*  -------------------------------- heap.h --------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef GENERIC_HEAP_H
#define GENERIC_HEAP_H

/*****************************************************************************
*                                                                            *
*  Define a structure for heaps.                                             *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour représenter le drapeau pour choisir un
 *    tas-min ou un tas-max avec des données de type <b>void*</b>. Le
 *    choix du type de tas est réalisé grâce à la fonction de
 *    comparaison.
 */
typedef struct generic_heap_t {
  int size; /**< nombre d'éléments dans le tableau */
  int (*compare)(const void *key1, const void *key2); /**< fonction de
							 comparaison
							 entre deux
							 éléments du
							 tas */
  void* (*build)(const void *data); /**< fonction de construction d'un
							 élément du tas */
  void (*destroy)(void *data);  /**< fonction de destruction d'un
							 élément du
							 tas */
  void **tree; /**< tableau représentant le tas */
} generic_heap_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

/*! \brief Initialisation d'un nouveau tas générique
 *
 * Met à zéro la taille et à NULL le pointeur sur le tableau. Cette
 * fonction nécessite une fonction de comparaison des éléments du tas,
 * une fonction de construction d'un élément du tas et une fonction de
 * destruction des éléments du tas.
 *
 * @param list est un pointeur vers la structure de données de tas
 *  generic_heap_t
 *
 * @param compare fonction permettant la comparaison de deux éléments
 * du tas
 *
 * @param build fonction permettant la construction d'une valeur du
 * tas
 *
 * @param destroy fonction permettant la destruction d'une valeur du
 * tas
 */
void generic_heap_init(generic_heap_t *heap,
		       int (*compare)(const void *key1, const void *key2),
		       void* (*build)(const void *data),
		       void (*destroy)(void *data));

/*! \brief Destruction d'un tas
 *
 * Met à zéro la taille et désalloue le tableau représentant le tas
 *
 * @param heap est un pointeur vers la structure de données de tas
 * generic_heap_t
 */
void generic_heap_destroy(generic_heap_t *heap);

/*! \brief Insertion d'un nouvel élément dans le tas
 *
 * Augmente la taille du tableau pour ajouter un nouvel élément. Puis,
 * à partir du nouvel élément remonte jusqu'à la racine en faisant des
 * permutations pour maintenir la propriété de tas-min ou tas-max
 *
 * @param heap est un pointeur vers la structure de données de tas
 * generic_heap_t
 *
 * @param data valeur de type <b>void*</b> à ajouter dans le tas
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour ok, -1
 * pour problème)
 */
int generic_heap_insert(generic_heap_t *heap, const void *data);

/*! \brief Extraction du plus petit (tas-min) ou du plus grand
 *  (tas-max) élément contenu dans le tas, i.e., à la racine.
 *
 * Affecte au paramètre data, la valeur du premier élément du
 * tableau. Echange la valeur du dernier élément avec la racine, puis
 * réduit la taille du tableau et descent de la racine jusqu'à une
 * feuille en faisant des permutations pour maintenir la propriété de
 * tas-min ou tas-max.
 *
 * @param heap est un pointeur vers la structure de données de tas
 * generic_heap_t
 *
 * @param data valeur de type <b>void*</b> extraite du tas
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour ok, -1
 * pour problème)
 */
int generic_heap_extract(generic_heap_t *heap, void **data);

/*! \brief Accesseur pour récupérer le nombre d'éléments dans le tas
 *
 * @param heap est un pointeur vers la structure de données de tas
 * generic_heap_t
 *
 * @return le nombre d'éléments dans le tas
 */
int generic_heap_size(generic_heap_t *heap);

/*! \brief Accesseur pour modifier une valeur du tas
 *
 * Mdofication en place d'une valeur contenue dans le tas à une
 * position donnée. La propriété du tas est assurée. Cette fonction
 * est utilisée dans l'algorithme de Dijkstra.
 *
 * @param heap est un pointeur vers la structure de données de tas
 * generic_heap_t
 *
 * @param position indique l'indice de la case du tableau pour
 * laquelle on veut changer la valeur
 *
 * @param data pointeur vers la nouvelle valeur à ajouter dans le tas
 *
 * @return un entier indiquant si tout s'est bien passé (0 pour ok, -1
 * pour problème)
 */
int generic_heap_change_key (generic_heap_t *heap, int position, void *data);

#endif
