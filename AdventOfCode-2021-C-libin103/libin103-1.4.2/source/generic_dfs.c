/*****************************************************************************
*                                                                            *
*  -------------------------------- dfs.c ---------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>

#include "generic_graph.h"
#include "generic_graphalg.h"
#include "generic_list.h"

/*****************************************************************************
*                                                                            *
*  ------------------------------- dfs_main -------------------------------  *
*                                                                            *
*****************************************************************************/

static int generic_dfs_aux(generic_graph_t *graph,
			   generic_adjlist_t *adjlist,
			   generic_list_t *ordered)
{

  generic_adjlist_t *clr_adjlist;

  generic_dfsvertex_t *clr_vertex;
  generic_dfsvertex_t *adj_vertex;

  generic_list_elmt_t *member;

  /* Color the vertex gray and traverse its adjacency list. */

  ((generic_dfsvertex_t *)adjlist->vertex)->color = gray;

  for (member = generic_list_head(&adjlist->adjacent);
       member != NULL;
       member = generic_list_next(member))
    {
      /* Determine the color of the next adjacent vertex. */
      adj_vertex = generic_list_data(member);

      if (generic_graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0) {
	return -1;
      }
      clr_vertex = clr_adjlist->vertex;

      /* Move one vertex deeper when the next adjacent vertex is white. */
      if (clr_vertex->color == white) {
	if (generic_dfs_aux(graph, clr_adjlist, ordered) != 0) {
         return -1;
	}
      }
    }

  /* Color the current vertex black and make it first in the list. */
  ((generic_dfsvertex_t *)adjlist->vertex)->color = black;

  if (generic_list_ins_next(ordered, NULL, (generic_dfsvertex_t *)adjlist->vertex) != 0) {
   return -1;
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------------- dfs ---------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_dfs(generic_graph_t *graph,
		generic_list_t *ordered)
{

  generic_dfsvertex_t *vertex;
  generic_list_elmt_t *element;

  /* Initialize all of the vertices in the graph. */
  for (element = generic_list_head(&graph->adjlists);
       element != NULL;
       element = generic_list_next(element))
    {
      vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;
      vertex->color = white;
    }

  /* Perform depth-first search. */
  generic_list_init(ordered, NULL, NULL, NULL);

  for (element = generic_list_head(&graph->adjlists);
       element != NULL; element = generic_list_next(element))
    {
      /* Ensure that every component of unconnected graphs is searched. */
      vertex = ((generic_adjlist_t *)generic_list_data(element))->vertex;

      if (vertex->color == white) {
	if (generic_dfs_aux(graph, (generic_adjlist_t *)generic_list_data(element), ordered) != 0) {
	  generic_list_destroy(ordered);
	  return -1;
	}
      }
    }

  return 0;
}
