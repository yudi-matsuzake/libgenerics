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

#include "priority_queue.h"

#define PARENT(i) ((i-1)/2)
#define LEFT(i)   (((i+1)*2)-1)
#define RIGHT(i)  (LEFT(i)+1)

void nswap (void* a, void* b, size_t n);
int default_compare_function(void* a, void* b, void* arg);
void max_heapify(pqueue_t* p, size_t i);

/** Populates the `p` structure and inicialize it.
  * A priority queue needs a compare_function. The default function
  * will only work for char, int and long. If you need a double or float
  * you need to implement the compare function and set with the
  * function `pqueue_set_compare_function`
  *
  * @param p		previous allocated pqueue_t struct
  * @param member_size	size in bytes of the indexed elements
  * @param function	comparison function callback that has
  * 			the following prototype:
  * 			int compare(void* a, void* b)
  * 			the a and b are the arguments
  * 			returns -1 if `a` has priority BIG than `B`
  * 			returns 0 if `a` has priority EQUAL than `B`
  * 			return 1 if `a` has priority LE
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `p` is a NULL
  */
gerror_t pqueue_create (pqueue_t* p, size_t member_size)
{
	if(!p) return GERROR_NULL_STRUCTURE;
	p->member_size = member_size;
	p->compare = default_compare_function;
	p->compare_argument = &p->member_size;
	vector_create(&p->queue, 0, member_size);
	return GERROR_OK;
}

/** Destroy (i.e. desallocates) the `p` structure fields.
  * TODO: A more datailed description of pqueue_destroy.
  *
  * @param p	previous allocated pqueue_t struct
  *
  * @return TODO
  */
gerror_t pqueue_destroy (pqueue_t* p)
{
	if(!p) return GERROR_NULL_STRUCTURE;
	p->size = 0;
	p->member_size = 0;
	p->compare = NULL;
	p->compare_argument = NULL;
	vector_destroy(&p->queue);
	return GERROR_OK;
}

/** Change the default comparison function of the priority queue `p` by
  * `function` with the argument `argument`.
  *
  * @param p		previous allocated pqueue_t struct
  * @param function	comparison function callback that has
  * 			the following prototype:
  * 			int compare(void* a, void* b)
  * 			the a and b are the arguments
  * 			returns -1 if `a` has priority BIG than `B`
  * 			returns 0 if `a` has priority EQUAL than `B`
  * 			return 1 if `a` has priority LE
  * @param argument	allocated pqueue_t struct
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `t` is a NULL
  */
gerror_t pqueue_set_compare_function (pqueue_t* p, compare_function function, void* argument)
{
	if(!p) return GERROR_NULL_STRUCTURE;
	p->compare = function;
	p->compare_argument = argument;
	return GERROR_OK;
}

/** Adds an element in the queue and max heap the queue.
  * TODO: A more datailed description of pqueue_add.
  *
  * @param p	previous allocated pqueue_t struct
  * @param e	the element to be added
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `t` is a NULL
  */
gerror_t pqueue_add (pqueue_t* p, void* e)
{
	if(!p) return GERROR_NULL_STRUCTURE;

	vector_add( &p->queue, e );
	p->size = p->queue.size;
	size_t i = p->size - 1;

	if(i>0){
		void* child, *parent;
		do{
			child = vector_ptr_at(&p->queue, i),
			parent = vector_ptr_at(&p->queue, PARENT(i));
			queue_priority_t priority;
			priority = p->compare(child, parent, p->compare_argument);
			
			if (i>0 && priority == G_PQUEUE_FIRST_PRIORITY)
				nswap(child, parent, p->member_size);
			i = PARENT(i);
		}while ( i > 0 && i < p->size );
	}

	return GERROR_OK;
}

/** Returns and does not remove the highest priority of the queue.
  * TODO: A more datailed description of pqueue_max_priority.
  *
  * @param p	previous allocated pqueue_t struct
  * @param e	pointer to previous allocated variable
  * 		with `member_size` size that will receive
  * 		a copy of the highest priority element of the queue.
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_ACESS_OUT_OF_BOUND in case the queue is empty
  * 		GERROR_NULL_STRUCURE in case `t` is a NULL
  */
gerror_t pqueue_max_priority (pqueue_t* p, void* e)
{
	if(!p) return GERROR_NULL_STRUCTURE;
	if(p->size == 0) return GERROR_ACCESS_OUT_OF_BOUND;
	vector_at( &p->queue, 0, e );
	return GERROR_OK;
}

/** Extracts the highest priority element in the queue and
  * writes in `e` pointer.
  *
  * @param p	previous allocated pqueue_t struct
  * @param e	pointer to previous allocated variable
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_ACESS_OUT_OF_BOUND in case the queue is empty
  * 		GERROR_NULL_STRUCURE in case `t` is a NULL
  */
gerror_t pqueue_extract (pqueue_t* p, void* e)
{
	if(!p) return GERROR_NULL_STRUCTURE;
	if(p->size == 0) return GERROR_ACCESS_OUT_OF_BOUND;

	vector_at( &p->queue, 0, e );
	void* ptr = vector_ptr_at( &p->queue, p->size-1 );
	vector_set_elem_at( &p->queue, 0, ptr );

	vector_resize_buffer( &p->queue, p->size-1 );
	p->size = p->queue.size;

	max_heapify( p, 0 );
	
	return GERROR_OK;
}

/*
 * auxiliar function to swap n bytes in
 * a to b
 */
void nswap (void* a, void* b, size_t n)
{
	void* cup = malloc(n);
	memcpy(cup, a, n);
	memcpy(a, b, n);
	memcpy(b, cup, n);
	free(cup);
}

/*
 * the default comparison function. Just compare
 * like to long
 */
int default_compare_function(void* a, void* b, void* arg)
{
	size_t size = *(size_t*)arg;

	long la = 0;
	long lb = 0;

	switch(size){
	case sizeof(int):
		la = *(int*)a;
		lb = *(int*)b;
		break;
	case sizeof(char):
		la = *(char*)a;
		lb = *(char*)b;
		break;
	case sizeof(long):
		la = *(long*)a;
		lb = *(long*)b;
		break;
	}
	return (la > lb) - (la < lb);
}

/*
 * classical heapify algorithm
 */
void max_heapify(pqueue_t* p, size_t i)
{
	size_t left = LEFT(i);
	size_t right = RIGHT(i);
	size_t largest;

	void* ptr_i = vector_ptr_at(&p->queue, i);
	void* pleft = vector_ptr_at(&p->queue, left);
	void* pright = vector_ptr_at(&p->queue, right);
	void* plargest;

	if( left < p->size &&
	    p->compare(pleft, ptr_i, p->compare_argument) == G_PQUEUE_FIRST_PRIORITY ) {
		largest = left;
		plargest = pleft;
	}else{
		largest = i;
		plargest = ptr_i;
	}

	if( right < p->size &&
	    p->compare(pright, plargest, p->compare_argument) == G_PQUEUE_FIRST_PRIORITY){
		largest = right;
		plargest = pright;
	}

	if( largest != i ){
		nswap(ptr_i, plargest, p->member_size);
		max_heapify(p, largest);
	}
}
