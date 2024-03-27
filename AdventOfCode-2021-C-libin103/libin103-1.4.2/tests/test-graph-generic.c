#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tap.h"
#include "generic_graph.h"
#include "generic_graphalg.h"

/* Shall define a new function to fit the specification with void* */
int compare (const void *a, const void *b) {
#ifdef DEBUG
  printf ("Enter %s in %s\n", __func__, __FILE__);
#endif
  const char* sa = a;
  const char* sb = b;
  return strcmp (sa, sb);
}

/* Shall define a new function to fit the specification with void* */
void* build (const void *data) {
#ifdef DEBUG
  printf ("Enter %s in %s\n", __func__, __FILE__);
#endif
  const char* sdata = data;
  int len = strlen(data);
  char *dest = malloc((len+1) * sizeof(char)); /* Do not forget the \0 value */
  /* By default strncpy return a pointer on dest */
  return strncpy (dest, sdata, len+1); /* Do not forget the \0 value */
}

int main (void) {

  plan(NO_PLAN);

  int code;

  generic_graph_t graph;

  generic_graph_init (&graph, compare, build, free);

  code = generic_graph_ins_vertex (&graph, "node1");
  ok (code == 0, "Vertex insertion in empty graph: Node1");

  code = generic_graph_ins_vertex (&graph, "node2");
  ok (code == 0, "Vertex insertion in non empty graph: Node2");

  code = generic_graph_ins_vertex (&graph, "node3");
  ok (code == 0, "Vertex insertion in non empty graph: Node3");

  code = generic_graph_ins_vertex (&graph, "node4");
  ok (code == 0, "Vertex insertion in non empty graph: Node4");

  code = generic_graph_ins_vertex (&graph, "node5");
  ok (code == 0, "Vertex insertion in non empty graph: Node5");

  code = generic_graph_ins_edge (&graph, "node1", "node2");
  ok (code == 0, "Edge insertion with knwon vertices: Node1->Node2");

  code = generic_graph_ins_edge (&graph, "node2", "node3");
  ok (code == 0, "Edge insertion with knwon vertices: Node2->Node3");

  code = generic_graph_ins_edge (&graph, "node1", "node3");
  ok (code == 0, "Edge insertion with knwon vertices: Node1->Node3");

  code = generic_graph_ins_edge (&graph, "node3", "node4");
  ok (code == 0, "Edge insertion with knwon vertices: Node3->Node4");

  code = generic_graph_ins_edge (&graph, "node1", "node5");
  ok (code == 0, "Edge insertion with knwon vertices: Node1->Node5");

  code = generic_graph_ins_edge (&graph, "node5", "node4");
  ok (code == 0, "Edge insertion with knwon vertices: Node5->Node4");

  code = generic_graph_ins_edge (&graph, "node1", "node10");
  ok (code == -1, "Edge insertion with unknwon vertex: Node1->Node10 (impossible)");

  generic_list_t resDFS;
  code = generic_dfs (&graph, &resDFS);
  ok (code == 0, "DFS traversal");
  ok (generic_list_size (&resDFS) == 5, "Check length (5) of the result");

  generic_list_elmt_t* elt;
  for (elt = generic_list_head (&resDFS);
       elt != generic_list_tail (&resDFS);
       elt = generic_list_next (elt))
    {
      printf ("%s, ", (char *)generic_list_data(elt));
    }
  printf ("%s\n", (char *)generic_list_data(generic_list_tail (&resDFS)));

  /* fprintf (stderr, "destroy list for DFS\n"); */
  /* generic_list_destroy (&resDFS); */

  generic_graph_destroy (&graph);

  done_testing();

  return EXIT_SUCCESS;
}
