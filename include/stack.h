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
#ifndef __STACK_T_H__
#define __STACK_T_H__
#include <stdlib.h>
#include <string.h>

/** node of a stack
  */
typedef struct snode_t{
	struct snode_t* next;
	struct snode_t* prev;

	void* data;
}snode_t;

/** represents the stack structure.
  */
typedef struct stack_t{
	size_t size;
	size_t member_size;
	struct snode_t* head;
}stack_t;

void stack_create(struct stack_t* q, size_t member_size);
void stack_push(struct stack_t* q, void* e);
void* stack_pop(struct stack_t* q);
void stack_destroy(struct stack_t* q);

#endif
