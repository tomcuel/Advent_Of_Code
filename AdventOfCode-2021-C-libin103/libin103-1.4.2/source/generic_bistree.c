
/*****************************************************************************
*                                                                            *
*  ------------------------------- bistree.c ------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "generic_bistree.h"

#ifdef DEBUG
#pragma message "activate debug mode generic_bistree"
#endif

static void* (*build_elem) (const void*);

static void* generic_build (const void *avl_data) {
  generic_avlnode_t *new_avl_data = malloc(sizeof(generic_avlnode_t));
  assert(new_avl_data!=NULL);
  new_avl_data->factor = ((generic_avlnode_t *)avl_data)->factor;
  new_avl_data->hidden = ((generic_avlnode_t *)avl_data)->hidden;
  /* printf ("(%s)%s data=%s\n", __FILE__, __func__, (char*)((generic_avlnode_t *)avl_data)->data); */
  new_avl_data->data = build_elem(((generic_avlnode_t *)avl_data)->data);
  return (void*)new_avl_data;
}

static void generic_destroy_right(generic_bistree_t *tree,
				  generic_bitreenode_t *node);

/*****************************************************************************
 *                                                                            *
 *  ------------------------------ rotate_left -----------------------------  *
 *                                                                            *
 *****************************************************************************/

static void generic_rotate_left(generic_bitreenode_t **node) {

  generic_bitreenode_t *left;
  generic_bitreenode_t *grandchild;

  left = (*node)->left;

  generic_avlnode_t* nodeLeft = generic_bitree_data(left);
  if (nodeLeft->factor == AVL_LFT_HEAVY) {

    /* Perform an LL rotation. */
    /* generic_bitreenode_t* leftLeft = (*node)->left; */
    /* leftLeft = left->right; */
    (*node)->left = left->right;

    /* generic_bitreenode_t* leftRight = left->right; */
    /* leftRight = *node; */
    left->right=*node;

    ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
    ((generic_avlnode_t *)generic_bitree_data(left))->factor = AVL_BALANCED;
    *node = left;
  }
  else {
    /* Perform an LR rotation. */
    grandchild = left->right;

    /* generic_bitreenode_t* leftRight = left->right; */
    /* leftRight = grandchild->left; */
    left->right = grandchild->left;

    /* generic_bitreenode_t* leftG = grandchild->left; */
    /* leftG = left; */
    grandchild->left = left;

    /* generic_bitreenode_t* leftN =  (*node)->left; */
    /* leftN = grandchild->right; */
    (*node)->left = grandchild->right;

    /* generic_bitreenode_t* rightG = grandchild->right; */
    /* rightG = *node; */
    grandchild->right = *node;

    switch (((generic_avlnode_t *)generic_bitree_data(grandchild))->factor) {

    case AVL_LFT_HEAVY:
      ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_RGT_HEAVY;
      ((generic_avlnode_t *)generic_bitree_data(left))->factor = AVL_BALANCED;
      break;

    case AVL_BALANCED:
      ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
      ((generic_avlnode_t *)generic_bitree_data(left))->factor = AVL_BALANCED;
      break;

    case AVL_RGT_HEAVY:
      ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
      ((generic_avlnode_t *)generic_bitree_data(left))->factor = AVL_LFT_HEAVY;
      break;
    }

    ((generic_avlnode_t *)generic_bitree_data(grandchild))->factor = AVL_BALANCED;
    *node = grandchild;
  }

  return;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- rotate_right -----------------------------  *
*                                                                            *
*****************************************************************************/

static void generic_rotate_right(generic_bitreenode_t **node) {

  generic_bitreenode_t *right;
  generic_bitreenode_t *grandchild;

  right = (*node)->right;

  generic_avlnode_t * nodeAvl = generic_bitree_data(right);
  if (nodeAvl->factor == AVL_RGT_HEAVY) {
   /* Perform an RR rotation. */

    /* generic_bitreenode_t *rightN = (*node)->right; */
    /* rightN = right->left; */
    (*node)->right = right->left;

    /* generic_bitreenode_t *rightLeft = right->left; */
    /* rightLeft= *node; */
    right->left = *node;

    ((generic_avlnode_t *)(*node)->data)->factor = AVL_BALANCED;
    ((generic_avlnode_t *)right->data)->factor = AVL_BALANCED;
    *node = right;
  }
  else {
   /* Perform an RL rotation. */
   grandchild = right->left;

   /* generic_bitreenode_t *rightLeft = right->left; */
   /* rightLeft = grandchild->right; */
   right->left = grandchild->right;

   /* generic_bitreenode_t *rightG = grandchild->right; */
   /* rightG = right; */
   grandchild->right = right;

   /* generic_bitreenode_t *rightN = (*node)->right; */
   /* rightN= grandchild->left; */
   (*node)->right = grandchild->left;

   /* generic_bitreenode_t *leftG = grandchild->left; */
   /* leftG= *node; */
   grandchild->left = *node;

   switch (((generic_avlnode_t *)generic_bitree_data(grandchild))->factor) {
   case AVL_LFT_HEAVY:
     ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
     ((generic_avlnode_t *)generic_bitree_data(right))->factor = AVL_RGT_HEAVY;
     break;

   case AVL_BALANCED:
     ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
     ((generic_avlnode_t *)generic_bitree_data(right))->factor = AVL_BALANCED;
     break;

   case AVL_RGT_HEAVY:
     ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_LFT_HEAVY;
     ((generic_avlnode_t *)generic_bitree_data(right))->factor = AVL_BALANCED;
     break;
   }

   ((generic_avlnode_t *)generic_bitree_data(grandchild))->factor = AVL_BALANCED;
   *node = grandchild;
  }

  return;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- destroy_left -----------------------------  *
*                                                                            *
*****************************************************************************/

static void generic_destroy_left(generic_bistree_t *tree,
				 generic_bitreenode_t *node)
{

  generic_bitreenode_t **position;

  /* Do not allow destruction of an empty tree. */

  if (generic_bitree_size(tree) == 0) {
    return;
  }

  /* Determine where to destroy nodes. */
  if (node == NULL) {
    position = &tree->root;
  }
  else {
    position = &node->left;
  }

  /* Destroy the nodes. */
  if (*position != NULL) {
    generic_destroy_left(tree, *position);
    generic_destroy_right(tree, *position);

    if (tree->destroy != NULL) {

      /* Call a user-defined function to free dynamically allocated data. */
      tree->destroy(((generic_avlnode_t *)(*position)->data)->data);
   }

    /* Free the AVL data in the node, then free the node itself. */
    free((*position)->data);
    free(*position);
    *position = NULL;

    /* Adjust the size of the tree to account for the destroyed node. */
    tree->size--;
  }

  return;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- destroy_right ----------------------------  *
*                                                                            *
*****************************************************************************/

static void generic_destroy_right(generic_bistree_t *tree,
				  generic_bitreenode_t *node)
{

  generic_bitreenode_t **position;

  /* Do not allow destruction of an empty tree. */
  if (generic_bitree_size(tree) == 0) {
    return;
  }

  /* Determine where to destroy nodes. */
  if (node == NULL) {
    position = &tree->root;
  }
  else {
   position = &node->right;
  }

  /* Destroy the nodes. */
  if (*position != NULL) {
    generic_destroy_left(tree, *position);
    generic_destroy_right(tree, *position);

    if (tree->destroy != NULL) {
      /* Call a user-defined function to free dynamically allocated data. */
      tree->destroy(((generic_avlnode_t *)(*position)->data)->data);
    }

    /* Free the AVL data in the node, then free the node itself. */
    free((*position)->data);
    free(*position);
    *position = NULL;

    /* Adjust the size of the tree to account for the destroyed node. */
    tree->size--;
  }

  return;
}

/*****************************************************************************
*                                                                            *
*  ------------------------ generic_insert --------------------------------  *
*                                                                            *
*****************************************************************************/

static int generic_insert(generic_bistree_t *tree,
			  generic_bitreenode_t **node,
			  void *data,
			  int *balanced)
{
#ifdef DEBUG
  fprintf (stderr, "## enter %s\n", __func__);
#endif


  generic_avlnode_t *avl_data = NULL;
  int cmpval;
  int retval;

  /* Insert the data into the tree. */
  if (generic_bitree_is_eob(*node)) {

#ifdef DEBUG
    fprintf (stderr, "## insert in root place\n");
#endif


    /* Handle insertion into an empty tree. */
    avl_data = malloc(sizeof(generic_avlnode_t));
    if (avl_data == NULL) {
      return -1;
    }
    avl_data->factor = AVL_BALANCED;
    avl_data->hidden = 0;
    avl_data->data = build_elem(data);/* tree->build(data); */
    return generic_bitree_ins_left(tree, *node, avl_data);
  }

  else {
    /* Handle insertion into a tree that is not empty. */

#ifdef DEBUG
    fprintf (stderr, "## insert in non empty tree \n");
#endif

    generic_avlnode_t *tempNode =  generic_bitree_data(*node);
    /* printf ("data  = %s\n", (char*) data); */
    /* printf ("tempNode->data  = %s\n", (char*) tempNode->data); */
    cmpval = tree->compare(data, tempNode->data);
    /* printf ("cmpval = %d\n", cmpval); */

    if (cmpval < 0) {
#ifdef DEBUG
    fprintf (stderr, "## insert in left tree\n");
#endif


      /* Move to the left. */
      if (generic_bitree_is_eob(generic_bitree_left(*node))) {

	avl_data = malloc(sizeof(generic_avlnode_t));
	if (avl_data == NULL) {
            return -1;
	}

	avl_data->factor = AVL_BALANCED;
	avl_data->hidden = 0;
	/* printf ("ici ici %s (%p)\n", (char*)data, data); */
	avl_data->data = build_elem(data);/* tree->build(data); */

	if (generic_bitree_ins_left(tree, *node, avl_data) != 0) {
	  return -1;
	}

	*balanced = 0;
      }

      else {
	/* generic_bitreenode_t *leftN = (*node)->left; */
	retval = generic_insert(tree, &((*node)->left), data, balanced);
	if (retval != 0) {
	  return retval;
	}
      }

      /* Ensure that the tree remains balanced. */

      if (!(*balanced)) {

	switch (((generic_avlnode_t *)generic_bitree_data(*node))->factor) {

	case AVL_LFT_HEAVY:
	  generic_rotate_left(node);
	  *balanced = 1;
	  break;

	case AVL_BALANCED:
	  ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_LFT_HEAVY;
	  break;

	case AVL_RGT_HEAVY:
	  ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
	  *balanced = 1;

	}
      }

    } /* if (cmpval < 0) */
    else if (cmpval > 0) {
#ifdef DEBUG
    fprintf (stderr, "## insert in right tree\n");
#endif

      /* Move to the right. */

      if (generic_bitree_is_eob((*node)->right)) {
	avl_data = malloc(sizeof(generic_avlnode_t));
	if (avl_data == NULL) {
            return -1;
	}

	avl_data->factor = AVL_BALANCED;
	avl_data->hidden = 0;
	avl_data->data = build_elem(data);

	if (generic_bitree_ins_right(tree, *node, avl_data) != 0) {
	  return -1;
	}

	*balanced = 0;
      }
      else {
	/* generic_bitreenode_t* rightN = (*node)->right; */
	retval = generic_insert(tree, &((*node)->right), data, balanced);
	if (retval != 0) {
	  return retval;
	}
      }

      /* Ensure that the tree remains balanced. */
      if (!(*balanced)) {
	switch (((generic_avlnode_t *)generic_bitree_data(*node))->factor) {

	case AVL_LFT_HEAVY:
	  ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_BALANCED;
	  *balanced = 1;
	  break;

	case AVL_BALANCED:
	  ((generic_avlnode_t *)generic_bitree_data(*node))->factor = AVL_RGT_HEAVY;
	  break;

	case AVL_RGT_HEAVY:
	  generic_rotate_right(node);
	  *balanced = 1;
	}
      }

    } /* if (cmpval > 0) */
    else {
      /* Handle finding a copy of the data. */
      if (!((generic_avlnode_t *)generic_bitree_data(*node))->hidden) {
         /* Do nothing since the data is in the tree and not hidden. */
	return 1;
      }
      else {
	/* Insert the new data and mark it as not hidden. */
	if (tree->destroy != NULL) {
	  /* Destroy the hidden data since it is being replaced. */
	  tree->destroy(((generic_avlnode_t *)generic_bitree_data(*node))->data);
	}

	/* printf ("ici la ici\n"); */
	((generic_avlnode_t *)generic_bitree_data(*node))->data = build_elem(data);/* tree->build(data); */
	((generic_avlnode_t *)generic_bitree_data(*node))->hidden = 0;

	/* Do not rebalance because the tree structure is unchanged. */
	*balanced = 1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  --------------------------------- hide ---------------------------------  *
*                                                                            *
*****************************************************************************/

static int generic_hide(generic_bistree_t *tree,
			generic_bitreenode_t *node,
			void *data)
{

  int cmpval;
  int retval;

  if (generic_bitree_is_eob(node)) {
    /* Return that the data was not found. */
    return -1;
  }

  generic_avlnode_t *tempNode = generic_bitree_data(node);
  cmpval = tree->compare(data, tempNode->data);

  if (cmpval < 0) {
    /* Move to the left. */
    retval = generic_hide(tree, node->left, data);
  }
  else if (cmpval > 0) {
   /* Move to the right. */
    retval = generic_hide(tree, node->right, data);
  }
  else {
   /* Mark the node as hidden. */
   ((generic_avlnode_t *)generic_bitree_data(node))->hidden = 1;
   retval = 0;
  }

  return retval;
}

/*****************************************************************************
*                                                                            *
*  -------------------------------- lookup --------------------------------  *
*                                                                            *
*****************************************************************************/

static int generic_lookup(generic_bistree_t *tree,
			  generic_bitreenode_t *node,
			  void **data)
{

  int cmpval;
  int retval;

  if (generic_bitree_is_eob(node)) {
    /* Return that the data was not found. */
    return -1;
  }

  cmpval = tree->compare(*data, ((generic_avlnode_t *)generic_bitree_data(node))->data);
  if (cmpval < 0) {
   /* Move to the left. */
    retval = generic_lookup(tree, node->left, data);
   }
  else if (cmpval > 0) {
    /* Move to the right. */
    retval = generic_lookup(tree, node->right, data);
   }
  else {
    if (!((generic_avlnode_t *)generic_bitree_data(node))->hidden) {

      /* Pass back the data from the tree. */
      *data = ((generic_avlnode_t *)generic_bitree_data(node))->data;
      retval = 0;
    }
    else {
      /* Return that the data was not found. */
      return -1;
    }
  }

  return retval;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- bistree_init -----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_bistree_init(generic_bistree_t *tree,
			  int (*compare)(const void *key1, const void *key2),
			  void* (*build) (const void *data),
			  void (*destroy)(void *data))
{
  build_elem = build;

  /* Initialize the tree. */
  generic_bitree_init(tree, compare, generic_build, free);

  return;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bistree_destroy ---------------------------  *
*                                                                            *
*****************************************************************************/

void generic_bistree_destroy(generic_bistree_t *tree) {

  /* Destroy all nodes in the tree. */

  generic_destroy_left(tree, NULL);

  /* No operations are allowed now, but clear the structure as a precaution. */
  memset(tree, 0, sizeof(generic_bitree_t));

  return;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bistree_insert ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bistree_insert(generic_bistree_t *tree,
			   void *data)
{

#ifdef DEBUG
  fprintf (stderr, "## enter %s\n", __func__);
#endif

  int balanced = 0;
  return generic_insert(tree, &(tree->root), data, &balanced);
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bistree_remove ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bistree_remove(generic_bistree_t *tree,
			   void *data)
{
  return generic_hide(tree, generic_bitree_root(tree), data);
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bistree_lookup ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bistree_lookup(generic_bistree_t *tree,
			   void **data)
{
  return generic_lookup(tree, generic_bitree_root(tree), data);
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- bistree_size ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_bistree_size(generic_bistree_t *tree)
{
  assert(tree != NULL);
  return tree->size;
}
