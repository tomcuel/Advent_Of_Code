/*****************************************************************************
*                                                                            *
*  ------------------------------- stack.c --------------------------------  *
*                                                                            *
*****************************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "generic_stack.h"

#ifdef DEBUG
#pragma message "activate debug mode generic_stack"
#endif

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_init ------------------------------  *
*                                                                            *
*****************************************************************************/

void generic_stack_init (generic_stack_t *stack,
			 int (*compare)(const void *key1, const void *key2),
			 void* (*build) (const void *data),
			 void (*destroy) (void *data))
{
  generic_list_init (stack, compare, build, destroy);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_destroy ---------------------------  *
*                                                                            *
*****************************************************************************/

void generic_stack_destroy (generic_stack_t *stack)
{
  generic_list_destroy (stack);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_push ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_stack_push(generic_stack_t *stack, const void *data) {

  /* Push the data onto the stack. */
  return generic_list_ins_next(stack, NULL, data);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_pop -------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_stack_pop(generic_stack_t *stack, void **data) {

  /* Pop the data off the stack. */
  return generic_list_rem_next(stack, NULL, data);
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_peek ------------------------------  *
*                                                                            *
*****************************************************************************/

void* generic_stack_peek(generic_stack_t *stack) {

  /* Get the data on the top of the stack. */
  if (stack->size == 0) {
    return NULL;
  }
  else {
    return stack->head->data;
  }
}

/*****************************************************************************
*                                                                            *
*  ------------------------------ stack_size ------------------------------  *
*                                                                            *
*****************************************************************************/

int generic_stack_size(generic_stack_t *stack) {
  return generic_list_size (stack);
}
