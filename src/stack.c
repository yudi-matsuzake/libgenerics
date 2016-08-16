/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * For more information, please refer to <http://unlicense.org/>
 */
#include "stack.h"

/** Creates a stack and populates the previous
  * allocated structure pointed by `s`;
  *
  * @param s		pointer to a stack structure;
  * @param member_size	size of the elements that will be
  * 			indexed by `s`
  *
  */
void stack_create(struct stack_t* s, size_t member_size)
{
	if(!s) return;

	s->member_size = member_size;
	s->size = 0;
	s->head = NULL;
}

/** Add the element `e` in the beginning of the stack `s`.
  *
  * @param s		pointer to a stack structure;
  * @param e	pointer to the element that will be indexed
  * 		by s.
  *
  */
void stack_push(struct stack_t* s, void* e)
{
	if(!s) return;

	struct snode_t* new_node = (snode_t*) malloc(sizeof(snode_t));
	new_node->data = malloc(s->member_size);
	new_node->prev = NULL;
	new_node->next = s->head;
	memcpy(new_node->data, e, s->member_size);

	s->head = new_node;

	s->size++;
}

/** Pops the first element of the stack `s`.
  *
  * @param s		pointer to a stack structure;
  * 
  * @return a pointer to the element that must be freed;
  */
void* stack_pop (struct stack_t* s)
{
	if(!s || !s->head) return NULL;

	void* ptr = s->head->data;
	struct snode_t* old_node = s->head;

	s->head = s->head->next;

	if( s->head )
		s->head->prev = NULL;

	s->size--;
	free(old_node);
	return ptr;
}


/** Deallocates the nodes of the structure pointed
  * by `s`. This function WILL NOT deallocate the
  * pointer `q`.
  *
  * @param s		pointer to a stack structure;
  *
  */
void stack_destroy(struct stack_t* s)
{
	if(!s) return;

	struct snode_t* i, *j;

	for( i=s->head; i!=NULL; i=j ){
		j = i->next;
		free(i->data);
		free(i);
	}
}
