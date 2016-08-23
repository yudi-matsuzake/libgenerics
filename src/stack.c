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
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_ELEMENT in case that `e` is empty.
  */
gerror_t stack_create(struct stack_t* s, size_t member_size)
{
	if(!s) return GERROR_NULL_STRUCTURE;

	s->member_size = member_size;
	s->size = 0;
	s->head = NULL;
	return GERROR_OK;
}

/** Add the element `e` in the beginning of the stack `s`.
  *
  * @param s		pointer to a stack structure;
  * @param e	pointer to the element that will be indexed
  * 		by s.
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `s` is a NULL
  */
gerror_t stack_push(struct stack_t* s, void* e)
{
	if(!s) return GERROR_NULL_STRUCTURE;

	struct snode_t* new_node = (snode_t*) malloc(sizeof(snode_t));
	if( s->member_size )
		new_node->data = malloc(s->member_size);
	else
		new_node->data = NULL;
	new_node->prev = NULL;
	new_node->next = s->head;

	if(s->member_size && e)
		memcpy(new_node->data, e, s->member_size);

	s->head = new_node;

	s->size++;

	return GERROR_OK;
}

/** Pops the first element of the stack `s`.
  *
  * @param s	pointer to a stack structure;
  * @param e	pointer to the previous allocated element
  * 
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `s` is a NULL
  * 		GERROR_NULL_HEAD in case that the head `s->head`
  * 		GERROR_TRY_REMOVE_EMPTY_STRUCTURE in case that `s`
  * 			is empty
  */
gerror_t stack_pop (struct stack_t* s, void* e)
{
	if(!s)		return GERROR_NULL_STRUCTURE;
	if(!s->head)	return GERROR_NULL_HEAD;
	if(!s->size)	return GERROR_TRY_REMOVE_EMPTY_STRUCTURE;

	void* ptr = s->head->data;
	struct snode_t* old_node = s->head;

	s->head = s->head->next;

	if( s->head )
		s->head->prev = NULL;

	s->size--;
	free(old_node);

	if(s->member_size && e)
		memcpy(e, ptr, s->member_size);
	if(ptr)
		free(ptr);
	return GERROR_OK;
}


/** Deallocates the nodes of the structure pointed
  * by `s`. This function WILL NOT deallocate the
  * pointer `q`.
  *
  * @param s		pointer to a stack structure;
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `s` is a NULL
  */
gerror_t stack_destroy(struct stack_t* s)
{
	if(!s) return GERROR_NULL_STRUCTURE;

	struct snode_t* i, *j;

	for( i=s->head; i!=NULL; i=j ){
		j = i->next;
		if(i->data)
			free(i->data);
		free(i);
	}

	return GERROR_OK;
}
