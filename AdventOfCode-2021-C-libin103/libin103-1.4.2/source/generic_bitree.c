/*****************************************************************************
*                                                                            *
*  ------------------------------- bitree.c -------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "generic_bitree.h"


#ifdef DEBUG
#pragma message "activate debug mode generic_bitree"
#endif


/*****************************************************************************
*                                                                            *
*  ------------------------------ bitree_init -----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_bitree_init(generic_bitree_t *tree,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build)(const void *data),
			 void (*destroy)(void *data)) {

  /*  Initialize the binary tree. */
  tree->size = 0;
  tree->compare = compare;
  tree->build = build;
  tree->destroy = destroy;
  tree->root = NULL;

  return;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bitree_destroy ----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_bitree_destroy(generic_bitree_t *tree) {

  /* Remove all the nodes from the tree. */
  generic_bitree_rem_left(tree, NULL);

  /*  No operations are allowed now, but clear the structure as a precaution. */
  memset(tree, 0, sizeof(generic_bitree_t));

  return;
}

/*****************************************************************************
 *                                                                            *
 *  ---------------------------- bitree_ins_left ---------------------------  *
 *                                                                            *
 *****************************************************************************/

int generic_bitree_ins_left(generic_bitree_t *tree,
			    generic_bitreenode_t *node,
			    const void *data)
{
#ifdef DEBUG
  fprintf (stderr, "%s: insert in left branch\n", __func__);
#endif

  generic_bitreenode_t *new_node;
  generic_bitreenode_t **position;

  /* Determine where to insert the node. */
  if (node == NULL) {
    /* Allow insertion at the root only in an empty tree. */

    if (generic_bitree_size(tree) > 0) {
      return -1;
    }
    position = &tree->root;
  }
  else {
    /* Normally allow insertion only at the end of a branch. */
    if (generic_bitree_left(node) != NULL) {
      return -1;
    }
    position = &node->left;
  }

  /* Allocate storage for the node. */
  new_node = malloc(sizeof(generic_bitreenode_t));
  if (new_node == NULL) {
    return -1;
  }

  /* Insert the node into the tree. */
  new_node->data = tree->build(data);
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node.*/
  tree->size++;

  return 0;
}

/*****************************************************************************
 *                                                                            *
*  --------------------------- bitree_ins_right ---------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bitree_ins_right(generic_bitree_t *tree,
			     generic_bitreenode_t *node,
			     const void *data)

{
#ifdef DEBUG
  fprintf (stderr, "%s: insert in right branch\n", __func__);
#endif

  generic_bitreenode_t *new_node;
  generic_bitreenode_t **position;

  /* Determine where to insert the node. */
  if (node == NULL) {
    /* Allow insertion at the root only in an empty tree. */
    if (generic_bitree_size(tree) > 0) {
      return -1;
    }
    position = &tree->root;
  }
  else {
    /* Normally allow insertion only at the end of a branch. */

    if (generic_bitree_right(node) != NULL) {
      return -1;
    }

    position = &node->right;
  }

  /* Allocate storage for the node. */
  new_node = malloc(sizeof(generic_bitreenode_t));
  if (new_node == NULL) {
    return -1;
  }

  /* Insert the node into the tree. */

  new_node->data = tree->build(data);
  new_node->left = NULL;
  new_node->right = NULL;
  *position = new_node;

  /* Adjust the size of the tree to account for the inserted node. */
  tree->size++;

  return 0;
}

/*****************************************************************************
 *                                                                            *
 *  ---------------------------- bitree_rem_left ---------------------------  *
 *                                                                            *
 *****************************************************************************/

void generic_bitree_rem_left(generic_bitree_t *tree,
			     generic_bitreenode_t *node)
 {

   generic_bitreenode_t         **position;

   /* Do not allow removal from an empty tree. */
   if (generic_bitree_size(tree) == 0) {
     return;
   }

   /* Determine where to remove nodes. */
   if (node == NULL) {
     position = &tree->root;
   }
   else {
     position = &node->left;
   }

   /* Remove the nodes. */
   if (*position != NULL) {

     generic_bitree_rem_left(tree, *position);
     generic_bitree_rem_right(tree, *position);

     if (tree->destroy != NULL) {

       /* Call a user-defined function to free dynamically allocated data. */
       tree->destroy((*position)->data);
     }

     free(*position);
     *position = NULL;

     /* Adjust the size of the tree to account for the removed node. */
     tree->size--;
   }

   return;
 }

/*****************************************************************************
 *                                                                            *
 *  --------------------------- bitree_rem_right ---------------------------  *
 *                                                                            *
 *****************************************************************************/

void generic_bitree_rem_right(generic_bitree_t *tree,
			      generic_bitreenode_t *node)

{

  generic_bitreenode_t **position;

  /* Do not allow removal from an empty tree. */
  if (generic_bitree_size(tree) == 0) {
    return;
  }

  /*  Determine where to remove nodes. */
  if (node == NULL) {
    position = &tree->root;
  }
  else {
    position = &node->right;
  }

  /* Remove the nodes. */
  if (*position != NULL) {

    generic_bitree_rem_left(tree, *position);
    generic_bitree_rem_right(tree, *position);

    if (tree->destroy != NULL) {
      /* Call a user-defined function to free dynamically allocated data. */
      tree->destroy((*position)->data);
    }

    free(*position);
    *position = NULL;

    /* Adjust the size of the tree to account for the removed node. */

    tree->size--;

  }

  return;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- bitree_merge -----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bitree_merge(generic_bitree_t *merge,
			 generic_bitree_t *left,
			 generic_bitree_t *right,
			 const void *data)
{

  /* Initialize the merged tree. */
  generic_bitree_init(merge, left->compare, left->build, left->destroy);

  /* Insert the data for the root node of the merged tree. */
  if (generic_bitree_ins_left(merge, NULL, data) != 0) {
    generic_bitree_destroy(merge);
    return -1;
  }

  /* Merge the two binary trees into a single binary tree. */
  generic_bitree_root(merge)->left = generic_bitree_root(left);
  generic_bitree_root(merge)->right = generic_bitree_root(right);

  /* Adjust the size of the new binary tree. */
  merge->size = merge->size + generic_bitree_size(left) + generic_bitree_size(right);

  /* Do not let the original trees access the merged nodes. */
  left->root = NULL;
  left->size = 0;
  right->root = NULL;
  right->size = 0;

  return 0;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_size -----------------------------  *
 *                                                                            *
 *****************************************************************************/

int generic_bitree_size (generic_bitree_t *tree)
{
  assert (tree != NULL);
  return tree->size;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_root -----------------------------  *
 *                                                                            *
 *****************************************************************************/

generic_bitreenode_t* generic_bitree_root (generic_bitree_t *tree) {
  assert (tree != NULL);
  return tree->root;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_is_eob -----------------------------  *
 *                                                                            *
 *****************************************************************************/

bool generic_bitree_is_eob (generic_bitreenode_t* node) {
  if (node == NULL) {
    return true;
  }
  else {
    return false;
  }
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_is_leaf -----------------------------  *
 *                                                                            *
 *****************************************************************************/

bool generic_bitree_is_leaf (generic_bitreenode_t* node) {
  assert (node != NULL);
  if (node->left == NULL && node->right == NULL) {
    return true;
  }
  else {
    return false;
  }
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_data -----------------------------  *
 *                                                                            *
 *****************************************************************************/

void* generic_bitree_data (generic_bitreenode_t* node) {
  assert (node != NULL);
  return node->data;
}

/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_left -----------------------------  *
 *                                                                            *
 *****************************************************************************/

generic_bitreenode_t* generic_bitree_left (generic_bitreenode_t *node) {
  assert (node != NULL);
  return node->left;
}


/*****************************************************************************
 *                                                                            *
 *  ----------------------------- bitree_right -----------------------------  *
 *                                                                            *
 *****************************************************************************/

generic_bitreenode_t* generic_bitree_right (generic_bitreenode_t *node) {
  assert (node != NULL);
  return node->right;
}
