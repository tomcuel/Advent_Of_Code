#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tap.h"
#include "graph.h"
#include "list.h"
#include "set.h"

void print_graph (integer_graph_t* graph) {

  generic_list_elmt_t* elem1 = generic_list_head(&(graph->adjlists));
  for (; elem1 != NULL; elem1 = generic_list_next(elem1)) {

    integer_adjlist_t* tempV = (integer_adjlist_t*)generic_list_data(elem1);
    printf ("Vertex %d: ", tempV->vertex);

    generic_list_elmt_t* elem2 = generic_list_head(&(tempV->adjacent));
    for (; elem2 != generic_list_tail(&(tempV->adjacent)); elem2 = generic_list_next(elem2)) {
      integer_adjlist_elmt_t *tempV2 = (integer_adjlist_elmt_t*)generic_list_data(elem2);
      printf ("%d->", tempV2->vertex);
    }
    if (generic_list_tail(&(tempV->adjacent)) != NULL) {
      integer_adjlist_elmt_t *tempV3 = (integer_adjlist_elmt_t*)generic_list_data(generic_list_tail(&(tempV->adjacent)));
      printf ("%d\n", tempV3->vertex);
    }
    else {
      printf ("\n");
    }
  }
}


int main (void) {
  plan(NO_PLAN);
  /* plan (19); */

  int code;
  integer_graph_t graph;
  integer_graph_init (&graph);

  for (int i = 101; i < 1000; i = i + 100) {
    code = integer_graph_ins_vertex(&graph, i);
    ok(code == 0, "Vertex insertion in empty graph");
  }

  code = integer_graph_ins_edge(&graph, 201, 101, 0.0);
  ok(code == 0, "Edge insertion with known vertices");

  code = integer_graph_rem_vertex(&graph, 401);
  ok(code == 0, "Vertex removal");

  print_graph (&graph);

  integer_graph_destroy (&graph);

  done_testing();

  return EXIT_SUCCESS;
}
