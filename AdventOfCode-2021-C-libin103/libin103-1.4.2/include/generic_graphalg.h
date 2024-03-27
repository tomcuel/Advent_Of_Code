/*! \file generic_graphalg.h
 *
 * \brief Liste d'algorithmes sur les graphes contenant des valeurs de type <b>void*</b>
 *
 * 1. Parcours de graphes en profondeur d'abord en deux versions
 * 2. Parcours de graphes en largeur d'abord en deux versions
 * 3. Algorithme de Kruskal: arbre couvrant de point minimal
 * 4. Algorithme de Dijkstra: plus court chemin pour un graphe à pondération positive
 *
 * @remarks: ces fonctions n'ont pas été très bien testées et sont
 * encore en développement. A ne pas utiliser !!!
 *
 */


/*****************************************************************************
*                                                                            *
*  ------------------------------ graphalg.h ------------------------------  *
*                                                                            *
*****************************************************************************/

#ifndef GENERIC_GRAPHALG_H
#define GENERIC_GRAPHALG_H

#include "generic_graph.h"
#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  Define a structure for vertices in depth-first search.                    *
*                                                                            *
*****************************************************************************/

typedef struct generic_dfsvertex_ {
  void *data;
  vertexcolor_t color;
} generic_dfsvertex_t;

/*****************************************************************************
*                                                                            *
*  Define a structure for vertices in breadth-first search.                  *
*                                                                            *
*****************************************************************************/

typedef struct generic_bfsvertex_ {
  void *data;
  vertexcolor_t color;
} generic_bfsvertex_t;

/*****************************************************************************
*                                                                            *
*  Define a structure for vertices in minimum spanning trees.                *
*                                                                            *
*****************************************************************************/

typedef struct generic_mstvertex_ {
  void *data;
  double weight;
  vertexcolor_t color;
  double key;
  struct generic_mstvertex_  *parent;
} generic_mstvertex_t;

/*****************************************************************************
*                                                                            *
*  Define a structure for vertices in shortest-path problems.                *
*                                                                            *
*****************************************************************************/

typedef struct generic_pathvertex_ {
  void *data;
  double weight;
  vertexcolor_t color;
  double d;
  struct generic_pathvertex_ *parent;
} generic_pathvertex_t;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/


int generic_dfs(generic_graph_t *graph,
		generic_list_t *ordered);


int generic_bfs(generic_graph_t *graph,
		generic_bfsvertex_t *start); 	/* or integer_list_t ?? */

int generic_mst(generic_graph_t *graph,
		const generic_mstvertex_t *start,
		generic_list_t *span,
		int (*compare)(const void *key1, const void *key2));

int generic_shortest(generic_graph_t *graph,
		     const generic_pathvertex_t *start,
		     generic_list_t *paths,
		     int (*match) (const void *key1, const void *key2));

#endif
