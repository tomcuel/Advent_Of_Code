/*****************************************************************************
*                                                                            *
*  -------------------------------- heap.c --------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "generic_heap.h"

/*****************************************************************************
*                                                                            *
*  Define private macros used by the heap implementation.                    *
*                                                                            *
*****************************************************************************/

#define generic_heap_parent(npos) ((int)(((npos) - 1) / 2))

#define generic_heap_left(npos) (((npos) * 2) + 1)

#define generic_heap_right(npos) (((npos) * 2) + 2)

/*****************************************************************************
*                                                                            *
*  ------------------------------- heap_init ------------------------------  *
*                                                                            *
*****************************************************************************/

void generic_heap_init(generic_heap_t *heap,
		       int (*compare)(const void *key1, const void *key2),
		       void* (*build)(const void *data),
		       void (*destroy)(void *data)) {

  /* Initialize the heap. */
  heap->size = 0;
  heap->compare = compare;
  heap->build = build;
  heap->destroy = destroy;
  heap->tree = NULL;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- heap_destroy -----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_heap_destroy(generic_heap_t *heap) {

  int i;

  /* Remove all the nodes from the heap. */
  if (heap->destroy != NULL) {
    for (i = 0; i < generic_heap_size(heap); i++) {
      /* Call a user-defined function to free dynamically allocated data. */
      heap->destroy(heap->tree[i]);
    }
  }

  /* Free the storage allocated for the heap. */
  free(heap->tree);

  /* No operations are allowed now, but clear the structure as a precaution. */
  memset(heap, 0, sizeof(generic_heap_t));
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ heap_insert -----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_heap_insert(generic_heap_t *heap, const void *data) {

  void *temp;
  int ipos;
  int ppos;

  /* Allocate storage for the node. */
  temp = (void **)realloc(heap->tree, (generic_heap_size(heap) + 1) * sizeof(void *));
  if (temp == NULL) {
    return -1;
  }
  else {
    heap->tree = temp;
  }

  /* Insert the node after the last node. */
  heap->tree[generic_heap_size(heap)] = heap->build(data);

  /* Heapify the tree by pushing the contents of the new node upward. */
  ipos = generic_heap_size(heap);
  ppos = generic_heap_parent(ipos);

  while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0) {

    /* Swap the contents of the current node and its parent. */
    temp = heap->tree[ppos];
    heap->tree[ppos] = heap->tree[ipos];
    heap->tree[ipos] = temp;

    /* Move up one level in the tree to continue heapifying. */
    ipos = ppos;
    ppos = generic_heap_parent(ipos);
  }

  /* Adjust the size of the heap to account for the inserted node. */
  heap->size++;

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- heap_extract -----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_heap_extract(generic_heap_t *heap, void **data) {

  void *save;
  void *temp;

  int ipos;
  int lpos;
  int rpos;
  int mpos;

  /* Do not allow extraction from an empty heap. */
  if (generic_heap_size(heap) == 0) {
    return -1;
  }

  /* Extract the node at the top of the heap. */
  *data = heap->tree[0];

  /* Adjust the storage used by the heap. */
  save = heap->tree[generic_heap_size(heap) - 1];

  if (generic_heap_size(heap) - 1 > 0) {
    temp = (void **)realloc(heap->tree, (generic_heap_size(heap) - 1) * sizeof(void *));
    if (temp == NULL) {
      return -1;
    }
    else {
      heap->tree = temp;
    }

    /* Adjust the size of the heap to account for the extracted node. */
    heap->size--;
  }
  else {
    /* Manage the heap when extracting the last node. */
    free(heap->tree);
    heap->tree = NULL;
    heap->size = 0;
    return 0;
  }

  /* Copy the last node to the top. */
  heap->tree[0] = save;

  /* Heapify the tree by pushing the contents of the new top downward. */
  ipos = 0;
  lpos = generic_heap_left(ipos);
  rpos = generic_heap_right(ipos);

  while (1) {
    /* Select the child to swap with the current node. */
    lpos = generic_heap_left(ipos);
    rpos = generic_heap_right(ipos);

    if ( (lpos < generic_heap_size(heap)) &&
	 (heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0) ) {

      mpos = lpos;
    }
    else {
      mpos = ipos;
   }

    if ( (rpos < generic_heap_size(heap)) &&
	 (heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0) ) {
      mpos = rpos;
    }
    /* When mpos is ipos, the heap property has been restored. */

    if (mpos == ipos) {
      break;
    }
    else {
      /* Swap the contents of the current node and the selected child. */
      temp = heap->tree[mpos];
      heap->tree[mpos] = heap->tree[ipos];
      heap->tree[ipos] = temp;

      /* Move down one level in the tree to continue heapifying. */
      ipos = mpos;
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- heap_size --------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_heap_size(generic_heap_t *heap) {
  assert (heap != NULL);
  return heap->size;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- heap_change_key --------------------------  *
*                                                                            *
*****************************************************************************/


int generic_heap_change_key (generic_heap_t *heap, int position, void *data) {

  /* Change the value of a node. */
  /* Assume that only the value used for comparison has changed
     (diminishing in heap min or increasing in heap max !!! */
  free(heap->tree[position]);
  heap->tree[position] = heap->build(data);

  /* Heapify the tree by pushing the contents of the new node upward. */
  int ipos = position;
  int ppos = generic_heap_parent(ipos);

  while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0) {

    /* Swap the contents of the current node and its parent. */
    void* temp = heap->tree[ppos];
    heap->tree[ppos] = heap->tree[ipos];
    heap->tree[ipos] = temp;

    /* Move up one level in the tree to continue heapifying. */
    ipos = ppos;
    ppos = generic_heap_parent(ipos);
  }

  return 0;
}
