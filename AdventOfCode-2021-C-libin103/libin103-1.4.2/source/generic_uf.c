#include <stdlib.h>
#include <assert.h>

#include <assert.h>

#include "generic_uf.h"

void generic_uf_init (generic_uf_t *dset,
		      int (*compare) (const void *key1, const void *key2),
		      void* (*build)(const void *data),
		      void (*destroy)(void *data))
{
  generic_list_init (&dset->set, compare, build, destroy);
  dset->compare = compare;
  dset->build = build;
  dset->destroy = destroy;
}

void generic_uf_destroy (generic_uf_t *dset){
  generic_list_destroy (&dset->set);
  dset->compare = NULL;
  dset->build = NULL;
  dset->destroy = NULL;
}


int generic_uf_add_element(generic_uf_t *dset, void *data)
{
  assert (dset != NULL);

  generic_uf_elm_t *singleton = malloc(sizeof(generic_uf_elm_t));
  if (singleton == NULL) {
    return -1;
  }

  singleton->value = dset->build(data);
  singleton->parent = NULL;
  singleton->depth = 1;

  return generic_list_ins_next (&dset->forest, NULL, singleton);
}

int generic_uf_find(generic_uf_t *dset, void *data, generic_uf_elm_t **elt)
{
  assert (dset != NULL);

  if(data == NULL) {
    return -1;
  }

  generic_uf_elm_t *elt = NULL;
  generic_uf_elm_t *original_elt;

  generic_list_elmt_t *temp = generic_list_head (&dset->forest);
  for (; temp != NULL; temp = generic_list_next(temp)) {
    generic_uf_elm_t* tt = generic_list_data(temp);
    if (dset->compare(tt->value, data) == 0) {
      elt = generic_list_data(temp);
      break;
    }
  }

  if (elt == NULL) {
    return -1;
  }

  original_elt = elt;

  while(elt->parent != NULL) {
    elt=elt->parent;
  }

  if(elt != original_elt->parent && elt != original_elt) {
    original_elt->parent = elt; //path compression
  }

  return elt;
}

generic_uf_elm_t* generic_uf_union(generic_uf_t *dset,
				   void *a,
				   void *b)
{
  generic_uf_elm_t *root_a = generic_uf_find(dset, a);
  generic_uf_elm_t *root_b = generic_uf_find(dset, b);
  generic_uf_elm_t *newroot = root_a;

  if(root_a != root_b) {
    if(root_a->depth > root_b->depth) { //merge by rank
      root_b->parent = root_a;
      root_a->depth++;
    }
    else {
      root_a->parent = root_b;
      root_b->depth++;
      newroot = root_b;
    }
  }

  return newroot;
}
