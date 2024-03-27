#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tap.h"
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
  /* plan(NO_PLAN); */
  plan (19);

  int code;
  generic_list_t list;

  generic_list_init (&list, compare, build, free);

  generic_list_elmt_t *head = generic_list_head (&list);
  ok (head == NULL, "Head access in empty list");
  dies_ok ({generic_list_data (head);}, "Can't access data in empty cell");

  generic_list_elmt_t *tail = generic_list_tail (&list);
  ok (tail == NULL, "Tail access in empty list");

  char *str1 = "salut les amis";
  code = generic_list_ins_next (&list, NULL, str1);
  ok (code == 0, "Head insertion in empty list");

  char *str2 = "les copains sont partis";
  code = generic_list_ins_next (&list, NULL, str2);
  ok (code == 0, "Head insertion in non empty list");
  ok (generic_list_size(&list) == 2, "List length access");

  str2 = "bye";
  generic_list_elmt_t* elt = generic_list_head (&list);
  for (; elt != NULL; elt = generic_list_next(elt)) {
    char *data = generic_list_data(elt);
    printf ("%s | ", data);
  }
  printf ("\n%s\n", str2);
  str2 = "les copains sont partis";

  head = generic_list_head (&list);
  char *data = generic_list_data (head); /* Variable declaration prevents using cast */
  ok (strcmp(data, str2) == 0, "Head access in non empty list");
  ok (generic_list_is_head(&list, head) == true, "Predicat is  head true");
  ok (generic_list_is_tail(head) == false, "Predicat is tail false");

  tail = generic_list_tail (&list);
  data = generic_list_data (tail);
  ok (strcmp (data, str1) == 0 , "Tail access in non empty list");
  ok (generic_list_is_tail(tail) == true, "Predicat is tail true");
  ok (generic_list_is_head(&list, tail) == false, "Predicat is  head false");
  ok (generic_list_next (head) == tail, "Next of head is tail true");
  ok (generic_list_next (tail) != head, "Next of tail is not head ");

  code = generic_list_rem_next (&list, NULL, (void **) &data);
  ok (code == 0, "Head deletion in non empty list");
  ok (generic_list_size(&list) == 1, "List length access after deletion");

  code = generic_list_rem_next (&list, NULL, (void **) &data);
  ok (code == 0, "Head deletion in non empty list");
  ok (generic_list_size(&list) == 0, "List length access after deletion");

  code = generic_list_rem_next (&list, NULL, NULL);
  ok (code == -1, "Head deletion in empty list");

  generic_list_destroy (&list);

  done_testing();

  return EXIT_SUCCESS;
}
