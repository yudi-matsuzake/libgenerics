#ifndef __STACK_T_H__
#define __STACK_T_H__
#include <stdlib.h>
#include <string.h>

typedef struct snode_t{
	struct snode_t* next;
	struct snode_t* prev;

	void* data;
}snode_t;

typedef struct stack_t{
	size_t size;
	size_t member_size;
	struct snode_t* head;
}stack_t;

void stack_create(struct stack_t* q, size_t member_size);
void stack_push(struct stack_t* q, void* e);
void* stack_pop(struct stack_t* q);
void stack_destroy(struct stack_t* q);

#endif // __STACK_T_H__
