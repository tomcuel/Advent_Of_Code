/*****************************************************************************
*                                                                            *
*  --------------------------------- mst.c --------------------------------  *
*                                                                            *
*****************************************************************************/

#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "generic_graph.h"
#include "generic_graphalg.h"
#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  ---------------------------------- mst ---------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_mst(generic_graph_t *graph,
		const generic_mstvertex_t *start,
		generic_list_t *span,
		int (*compare)(const void *key1, const void *key2))
{

  generic_adjlist_t *adjlist = NULL;

  generic_mstvertex_t *mst_vertex;
  generic_mstvertex_t *adj_vertex;

  generic_list_elmt_t *element;
  generic_list_elmt_t *member;

  double minimum;
  bool found;
  int i;

  /* Initialize all of the vertices in the graph. */
  found = false;

  for (element = generic_list_head(&graph->adjlists); element != NULL; element = generic_list_next(element)) {
    mst_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

    if (compare(mst_vertex, start) == 0) {

      /* Initialize the start vertex. */
      mst_vertex->color = white;
      mst_vertex->key = 0;
      mst_vertex->parent = NULL;
      found = true;
    }
    else {
      /* Initialize vertices other than the start vertex. */
      mst_vertex->color = white;
      mst_vertex->key = DBL_MAX;
	mst_vertex->parent = NULL;
    }
  }

  /* Return if the start vertex was not found. */
  if (found == false) {
    return -1;
  }

  /* Use Prim's algorithm to compute a minimum spanning tree. */
  i = 0;

  while (i < generic_graph_vcount(graph)) {
    /* Select the white vertex with the smallest key value. */
    minimum = DBL_MAX;
   for (element = generic_list_head(&graph->adjlists); element != NULL; element = generic_list_next(element)) {
     mst_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;
     if (mst_vertex->color == white && mst_vertex->key < minimum) {
       minimum = mst_vertex->key;
       adjlist = generic_list_data(element);
     }
   }

   assert (adjlist != NULL);

   /* Color the selected vertex black. */
   ((generic_mstvertex_t *)adjlist->vertex)->color = black;

   /* Traverse each vertex adjacent to the selected vertex. */
   for (member = generic_list_head(&adjlist->adjacent);
	member != NULL;
	member = generic_list_next(member))
     {
      adj_vertex = generic_list_data(member);

      /* Find the adjacent vertex in the list of adjacency-list structures. */
      for (element = generic_list_head(&graph->adjlists);
	   element != NULL; element = generic_list_next(element))
	{
	  mst_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

         if (compare(mst_vertex, adj_vertex) == 0) {
            /* Decide whether to change the key value and parent of the
	     *  adjacent vertex in the list of adjacency-list structures. */

            if (mst_vertex->color == white &&
		adj_vertex->weight < mst_vertex->key) {

               mst_vertex->key = adj_vertex->weight;
	       if (adjlist != NULL) {
		 mst_vertex->parent = adjlist->vertex;
	       }
            }

            break;
         }
      }
     }

   /* Prepare to select the next vertex. */
   i++;
  }

  /* Load the minimum spanning tree into a list. */

  generic_list_init(span, NULL, NULL, NULL);

  for (element = generic_list_head(&graph->adjlists); element != NULL; element = generic_list_next(element)) {

   /* Load each black vertex from the list of adjacency-list structures. */
      mst_vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;
      if (mst_vertex->color == black) {
	if (generic_list_ins_next(span, generic_list_tail(span), mst_vertex) != 0)
	  {
	    generic_list_destroy(span);
	    return -1;
	}
      }
    }

  return 0;
}
