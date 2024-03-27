#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "integer_uf.h"

void integer_uf_init (integer_uf_t *dset, int size)
{
  dset->forest = malloc (size * sizeof(integer_uf_elm_t*));
  if (dset->forest == NULL) {
    assert (false);
  }
  for (int i = 0; i < size; i++) {
    dset->forest[i] = NULL;
  }

  dset->size = size;
  dset->components = 0;
}

void integer_uf_destroy (integer_uf_t *dset){
  for (int i = 0; i < dset->size; i++) {
    free(dset->forest[i]);
  }
  free(dset->forest);

  dset->size = 0;
  dset->components = 0;
}


int integer_uf_add_element(integer_uf_t *dset, int value)
{
  assert (dset != NULL);
  assert (value >= 0 && value < dset->size);

  integer_uf_elm_t *singleton = malloc(sizeof(integer_uf_elm_t));
  if (singleton == NULL) {
    return -1;
  }

  dset->forest[value] = singleton;
  dset->components++;

  singleton->value = value;
  singleton->parent = NULL;
  singleton->depth = 1;

  return 0;
}

int integer_uf_find(integer_uf_t *dset, int value, integer_uf_elm_t **result)
{
  assert (dset != NULL);
  assert (value >= 0 && value < dset->size);

  integer_uf_elm_t *elt = dset->forest[value];
  integer_uf_elm_t *original_elt = elt;

  if (elt == NULL) {
    result = NULL;
    return -1;
  }

  while(elt->parent != NULL) {
    elt=elt->parent;
  }

  if(elt != original_elt->parent && elt != original_elt) {
    original_elt->parent = elt; //path compression
  }

  *result = elt;

  return 0;
}

int integer_uf_union(integer_uf_t *dset, int value1, int value2)
{
  integer_uf_elm_t *root1;
  integer_uf_find(dset, value1, &root1);
  if (root1 == NULL) {
    return -1;
  }
  integer_uf_elm_t *root2;
  integer_uf_find(dset, value2, &root2);
  if (root2 == NULL) {
    return -1;
  }

  /* integer_uf_elm_t *newroot = root1; */

  if(root1 != root2) {
    dset->components--;
    if(root1->depth > root2->depth) { //merge by rank
      root2->parent = root1;
      root1->depth++;
    }
    else {
      root1->parent = root2;
      root2->depth++;
      /* newroot = root2; */
    }
  }

  return 0;
}

bool integer_uf_are_connected (integer_uf_t *dset,
			       int value1,
			       int value2)
{
  assert (dset != NULL);

  integer_uf_elm_t *root1;
  integer_uf_find(dset, value1, &root1);
  if (root1 == NULL) {
    return false;
  }

  integer_uf_elm_t *root2;
  integer_uf_find(dset, value2, &root2);
  if (root2 == NULL) {
    return false;
  }

  return root1 == root2;
}

int integer_uf_size (integer_uf_t *dset)
{
  assert (dset != NULL);
  return dset->size;
}

int integer_uf_components (integer_uf_t *dset)
{
  assert (dset != NULL);
  return dset->components;
}
