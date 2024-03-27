/*****************************************************************************
*                                                                            *
*  ------------------------------ shortest.c ------------------------------  *
*                                                                            *
*****************************************************************************/

#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "generic_graph.h"
#include "generic_graphalg.h"
#include "generic_list.h"
#include "generic_set.h"

/*****************************************************************************
*                                                                            *
*  --------------------------------- relax --------------------------------  *
*                                                                            *
*****************************************************************************/

static void generic_relax(generic_pathvertex_t *u,
			  generic_pathvertex_t *v,
			  double weight)
{

  /* Relax an edge between two vertices u and v. */

  if (v->d > u->d + weight) {
    v->d = u->d + weight;
    v->parent = u;
  }
}

/*****************************************************************************
*                                                                            *
*  ------------------------------- shortest -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_shortest(generic_graph_t *graph,
		     const generic_pathvertex_t *start,
		     generic_list_t *paths,
		     int (*compare) (const void *key1, const void *key2))
{

  generic_adjlist_t *adjlist = NULL;

  generic_pathvertex_t *pth_vertex;
  generic_pathvertex_t *adj_vertex;

  generic_list_elmt_t *element;
  generic_list_elmt_t *member;

  double minimum;
  bool found;
  int i;

  /* Initialize all of the vertices in the graph. */

  found = false;

  for (element = generic_list_head(&graph->adjlists);
       element != NULL;
       element = generic_list_next(element))
    {
      pth_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

      if (compare(pth_vertex, start) == 0) {
	/* Initialize the start vertex. */
	pth_vertex->color = white;
	pth_vertex->d = 0;
	pth_vertex->parent = NULL;
	found = true;
      }
      else {
	/* Initialize vertices other than the start vertex. */
	pth_vertex->color = white;
	pth_vertex->d = DBL_MAX;
	pth_vertex->parent = NULL;
      }
    }

  /* Return if the start vertex was not found. */
  if (found == false) {
    return -1;
  }

  /* Use Dijkstra's algorithm to compute shortest paths from the start vertex. */

  i = 0;

  while (i < generic_graph_vcount(graph)) {
   /* Select the white vertex with the smallest shortest-path estimate. */
   minimum = DBL_MAX;

   for (element = generic_list_head(&graph->adjlists); element != NULL;	element = generic_list_next(element)) {

     pth_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

     if (pth_vertex->color == white && pth_vertex->d < minimum) {
       minimum = pth_vertex->d;
       adjlist = generic_list_data(element);
     }
   }

   assert (adjlist != NULL);

   /* Color the selected vertex black. */
   ((generic_pathvertex_t *)adjlist->vertex)->color = black;

   /* Traverse each vertex adjacent to the selected vertex. */
   for (member = generic_list_head(&adjlist->adjacent); member != NULL;	member = generic_list_next(member)) {
     adj_vertex = generic_list_data(member);

     /* Find the adjacent vertex in the list of adjacency-list structures. */
     for (element = generic_list_head(&graph->adjlists); element != NULL; element = generic_list_next(element)) {
       pth_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

       if (compare(pth_vertex, adj_vertex) == 0) {
	 /* Relax the adjacent vertex in the list of adjacency-list
	  *  structures. */
	 if (adjlist != NULL) {
	   generic_relax(adjlist->vertex, pth_vertex, adj_vertex->weight);
	 }
       }
     }
   }

   /* Prepare to select the next vertex. */
   i++;
  }

  /* Load the vertices with their path information into a list. */

  generic_list_init(paths, NULL, NULL, NULL);

  for (element = generic_list_head(&graph->adjlists);
       element != NULL;
       element = generic_list_next(element))
    {
   /* Load each black vertex from the list of adjacency-list structures. */
   pth_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

   if (pth_vertex->color == black)
     {
       if (generic_list_ins_next(paths, generic_list_tail(paths), pth_vertex) != 0)
	 {
	   generic_list_destroy(paths);
	   return -1;
	 }
     }
    }

  return 0;
}
