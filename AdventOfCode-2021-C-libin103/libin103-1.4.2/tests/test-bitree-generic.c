#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "tap.h"
#include "bitree.h"
#include "bitreealg.h"
#include "list.h"

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
  /* plan (5); */

  int code;

  generic_bitree_t tree;
  generic_bitree_init (&tree, compare, build, free); 	/* Optional in this case */

  ok (generic_bitree_size(&tree) == 0, "Size in empty tree");

  generic_bitreenode_t* root = generic_bitree_root (&tree);
  ok (root == NULL, "Root is NULL in empty tree");

  code = generic_bitree_ins_left (&tree, NULL, "1");
  ok (code == 0, "Insertion of str(1) in Tree");
  ok (generic_bitree_size(&tree) == 1, "Tree size is one");

  root = generic_bitree_root (&tree);
  code = generic_bitree_ins_left (&tree, root, "2");
  ok (code == 0, "Insertion of str(2) in Tree->Left");
  code = generic_bitree_ins_right (&tree, root, "3");
  ok (code == 0, "Insertion of str(3) in Tree->Right");

  generic_bitreenode_t* left = generic_bitree_left(root);
  code = generic_bitree_ins_left (&tree, left, "4");
  ok (code == 0, "Insertion of str(4) in Tree->Left->Left");
  code = generic_bitree_ins_right (&tree, left, "5");
  ok (code == 0, "Insertion of str(5) in Tree->Left->Right");

  generic_bitreenode_t* right = generic_bitree_right(root);
  code = generic_bitree_ins_left (&tree, right, "6");
  ok (code == 0, "Insertion of str(6) in Tree->Right->Left");
  code = generic_bitree_ins_right (&tree, right, "7");
  ok (code == 0, "Insertion of str(7) in Tree->Right->Right");

  ok (generic_bitree_size(&tree) == 7, "Size in 7 elements tree");

  generic_list_t t_preorder;
  generic_list_init (&t_preorder, compare, build, free);
  root = generic_bitree_root (&tree);
  generic_preorder (root, &t_preorder);

  ok(generic_list_size (&t_preorder) == 7, "List is not empty");

  generic_list_elmt_t *elt = generic_list_head(&t_preorder);
  printf ("List (preorder): ");
  for (; elt != NULL; elt = generic_list_next (elt)) {
    printf ("%s, ", (char*)elt->data);
  }
  printf ("\n");

  generic_bitree_destroy (&tree);

  done_testing();

  return EXIT_SUCCESS;
}
