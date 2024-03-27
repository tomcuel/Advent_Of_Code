#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tap.h"
#include "integer_uf.h"


int main (void) {
  plan(NO_PLAN);
  /* plan (19); */

  int size = 10;
  integer_uf_t dset;
  integer_uf_init (&dset, size); 	/* Optional in this case */

  for (int i = 0; i < size; i++) {
    int code = integer_uf_add_element (&dset, i);
    if (code == -1) {
      fprintf (stderr, "Erreur insertion nouvel element\n");
      integer_uf_destroy (&dset);
      return EXIT_FAILURE;
    }
  }

  ok(integer_uf_components (&dset) == 10, "At start, all elements are singletons");

  integer_uf_union(&dset, 3, 4);
  integer_uf_union(&dset, 8, 0);
  integer_uf_union(&dset, 2, 3);
  integer_uf_union(&dset, 5, 6);
  integer_uf_union(&dset, 5, 1);
  integer_uf_union(&dset, 7, 3);
  integer_uf_union(&dset, 1, 6);
  integer_uf_union(&dset, 4, 8);

  ok (integer_uf_are_connected (&dset, 2, 4) == true, "Node 2 and Node 4 are connected");

  ok (integer_uf_are_connected (&dset, 8, 5) == false, "Node 8 and Node 5 are not connected");

  ok(integer_uf_components (&dset) == 3, "At end, only 3 components");

  for (int i = 0; i < integer_uf_size(&dset); i++) {
    integer_uf_elm_t *tmp;
    integer_uf_find (&dset, i, &tmp);
    printf ("Node (%d) in component [%d]\n", i, tmp->value);
  }

  integer_uf_destroy (&dset);

  done_testing();

  return EXIT_SUCCESS;
}
