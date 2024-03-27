/*****************************************************************************
*                                                                            *
*  -------------------------------- set.c ---------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "generic_list.h"
#include "generic_set.h"


#ifdef DEBUG
#pragma message "activate debug mode generic_set"
#endif

/*****************************************************************************
*                                                                            *
*  ------------------------------- set_init -------------------------------  *
*                                                                            *
*****************************************************************************/

void generic_set_init(generic_set_t  *set,
		      int (*compare)(const void *key1, const void *key2),
		      void* (*build) (const void* data),
		      void (*destroy)(void *data))
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  /* Initialize the set. */
  generic_list_init(set, compare, build, destroy);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------- set_destroy ----------------------------  *
*                                                                            *
*****************************************************************************/

void generic_set_destroy(generic_set_t  *set)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_destroy (set);
}


/*****************************************************************************
*                                                                            *
*  ------------------------------ set_insert ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_insert(generic_set_t *set, const void *data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (set != NULL);
  assert (data != NULL);
  /* Do not allow the insertion of duplicates. */
  if (generic_set_is_member(set, data)) {
    return 1;
  }

  /* Insert the data. */
  return generic_list_ins_next(set, generic_list_tail(set), data);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ set_remove ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_remove(generic_set_t  *set, void **data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif
  assert (set != NULL);

  generic_list_elmt_t *member;
  generic_list_elmt_t *prev;

  /* Find the member to remove. */
  prev = NULL;

  for (member = generic_list_head(set);
       member != NULL;
       member = generic_list_next(member)) {

    if (set->compare(*data, generic_list_data(member)) == 0) {
      break;
    }

    prev = member;
  }

  /* Return if the member was not found. */
  if (member == NULL) {
    return -1;
  }

  /* Remove the member. */
  return generic_list_rem_next(set, prev, data);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------- set_union ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_union(generic_set_t  *setu,
		      generic_set_t  *set1,
		      generic_set_t  *set2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (set1 != NULL);
  assert (set2 != NULL);

  generic_list_elmt_t *member;
  void *data;

  /* Initialize the set for the union. */
  generic_set_init(setu, set1->compare, set1->build, set1->destroy);

  /* Insert the members of the first set. */
  for (member = generic_list_head(set1);
       member != NULL;
       member = generic_list_next(member)) {

    data = generic_list_data(member);
    if (generic_list_ins_next(setu, generic_list_tail(setu), data) != 0) {
      generic_set_destroy(setu);
      return -1;
   }
  }

  /* Insert the members of the second set. */
  for (member = generic_list_head(set2); member != NULL;
       member = generic_list_next(member)) {

    if (generic_set_is_member(set1, generic_list_data(member))) {
      /* Do not allow the insertion of duplicates. */
      continue;
    }
    else {
      data = generic_list_data(member);
      if (generic_list_ins_next(setu, generic_list_tail(setu), data) != 0) {
	generic_set_destroy(setu);
	return -1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  --------------------------- set_intersection ---------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_intersection(generic_set_t  *seti,
			     generic_set_t  *set1,
			     generic_set_t  *set2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t  *member;
  void *data;

  /* Initialize the set for the intersection. */
  generic_set_init(seti, set1->compare, set1->build, set1->destroy);

  /* Insert the members present in both sets. */
  for (member = generic_list_head(set1); member != NULL;
       member = generic_list_next(member)) {

    if (generic_set_is_member(set2, generic_list_data(member))) {

      data = generic_list_data(member);
      if (generic_list_ins_next(seti, generic_list_tail(seti), data) != 0) {
         generic_set_destroy(seti);
         return -1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ---------------------------- set_difference ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_difference(generic_set_t  *setd,
			   generic_set_t  *set1,
			   generic_set_t  *set2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *member;
  void *data;

  /* Initialize the set for the difference. */
  generic_set_init(setd, set1->compare, set1->build, set1->destroy);

  /* Insert the members from set1 not in set2. */
  for (member = generic_list_head(set1); member != NULL;
       member = generic_list_next(member)) {

    if (!generic_set_is_member(set2, generic_list_data(member))) {
      data = generic_list_data(member);
      if (generic_list_ins_next(setd, generic_list_tail(setd), data) != 0) {

         generic_set_destroy(setd);
         return -1;
      }
    }
  }

  return 0;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- set_is_member ----------------------------  *
*                                                                            *
*****************************************************************************/

bool generic_set_is_member(generic_set_t *set, const void *data)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (set != NULL);
  /* if (generic_set_size(set) == 0) { */
  /*   return false; */
  /* } */

  generic_list_elmt_t *member = generic_list_head(set);
  /* Determine if the data is a member of the set. */
  for (; member != NULL; member = generic_list_next(member)) {
    assert (member != NULL);
    if (set->compare(data, generic_list_data(member)) == 0) {
      return true;
    }
  }

  return false;
}

/*****************************************************************************
*                                                                            *
*  ----------------------------- set_is_subset ----------------------------  *
*                                                                            *
*****************************************************************************/

bool generic_set_is_subset(generic_set_t  *set1,
			   generic_set_t  *set2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  generic_list_elmt_t *member;

  /* Do a quick test to rule out some cases. */
  if (generic_set_size(set1) > generic_set_size(set2)) {
    return false;
  }

  /* Determine if set1 is a subset of set2. */
  for (member = generic_list_head(set1); member != NULL;
       member = generic_list_next(member)) {

    if (!generic_set_is_member(set2, generic_list_data(member))) {
      return false;
    }
  }

  return true;
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ set_is_equal ----------------------------  *
*                                                                            *
*****************************************************************************/

bool generic_set_is_equal(generic_set_t  *set1,
			  generic_set_t  *set2)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  /* Do a quick test to rule out some cases. */
  if (generic_set_size(set1) != generic_set_size(set2)) {
    return false;
  }

  /* Sets of the same size are equal if they are subsets. */
  return generic_set_is_subset(set1, set2);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ set_size ----------------------------  *
*                                                                            *
*****************************************************************************/

int generic_set_size (generic_set_t  *set)
{
#ifdef DEBUG
  printf ("\t\tEnter %s\n", __func__);
#endif

  assert (set != NULL);
  return set->size;
}
