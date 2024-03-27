#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tap.h"
#include "integer_list.h"


int main (void) {
  plan(NO_PLAN);
  /* plan (19); */

  int code;
  int data;
  integer_list_t list;
  integer_list_init (&list); 	/* Optional in this case */

  integer_list_elmt_t *head = integer_list_head (&list);
  ok (head == NULL, "Head access in empty list");
  dies_ok ({data = integer_list_data (head); /* Assertion shall failed */
      printf("%d\n", data);}, "Can't access data in empty cell");

  integer_list_elmt_t *tail = integer_list_tail (&list);
  ok (tail == NULL, "Tail access in empty list");

  code = integer_list_ins_next (&list, NULL, 3);
  ok (code == 0, "Head insertion in empty list");

  code = integer_list_ins_next (&list, NULL, 10);
  ok (code == 0, "Head insertion in non empty list");
  ok (integer_list_size(&list) == 2, "List length access");

  head = integer_list_head (&list);
  ok (head->data == 10, "Head access in non empty list");
  ok (integer_list_is_head(&list, head) == true, "Predicat is  head true");
  ok (integer_list_is_tail(head) == false, "Predicat is tail false");

  tail = integer_list_tail (&list);
  ok (tail->data == 3, "Tail access in non empty list");
  ok (integer_list_is_tail(tail) == true, "Predicat is tail true");
  ok (integer_list_is_head(&list, tail) == false, "Predicat is  head false");
  ok (integer_list_next (head) == tail, "Next of head is tail true");
  ok (integer_list_next (tail) != head, "Next of tail is not head ");

  code = integer_list_rem_next (&list, NULL, &data);
  ok (code == 0, "Head deletion in non empty list");
  ok (integer_list_size(&list) == 1, "List length access after deletion");
  ok (data == 10, "Check the correctness of the value");

  code = integer_list_rem_next (&list, NULL, &data);
  ok (code == 0, "Head deletion in non empty list");
  ok (integer_list_size(&list) == 0, "List length access after deletion");
  ok (data == 3, "Check the correctness of the value");

  code = integer_list_rem_next (&list, NULL, &data);
  ok (code == -1, "Head deletion in empty list");

  integer_list_destroy (&list);

  done_testing();

  return EXIT_SUCCESS;
}
