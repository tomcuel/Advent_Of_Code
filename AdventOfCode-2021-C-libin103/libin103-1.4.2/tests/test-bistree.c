#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tap.h"
#include "real_bistree.h"
#include "bitree.h"
#include "bitreealg.h"
#include "real_list.h"

int main (void) {
  plan(NO_PLAN);

  int code;
  /* int len; */

  real_bistree_t tree;

  real_bistree_init (&tree); 	/* Optional in this case */
  ok (real_bistree_size(&tree) == 0, "Size in empty tree");

  code = real_bistree_insert (&tree, 1.0);
  ok (code == 0, "Insertion of 1.0");
  ok (real_bistree_size(&tree) == 1, "Size in one element tree");

  code = real_bistree_insert (&tree, 20.0);
  ok (code == 0, "Insertion of 20.0");
  code = real_bistree_insert (&tree, 3.0);
  ok (code == 0, "Insertion of 3.0");

  code = real_bistree_insert (&tree, 4.5);
  ok (code == 0, "Insertion of 4.5");
  code = real_bistree_insert (&tree, 5.0);
  ok (code == 0, "Insertion of 5.0");

  code = real_bistree_insert (&tree, 6.0);
  ok (code == 0, "Insertion of 6.0");
  code = real_bistree_insert (&tree, 7.0);
  ok (code == 0, "Insertion of 7.0");

  ok (real_bistree_size(&tree) == 7, "Size in 7 elements tree");
  ok (real_bistree_lookup(&tree, 8.0) == false, "Search unkwon value 8.0");
  ok (real_bistree_lookup(&tree, 4.5) == true, "Search kwon value 4.5");

  real_bistree_destroy (&tree);

  done_testing();

  return EXIT_SUCCESS;
}
