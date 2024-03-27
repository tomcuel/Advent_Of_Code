/*****************************************************************************
*                                                                            *
*  -------------------------------- graph.c -------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "generic_graph.h"
#include "generic_list.h"
#include "generic_set.h"

#ifdef DEBUG
#pragma message "activate debug mode generic_graph"
#endif

/*
 * Probleme d'allocation mémoire dans la liste générique à cause
 * de la non définition d'une fonction build.
 * A gérer pour utiliser cette version spécialisée de la bibliothèque
 */

/* void* generic_build (const void *adjlist_data) { */
/* #ifdef DEBUG */
/*   printf ("\t\tEnter %s\n", __func__); */
/* #endif */

/*   assert (adjlist_data != NULL); */
/*   generic_adjlist_t  *new_adjlist_data = malloc(sizeof(generic_adjlist_t)); */
/*   assert(new_adjlist_data!=NULL); */

/*   new_adjlist_data->vertex = ((generic_adjlist_t *)adjlist_data)->vertex; */
/*   /\* Recopy the set of adjacent vertices *\/ */
/*   generic_list_elmt_t* elt; */
/*   for (elt = generic_list_head(&((generic_adjlist_t *)adjlist_data)->adjacent); */
/*        elt != NULL; */
/*        elt = generic_list_next (elt)) */
/*     { */
/*       generic_set_insert (&new_adjlist_data->adjacent, */
/* 			  generic_list_data(elt)); */
/*     } */

/*   /\* new_adjlist_data->adjacent; *\/ */
/*   return (void*) new_adjlist_data; */
/* } */

/*****************************************************************************
*                                                                            *
*  ------------------------------ graph_init ------------------------------  *
*                                                                            *
*****************************************************************************/

void generic_graph_init(generic_graph_t *graph,
			int (*compare)(const void *key1, const void *key2),
			void* (*build) (const void *data),
			void (*destroy)(void *data))
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  /* Initialize the graph. */

  graph->vcount = 0;
  graph->ecount = 0;
  graph->compare = compare;
  graph->build = build;
  graph->destroy = destroy;

  /* Initialize the list of adjacency-list structures. */
  generic_list_init(&graph->adjlists, compare, NULL, free);
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- graph_destroy ----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_graph_destroy(generic_graph_t *graph)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_adjlist_t *adjlist = NULL;

  /* Remove each adjacency-list structure and destroy its adjacency list. */
  while (generic_list_size(&graph->adjlists) > 0) {

    if (generic_list_rem_next(&graph->adjlists, NULL, (void **)&adjlist) == 0) {
      generic_set_destroy(&adjlist->adjacent);

      if (graph->destroy != NULL) {
         graph->destroy(adjlist->vertex);
      }

      /* Why this line of code generates an error ? */
      free(adjlist);
    }
  }

  /* Destroy the list of adjacency-list structures, which is now empty. */
  generic_list_destroy(&graph->adjlists);

  /* No operations are allowed now, but clear the structure as a precaution. */
  memset(graph, 0, sizeof(generic_graph_t));
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_ins_vertex ---------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_ins_vertex(generic_graph_t *graph,
			     const void *data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element = NULL;
  generic_adjlist_t *adjlist = NULL;
  int retval = 0;

  /* Do not allow the insertion of duplicate vertices. */
  for (element = generic_list_head(&graph->adjlists); element != NULL; element = generic_list_next(element)) {
    if (graph->compare(data, ((generic_adjlist_t *)generic_list_data(element))->vertex) == 0) {
      return 1;
    }
  }

  /* Insert the vertex. */
  adjlist = malloc(sizeof(generic_adjlist_t));
  if (adjlist == NULL) {
   return -1;
  }

  adjlist->vertex = graph->build(data);
  generic_set_init(&adjlist->adjacent, graph->compare, graph->build, graph->destroy);

  /* Here we have a problem !!  the function build used here build
   * only a string not an generic_adjlist_t !!  so insert vertex is ok
   * but insert edges is not ok and generates segmentation fault !!!
   * Solution : expose the structure of generic_adjlist_t in the test
   * programs to write the corrected build function even for primitive types !!
   *
   * The solution was to remove the build function in the list to avoid such problem !!
   */
  retval = generic_list_ins_next(&graph->adjlists,
				 generic_list_tail(&graph->adjlists), (void*)adjlist);

#ifdef DEBUG
  printf ("\t\t\t%s: generic_list_size(&graph->adjlists)=%d\n", __func__, generic_list_size(&graph->adjlists));
  printf ("\t\t\t%s: generic_set_size(tail(&graph->adjlists))=%d\n", __func__, generic_set_size(&((generic_adjlist_t*)generic_list_tail(&graph->adjlists))->adjacent));
#endif


  if (retval != 0) {
    return retval;
  }
  /* Remove local element as it has been copy in the list */
  /* free(adjlist); */

  /* Adjust the vertex count to account for the inserted vertex. */
  graph->vcount++;

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- graph_ins_edge ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_ins_edge(generic_graph_t *graph,
			   const void *data1,
			   const void *data2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element1 = NULL;
  generic_list_elmt_t *element2 = NULL;
  int retval = 0;

  /* Do not allow insertion of an edge without both its vertices in the graph. */
  for (element2 = generic_list_head(&graph->adjlists); element2 != NULL; element2 = generic_list_next(element2)) {
    if (graph->compare(data2, ((generic_adjlist_t *)generic_list_data(element2))->vertex) == 0) {
      break;
    }
  }

#ifdef DEBUG
  printf ("\t\t\t%s END search data2\n", __func__);
#endif

  /* data2 is not in the graph !!*/
  if (element2 == NULL) {
    return -1;
  }

  for (element1 = generic_list_head(&graph->adjlists); element1 != NULL; element1 = generic_list_next(element1)) {
    if (graph->compare(data1, ((generic_adjlist_t *)generic_list_data(element1))->vertex) == 0) {
      break;
    }
  }

#ifdef DEBUG
  printf ("\t\t\t%s END search data1\n", __func__);
#endif

  /* data1 is not in the graph !!*/
  if (element1 == NULL) {
    return -1;
  }

  /* Insert the second vertex into the adjacency list of the first vertex. */
  retval = generic_set_insert(&((generic_adjlist_t *)generic_list_data(element1))->adjacent, data2);

#ifdef DEBUG
  printf ("\t\t\t%s END insert data2 in adjacent set of data1\n", __func__);
#endif


  if (retval != 0) {
   return retval;
  }

  /* Adjust the edge count to account for the inserted edge. */
  graph->ecount++;

  return 0;
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_rem_vertex ---------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_rem_vertex(generic_graph_t *graph,
			     void **data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element = NULL;
  generic_list_elmt_t *temp = NULL;
  generic_list_elmt_t *prev = NULL;
  generic_adjlist_t *adjlist = NULL;
  bool found;

  /* Traverse each adjacency list and the vertices it contains. */
  prev = NULL;
  found = false;

  for (element = generic_list_head(&graph->adjlists); element != NULL;
       element = generic_list_next(element)) {

    /* Do not allow removal of the vertex if it is in an adjacency list. */
    if (generic_set_is_member(&((generic_adjlist_t *)generic_list_data(element))->adjacent, *data)) {
      return -1;
    }
   /* Keep a pointer to the vertex to be removed. */
   if (graph->compare(*data, ((generic_adjlist_t *)generic_list_data(element))->vertex) == 0)
     {
      temp = element;
      found = true;
   }

   /* Keep a pointer to the vertex before the vertex to be removed. */
   if (!found) {
      prev = element;
   }
  }

  /* Return if the vertex was not found. */
  if (!found) {
    return -1;
  }
  /* Do not allow removal of the vertex if its adjacency list is not empty. */
  if (generic_set_size(&((generic_adjlist_t *)generic_list_data(temp))->adjacent) > 0) {
   return -1;
  }

  /* Remove the vertex. */
  if (generic_list_rem_next(&graph->adjlists, prev, (void **)&adjlist) != 0) {
   return -1;
  }

  /* Free the storage allocated by the abstract data type. */
  *data = adjlist->vertex;
  free(adjlist);

  /* Adjust the vertex count to account for the removed vertex. */
  graph->vcount--;

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- graph_rem_edge ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_rem_edge(generic_graph_t *graph,
			   void *data1,
			   void **data2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element;

  /* Locate the adjacency list for the first vertex. */

  for (element = generic_list_head(&graph->adjlists); element != NULL;
       element = generic_list_next(element)) {

    if (graph->compare(data1, ((generic_adjlist_t *)generic_list_data(element))->vertex) == 0) {
      break;
    }
  }

  if (element == NULL) {
   return -1;
  }

  /* Remove the second vertex from the adjacency list of the first vertex. */
  if (generic_set_remove(&((generic_adjlist_t *)generic_list_data(element))->adjacent, data2) != 0) {
    return -1;
  }
  /* Adjust the edge count to account for the removed edge. */
  graph->ecount--;

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- graph_adjlist ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_adjlist(const generic_graph_t *graph,
			  const void *data,
			  generic_adjlist_t **adjlist)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element;
  /* generic_list_elmt_t *prev; */

  /* Locate the adjacency list for the vertex. */
  /* prev = NULL; */

  generic_list_t tempAdjlist = graph->adjlists;
  for (element = generic_list_head(&tempAdjlist); element != NULL;
       element = generic_list_next(element)) {

    if (graph->compare(data, ((generic_adjlist_t *)generic_list_data(element))->vertex) == 0) {
      break;
    }
    /* prev = element; */
  }

  /* Return if the vertex was not found. */
  if (element == NULL) {
   return -1;
  }
  /* Pass back the adjacency list for the vertex. */

  *adjlist = generic_list_data(element);

  return 0;
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_is_adjacent --------------------------  *
*                                                                            *
*****************************************************************************/

bool generic_graph_is_adjacent(const generic_graph_t *graph,
			      const void *data1,
			      const void *data2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *element;
  /* generic_list_elmt_t *prev; */

  /* Locate the adjacency list of the first vertex. */
  /* prev = NULL; */

  generic_list_t tempAdjlist = graph->adjlists;
  for (element = generic_list_head(&tempAdjlist); element != NULL;
       element = generic_list_next(element)) {

    if (graph->compare(data1, ((generic_adjlist_t *)generic_list_data(element))->vertex) == 0) {
      break;
    }

    /* prev = element; */
  }

  /* Return if the first vertex was not found. */
  if (element == NULL) {
   return false;
  }
/* Return whether the second vertex is in the adjacency list of the first. */

  return generic_set_is_member(&((generic_adjlist_t *)generic_list_data(element))->adjacent, data2);
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_adjlists -----------------------------  *
*                                                                            *
*****************************************************************************/

generic_list_t generic_graph_adjlists(generic_graph_t *graph)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (graph != NULL);
  return graph->adjlists;
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_vcount -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_vcount(generic_graph_t *graph)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (graph != NULL);
  return graph->vcount;
}

/*****************************************************************************
*                                                                            *
*  --------------------------- graph_ecount -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_graph_ecount(generic_graph_t *graph)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (graph != NULL);
  return graph->ecount;
}
