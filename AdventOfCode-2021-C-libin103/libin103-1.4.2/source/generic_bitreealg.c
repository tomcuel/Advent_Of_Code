/*****************************************************************************
*                                                                            *
*  ------------------------------ traverse.c ------------------------------  *
*                                                                            *
*****************************************************************************/
#include <stdio.h>

#include "generic_list.h"
#include "generic_bitreealg.h"
#include "bistree.h"

/*****************************************************************************
*                                                                            *
*  ------------------------------- preorder -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_preorder(generic_bitreenode_t *node, generic_list_t *list) {

  /* Load the list with a generic_preorder listing of the tree. */
  if (!generic_bitree_is_eob(node)) {
    if (generic_list_ins_next(list, generic_list_tail(list),
			      generic_bitree_data(node)) != 0) {
      return -1;
    }

    if (!generic_bitree_is_eob(generic_bitree_left(node))) {
      if (generic_preorder(generic_bitree_left(node), list) != 0) {
	return -1;
      }
    }

    if (!generic_bitree_is_eob(generic_bitree_right(node))) {
      if (generic_preorder(generic_bitree_right(node), list) != 0) {
	return -1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  -------------------------------- inorder -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_inorder(generic_bitreenode_t *node, generic_list_t *list) {

  /* Load the list with an generic_inorder listing of the tree. */
  if (generic_bitree_is_eob(node) == false) {
    /* printf ("%s Not a null pointer\n", __func__); */
    if (generic_bitree_is_eob(node->left) == false) {
      /* printf ("%s Not a null pointer on the left recurse\n", __func__); */
      if (generic_inorder(node->left, list) != 0) {
	return -1;
      }
    }

    /* printf ("%s Insert node %d\n", __func__, ((integer_avlnode_t*)generic_bitree_data(node))->data); */
    if (generic_list_ins_next(list, generic_list_tail(list),
			      generic_bitree_data(node)) != 0) {
      return -1;
    }

    if (generic_bitree_is_eob(node->right) == false) {
      /* printf ("%s Not a null pointer on the right recurse\n", __func__); */
      if (generic_inorder(node->right, list) != 0) {
	return -1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ------------------------------- generic_postorder ----------------------  *
*                                                                            *
*****************************************************************************/

int generic_postorder(generic_bitreenode_t *node, generic_list_t *list) {

  /* Load the list with a postorder listing of the tree. */
  if (!generic_bitree_is_eob(node)) {

    if (!generic_bitree_is_eob(generic_bitree_left(node))) {
      if (generic_postorder(generic_bitree_left(node), list) != 0) {
	return -1;
      }
    }

    if (!generic_bitree_is_eob(generic_bitree_right(node))) {
      if (generic_postorder(generic_bitree_right(node), list) != 0) {
	return -1;
      }
    }

    if (generic_list_ins_next(list, generic_list_tail(list),
			      generic_bitree_data(node)) != 0) {
      return -1;
    }
  }

  return 0;
}
