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

#ifndef __PRIORITY_QUEUE_T_H__
#define __PRIORITY_QUEUE_T_H__
#include <stdlib.h>
#include "gerror.h"
#include "vector.h"

typedef enum{
	G_PQUEUE_FIRST_PRIORITY = -1,	/* a > b  */
	G_PQUEUE_EQUAL_PRIORITY,	/* a == b */
	G_PQUEUE_SECOND_PRIORITY	/* a < b  */
}queue_priority_t;

typedef int (*compare_function)(void* a, void* b, void* arg);

typedef struct priority_queue_t{
	size_t size;
	size_t member_size;

	compare_function compare;
	void* compare_argument;
	struct vector_t queue;
} priority_queue_t;

typedef struct priority_queue_t pqueue_t;

gerror_t pqueue_create(	pqueue_t* p, size_t member_size);
gerror_t pqueue_destroy(pqueue_t* p);
gerror_t pqueue_set_compare_function(pqueue_t* p, compare_function function, void* argument);
gerror_t pqueue_add(pqueue_t* p, void* e);
gerror_t pqueue_max_priority(pqueue_t* p, void* e);
gerror_t pqueue_extract(pqueue_t* p, void* e);

#endif
