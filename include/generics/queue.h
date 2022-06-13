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
#ifndef __QUEUE_T_H__
#define __QUEUE_T_H__
#include <stdlib.h>
#include <string.h>
#include "gerror.h"

/** queue node.
  *
  */
typedef struct qnode_t{
	struct qnode_t* next;
	struct qnode_t* prev;

	void* data;
}qnode_t;

/** Represents a queue structure.
  */
typedef struct queue_t{
	size_t size;
	size_t member_size;
	struct qnode_t* head;
	struct qnode_t* tail;
}queue_t;

gerror_t queue_create(struct queue_t* q, size_t member_size);
gerror_t queue_enqueue(struct queue_t* q, void* e);
gerror_t queue_dequeue(struct queue_t* q, void* e);
gerror_t queue_destroy(struct queue_t* q);
gerror_t queue_remove(struct queue_t* q, struct qnode_t* node, void* e);

#endif
