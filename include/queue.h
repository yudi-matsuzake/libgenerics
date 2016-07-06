#ifndef __QUEUE_T_H__
#define __QUEUE_T_H__
#include <stdlib.h>
#include <string.h>

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

void queue_create(struct queue_t* q, size_t member_size);
void queue_enqueue(struct queue_t* q, void* e);
void* queue_dequeue(struct queue_t* q);
void queue_destroy(struct queue_t* q);
void* queue_remove(struct queue_t* q, struct qnode_t* node);

#endif // __QUEUE_T_H__
