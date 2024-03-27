/*! \file generic_graph.h
 *
 * \brief Structure de données de graphes orientés ou non, pondérés ou
 * non, représentés par une liste d'adjacence.
 *
 * Une liste d'adjacence est représentée par une liste simplement
 * chainée (générique) dont les éléments sont des ensembles
 * (génériques) contenant des valeurs et un poids (pondération de
 * l'arc).
 *
 */

/*****************************************************************************
*                                                                            *
*  -------------------------------- graph.h -------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef GENERIC_GRAPH_H
#define GENERIC_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>

#include "generic_list.h"
#include "generic_set.h"

/*****************************************************************************
*                                                                            *
*  Define a structure for adjacency lists.                                   *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour représenter la liste des voisins d'un
 * sommet du graphe
 */
typedef struct generic_adjlist_ {
  void *vertex; /**< valeur définisssant un noeud voisin */
  generic_set_t adjacent; /**< liste des voisins */
} generic_adjlist_t;

/*****************************************************************************
*                                                                            *
*  Define a structure for graphs.                                            *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour représenter un graphe comme une liste
 * d'adjacence
 */
typedef struct generic_graph_ {
  int vcount;  /**< nombre de sommets */
  int ecount; /**< nombre d'arcs */
  int (*compare)(const void *key1, const void *key2); /**< fonction de
							 comparaison
							 entre deux
							 sommets du
							 graphe */
  void* (*build) (const void *data); /**< fonction de construction
							 d'un sommet
							 du graphe */
  void (*destroy)(void *data); /**< fonction de destruction d'un
							 sommet du
							 graphe */
  generic_list_t adjlists; /**< liste d'adjacence */

} generic_graph_t;

/*****************************************************************************
*                                                                            *
*  Define colors for vertices in graphs.                                     *
*                                                                            *
*****************************************************************************/

/*! \brief Type de données pour annoter les sommets lors d'un parcours
 * de graphe.
 */
typedef enum vertexcolor_ {
  white, /**< drapeau pour indiquer un sommet non considéré */
  gray, /**< drapeau pour indiquer un sommet en cours de traitement */
  black /**< drapeau pour indiquer un sommet qui a été traité */
} vertexcolor_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

/*! \brief Initialisation d'un nouveau graphe
 *
 * Met à zéro du nombre de sommets, du nombre d'arcs et initialisation
 * de la liste d'adjacence. Cette fonction nécessite une fonction de
 * comparaison des éléments du graphe, une fonction de construction
 * d'un élément du graphe et une fonction de destruction des éléments
 * du graphe.
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param compare fonction permettant la comparaison de deux sommets
 * du graphe
 *
 * @param build fonction permettant la construction d'un sommet du
 * graphe
 *
 * @param destroy fonction permettant la destruction d'un sommet du
 * graphe
 */
void generic_graph_init(generic_graph_t *graph,
			int (*compare)(const void *key1, const void *key2),
			void* (*build) (const void *data),
			void (*destroy)(void *data));

/*! \brief Destruction d'un graphe
 *
 * Parcours la liste d'adjacence (adjlists) puis parcours de
 * l'ensemble (adjacent) pour détruire tous les éléments de la liste
 * d'adjacence. Puis, mise à zéro du nombre de sommets, du nombre
 * d'arcs et à NULL la liste d'adjacence (adjlists)
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 */
void generic_graph_destroy(generic_graph_t *graph);


/*! \brief Insertion d'un nouveau sommet dans un graphe
 *
 * 1. Parcours la liste d'adjacence (adjlists) pour savoir si le
 * sommet est déjà présent ou pas
 * 2. Si pas présent, ajout d'un nouvel élément à la liste d'adjacence
 * (adjlists)
 * 3. Mise à jour du nombre de sommets dans le graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data nouveau noeud, représenté par une valeur de
 * type <b>void*</b>, à ajouter
 *
 * @return une valeur entière indiquant si tout s'est bien passé (0
 * pour ok, -1 pour erreur)
 */
int generic_graph_ins_vertex(generic_graph_t *graph,
			     const void *data);

/*! \brief Insertion d'un nouvel arc dans un graphe
 *
 * <b>Remarque:</b> par défaut on considère un arc directionnel donc
 * un graphe dirigé.
 *
 * 1. Vérifie que l'arc n'existe pas déjà
 * 2. Ajoute un nouveau voisin au sommet data1 avec la pondération
 * 3. Mise à jour du nombre d'arcs dans le graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data1 source de l'arc, représenté par une valeur de type
 * <b>void*</b>
 *
 * @param data2 destination de l'arc, représenté par une valeur de
 * type <b>void*</b>
 *
 * @return une valeur entière indiquant si tout s'est bien passé (0
 * pour ok, -1 pour erreur)
 */
int generic_graph_ins_edge(generic_graph_t *graph,
			   const void *data1,
			   const void *data2);

/*! \brief Suppression d'un sommet dans un graphe
 *
 * On peut supprimer un sommet que s'il n'a plus de voisins, c'est à dire,
 * plus d'arc entre lui et les autres sommets du graphe.
 *
 * 1. Vérifie que le sommet ne contient pas de voisins (adjlists vide)
 * 2. Si oui suppression du sommet
 * 3. Mise à jour du nombre de sommets dans le graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data sommet à supprimer
 *
 * @return une valeur entière indiquant si tout s'est bien passé (0
 * pour ok, -1 pour erreur)
 */
int generic_graph_rem_vertex(generic_graph_t *graph,
			     void **data);

/*! \brief Suppression d'un arc dans un graphe
 *
 * 1. Vérifie que l'arc existe
 * 2. Si oui suppression du sommet data2 dans la liste des voisins de data1
 * 3. Mise à jour du nombre de sommets dans le graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data1 source de l'arc, représenté par une valeur de type
 * <b>void*</b>
 *
 * @param data2 destination de l'arc, représenté par une valeur de
 * type <b>void*</b>
 *
 * @return une valeur entière indiquant si tout s'est bien passé (0
 * pour ok, -1 pour erreur)
 */
int generic_graph_rem_edge(generic_graph_t *graph,
			   void *data1,
			   void **data2);

/*! \brief Accesseur de la liste des voisins d'un sommet
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data sommet pour lequel la liste des voisins est voulue
 *
 * @return un pointeur vers une liste chainée de type
 * <b>generic_list_t</b> contenant la liste des voisins (sans
 * pondération)
 */
int generic_graph_adjlist(const generic_graph_t *graph,
			  const void *data,
			  generic_adjlist_t **adjlist);

/*! \brief Prédicat indiquant si deux sommets sont liés par un arc
 *
 * <b>Remarque</b>: on considère le lien directionnel entre deux
 * sommets.
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @param data1 sommet source
 *
 * @param data2 sommet destination
 *
 * @return une valeur booléenne indiquant s'il y a un arc entre
 * vertex1 et vertex2 (orienté).
 */
bool generic_graph_is_adjacent(const generic_graph_t *graph,
			       const void *data1,
			       const void *data2);


/*! \brief Accesseur de la liste d'ajdacence d'un graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @return un pointeur vers une liste chainée de type
 * <b>generic_list_t</b> contenant la liste des sommets et leurs voisins
 */
generic_list_t generic_graph_adjlists(generic_graph_t *graph);


/*! \brief Accesseur du nombre de sommets du graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @return valeur entière donnant le nombre de sommets du graphe
 */
int generic_graph_vcount(generic_graph_t *graph);

/*! \brief Accesseur du nombre d'arcs du graphe
 *
 * @param graph est un pointeur vers la structure de données de graphe
 * generic_graph_t
 *
 * @return valeur entière donnant le nombre d'arcs du graphe
 */
int generic_graph_ecount(generic_graph_t *graph);

#endif
