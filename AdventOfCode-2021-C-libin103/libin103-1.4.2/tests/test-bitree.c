#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "tap.h"
#include "integer_bitree.h"
#include "integer_bitreealg.h"


int main (void) {
  plan(NO_PLAN);

  int code;
  /* int len; */

  integer_bitree_t tree;
  integer_bitree_init (&tree); 	/* Optional in this case */

  ok (integer_bitree_size(&tree) == 0, "Size in empty tree");

  integer_bitreenode_t* root = integer_bitree_root (&tree);
  ok (root == NULL, "Root is NULL in empty tree");

  code = integer_bitree_ins_left (&tree, NULL, 1);
  printf ("code = %d\n", code);

  ok (integer_bitree_size(&tree) == 1, "Size in one element tree");

  root = integer_bitree_root (&tree);
  printf ("code = %d\n", code);
  code = integer_bitree_ins_left (&tree, root, 2);
  printf ("code = %d\n", code);
  code = integer_bitree_ins_right (&tree, root, 3);
  printf ("code = %d\n", code);

  integer_bitreenode_t* left = integer_bitree_left(root);
  code = integer_bitree_ins_left (&tree, left, 4);
  printf ("code = %d\n", code);
  code = integer_bitree_ins_right (&tree, left, 5);
  printf ("code = %d\n", code);

  integer_bitreenode_t* right = integer_bitree_right(root);
  code = integer_bitree_ins_left (&tree, right, 6);
  printf ("code = %d\n", code);
  code = integer_bitree_ins_right (&tree, right, 7);
  printf ("code = %d\n", code);

  ok (integer_bitree_size(&tree) == 7, "Size in 7 elements tree");

  integer_list_t t_preorder;
  integer_list_init (&t_preorder);

  integer_list_t t_inorder;
  integer_list_init (&t_inorder);

  integer_list_t t_postorder;
  integer_list_init (&t_postorder);


  root = integer_bitree_root (&tree);
  integer_preorder (root, &t_preorder);
  integer_inorder (root, &t_inorder);
  integer_postorder (root, &t_postorder);

  ok(integer_list_size (&t_preorder) == 7, "List is not empty");
  ok(integer_list_size (&t_inorder) == 7, "List is not empty");
  ok(integer_list_size (&t_postorder) == 7, "List is not empty");

  integer_list_elmt_t *elt = integer_list_head(&t_preorder);
  printf ("List (preorder): ");
  for (; elt != NULL; elt = integer_list_next (elt)) {
    printf ("%d, ", elt->data);
  }
  printf ("\n");

  elt = integer_list_head(&t_inorder);
  printf ("List (inorder): ");
  for (; elt != NULL; elt = integer_list_next (elt)) {
    printf ("%d, ", elt->data);
  }
  printf ("\n");

  elt = integer_list_head(&t_postorder);
  printf ("List (postorder): ");
  for (; elt != NULL; elt = integer_list_next (elt)) {
    printf ("%d, ", elt->data);
  }
  printf ("\n");

  /* dies_ok ({integer_list_t list; */
  /*     integer_list_elmt_t *head = integer_list_head (&list); */
  /*     int data = integer_list_data (head); /\* Assertion shall failed *\/ */
  /*     printf("%d\n", data);}, "Can't access data in empty cell"); */


  integer_bitree_destroy (&tree);

  done_testing();

  return EXIT_SUCCESS;
}
