/*****************************************************************************
*                                                                            *
*  -------------------------------- bfs.c ---------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>

#include "generic_graph.h"
#include "generic_graphalg.h"
#include "generic_list.h"
#include "generic_queue.h"

/*****************************************************************************
*                                                                            *
*  --------------------------------- bfs ----------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bfs(generic_graph_t *graph,
		generic_bfsvertex_t *start)
{

  generic_queue_t queue;

  generic_adjlist_t *adjlist;
  generic_adjlist_t *clr_adjlist;

  generic_bfsvertex_t *clr_vertex;
  generic_bfsvertex_t *adj_vertex;

  generic_list_elmt_t *element;
  generic_list_elmt_t *member;

  /* Initialize all of the vertices in the graph. */
  for (element = generic_list_head(&graph->adjlists);
       element != NULL;
       element = generic_list_next(element))
    {
      generic_adjlist_t* tempData = generic_list_data(element);
      clr_vertex = tempData->vertex;

      if (graph->compare(clr_vertex, start) == 0) {
	/* Initialize the start vertex. */
	clr_vertex->color = gray;
      }
      else {
	/* Initialize vertices other than the start vertex. */
	clr_vertex->color = white;
      }
    }

  /* Initialize the queue with the adjacency list of the start vertex. */
  generic_queue_init(&queue, NULL, NULL, NULL);

  if (generic_graph_adjlist(graph, start, &clr_adjlist) != 0) {
    generic_queue_destroy(&queue);
    return -1;
  }

  if (generic_queue_enqueue(&queue, clr_adjlist) != 0) {
    generic_queue_destroy(&queue);
    return -1;
  }

  /* Perform breadth-first search. */

  while (generic_queue_size(&queue) > 0) {
    adjlist = generic_queue_peek(&queue);

    /* Traverse each vertex in the current adjacency list. */
    for (member = generic_list_head(&adjlist->adjacent);
	 member != NULL;
	 member = generic_list_next(member))
      {
	adj_vertex = generic_list_data(member);

	/* Determine the color of the next adjacent vertex. */
	if (generic_graph_adjlist(graph, adj_vertex, &clr_adjlist) != 0) {
	  generic_queue_destroy(&queue);
	  return -1;
	}

	clr_vertex = clr_adjlist->vertex;

	/* Color each white vertex gray and enqueue its adjacency list. */
	if (clr_vertex->color == white) {
         clr_vertex->color = gray;
         if (generic_queue_enqueue(&queue, clr_adjlist) != 0) {
            generic_queue_destroy(&queue);
            return -1;
         }
	}
      }

    /* Dequeue the current adjacency list and color its vertex black. */
    if (generic_queue_dequeue(&queue, (void **)&adjlist) == 0) {
      ((generic_bfsvertex_t *)adjlist->vertex)->color = black;
    }
    else {
      generic_queue_destroy(&queue);
      return -1;
    }
  }

  generic_queue_destroy(&queue);

  return 0;
}
