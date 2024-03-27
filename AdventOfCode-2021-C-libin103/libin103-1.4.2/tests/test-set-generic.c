#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tap.h"
#include "generic_set.h"
#include "generic_list.h"

/* Shall define a new function to fit the specification with void* */
int compare (const void *a, const void *b) {
  const char* sa = a;
  const char* sb = b;
  return strcmp (sa, sb);
}

/* Shall define a new function to fit the specification with void* */
void* build (const void *data) {
  const char* sdata = data;
  int len = strlen(data);
  char *dest = malloc((len+1) * sizeof(char)); /* Do not forget the \0 value */
  /* By default strncpy return a pointer on dest */
  return strncpy (dest, sdata, len+1); /* Do not forget the \0 value */
}

int main (void) {
  plan(NO_PLAN);

  int code;
  generic_set_t set1;

  generic_set_init (&set1, compare, build, free);

  ok (generic_set_size (&set1) == 0, " Empty Set1 has zero element");

  code = generic_set_insert (&set1, "aaa");
  ok (code == 0, "Insertion of an element in Set1");
  ok (generic_set_size (&set1) == 1, "Set1 has one element");
  generic_set_insert (&set1, "bbb");
  ok (code == 0, "Insertion of an element in non empty set");
  ok (generic_set_size (&set1) == 2, "Set1 has two elements");

  code = generic_set_insert (&set1, "aaa");
  ok (code == 1, "Insertion of an element already in the set");
  ok (generic_set_size (&set1) == 2, "Set1 has still two elements");

  char *str = "bbb";
  code = generic_set_remove (&set1, (void**)&str);
  ok (code == 0, "Remove of an element already in the set");
  ok (generic_set_size (&set1) == 1, "Set1 has one element");

  generic_set_t set2;
  generic_set_init (&set2, compare, build, free);

  code = generic_set_insert (&set2, "aaa");
  ok (code == 0, "Insertion of an element in Set2");
  ok (generic_set_size (&set2) == 1, "Set2 has one element");
  generic_set_insert (&set2, "ccc");
  ok (code == 0, "Insertion of an element in non empty set");
  ok (generic_set_size (&set2) == 2, "Set2 has two elements");

  generic_set_t set3;
  code = generic_set_union (&set3, &set1, &set2);
  ok (code == 0, "Set3 is union of Set1 and Set2");
  ok (generic_set_size (&set3) == 2, "Set3 has two elements");

  generic_list_elmt_t *elt3 = generic_list_head (&set3);
  for (; elt3 != NULL; elt3 = generic_list_next (elt3)) {
    printf ("%s, ", (char*)generic_list_data(elt3));
  }
  printf ("\n");

  generic_set_t set4;
  code = generic_set_intersection (&set4, &set1, &set2);
  ok (code == 0, "Set4 is intersection of Set1 and Set2");
  ok (generic_set_size (&set4) == 1, "Set4 has one element");

  generic_list_elmt_t *elt4 = generic_list_head (&set4);
  for (; elt4 != NULL; elt4 = generic_list_next (elt4)) {
    printf ("%s, ", (char*)generic_list_data(elt4));
  }
  printf ("\n");

  generic_set_t set5;
  code = generic_set_difference (&set5, &set1, &set2);
  ok (code == 0, "Set5 is difference of Set1 by Set2 (keep elements of Set1 not in Set2");
  ok (generic_set_size (&set5) == 0, "Set5 has zero elements");

  generic_list_elmt_t *elt5 = generic_list_head (&set5);
  for (; elt5 != NULL; elt5 = generic_list_next (elt5)) {
    printf ("%s, ", (char*)generic_list_data(elt5));
  }
  printf ("\n");

  ok (generic_set_is_subset (&set1, &set2) == true, "Set1 is subset of Set2");
  ok (generic_set_is_equal (&set1, &set2) == false, "Set1 is not equal to Set2");
  ok (generic_set_is_equal (&set3, &set2) == true, "Set3 is equal to Set2");

  generic_set_destroy (&set1);
  generic_set_destroy (&set2);

  done_testing();

  return EXIT_SUCCESS;
}
