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
#include "queue.h"

/** Creates a queue and populates the previous
  * allocated structure pointed by `q`;
  *
  * @param q		pointer to a queue structure;
  * @param member_size	size of the elements that will be
  * 			indexed by `q`
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `q` is a NULL
  * 		pointer
  */
gerror_t queue_create(struct queue_t* q, size_t member_size)
{
	if(!q) return GERROR_NULL_STRUCTURE;

	q->member_size = member_size;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;

	return GERROR_OK;
}

/** Enqueues the element pointed by `e` in the
  * queue `q`.
  *
  * @param q	pointer to a queue structure;
  * @param e	pointer to the element that will be indexed
  * 		by q.
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `q` is a NULL
  * 		pointer
  *
  */
gerror_t queue_enqueue(struct queue_t* q, void* e)
{
	if(!q) return GERROR_NULL_STRUCTURE;
	
	struct qnode_t* new_node = (qnode_t*) malloc(sizeof(qnode_t));

	if(q->member_size)
		new_node->data = malloc(q->member_size);
	else
		new_node->data = NULL;

	new_node->next = new_node->prev = NULL;

	if(q->member_size && e)
		memcpy(new_node->data, e, q->member_size);

	if(q->tail){
		q->tail->next = new_node;
		new_node->prev = q->tail;
		q->tail = new_node;
	}else{
		q->head = new_node;
		q->tail = new_node;
	}

	q->size++;

	return GERROR_OK;
}

/** Dequeues the first element of the queue `q`
  *
  * @param q	pointer to a queue structure;
  * @param e	pointer to the previous allocated element
  * 		memory that will be write with de dequeued
  * 		element.
  * 
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_HEAD in case that the head `q->head`
  * 		is a null pointer.
  * 		GERROR_NULL_STRUCURE in case `q` is a NULL
  * 		pointer
  * 		GERROR_TRY_REMOVE_EMPTY_STRUCTURE in case
  * 		that `q` has no element.
  */
gerror_t queue_dequeue(struct queue_t* q, void* e)
{
	if(!q)		return GERROR_NULL_STRUCTURE;
	if(!q->head)	return GERROR_NULL_HEAD;
	if(!q->size)	return GERROR_TRY_REMOVE_EMPTY_STRUCTURE;

	void* ptr = q->head->data;
	struct qnode_t* old_node = q->head;

	q->head = q->head->next;

	if( q->head )
		q->head->prev = NULL;
	else
		q->tail = NULL;

	q->size--;
	free(old_node);
	if(q->member_size && e)
		memcpy(e, ptr, q->member_size);
	if(ptr)
		free(ptr);
	return GERROR_OK;
}

/** Removes the element `node` of the queue `q`.
  * 
  * @param q	pointer to a queue structure;
  * @param node	element to be removed from the queue
  * @param e	pointer to the memory that will be
  * 		write with the removed element
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `q` is a NULL
  * 		pointer
  * 		GERROR_NULL_NODE in case `node` is NULL;
  * 		GERROR_TRY_REMOVE_EMPTY_STRUCTURE in case
  * 		that `q` has no element.
  *
  */
gerror_t queue_remove(struct queue_t* q, struct qnode_t* node, void* e)
{
	if(!q)		return GERROR_NULL_STRUCTURE;
	if(!node)	return GERROR_NULL_NODE;
	if(!q->size)	return GERROR_TRY_REMOVE_EMPTY_STRUCTURE;

	if(node->prev)
		node->prev->next = node->next;
	else
		q->head = node->next;

	if(node->next)
		node->next->prev = node->prev;
	else
		q->tail = node->prev;

	q->size--;
	node->next = node->prev = NULL;
	void* ptr = node->data;
	free(node);
	
	if(q->member_size && e)
		memcpy(e, ptr, q->member_size);
	if(ptr)
		free(ptr);
	return GERROR_OK;
}

/** Deallocate the nodes of the queue q.
  * This function WILL NOT deallocate the pointer q.
  *
  * @param q	pointer to a queue structure;
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `q` is a NULL
  * 		pointer
  */
gerror_t queue_destroy(struct queue_t* q)
{
	if(!q) return GERROR_NULL_STRUCTURE;

	struct qnode_t* i, *j;

	for( i=q->head; i!=NULL; i=j ){
		j = i->next;
		if(i->data)
			free(i->data);
		free(i);
	}

	return GERROR_OK;
}
