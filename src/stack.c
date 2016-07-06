#include "stack.h"

void stack_create(struct stack_t* s, size_t member_size)
{
	if(!s) return;

	s->member_size = member_size;
	s->size = 0;
	s->head = NULL;
}

void stack_push(struct stack_t* s, void* e)
{
	if(!s) return;

	struct snode_t* new_node = (snode_t*) malloc(sizeof(snode_t));
	new_node->data = malloc(sizeof(s->member_size));
	new_node->prev = NULL;
	new_node->next = s->head;
	memcpy(new_node->data, e, s->member_size);

	s->head = new_node;

	s->size++;
}

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
