#include "queue.h"

void queue_create(struct queue_t* q, size_t member_size)
{
	if(!q) return;

	q->member_size = member_size;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}

void queue_enqueue(struct queue_t* q, void* e)
{
	if(!q) return;

	struct qnode_t* new_node = (qnode_t*) malloc(sizeof(qnode_t));
	new_node->data = malloc(sizeof(q->member_size));
	new_node->next = new_node->prev = NULL;
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
}

void* queue_dequeue(struct queue_t* q)
{
	if(!q || !q->head) return NULL;

	void* ptr = q->head->data;
	struct qnode_t* old_node = q->head;

	q->head = q->head->next;

	if( q->head )
		q->head->prev = NULL;
	else
		q->tail = NULL;

	q->size--;
	free(old_node);
	return ptr;
}


void queue_destroy(struct queue_t* q)
{
	if(!q) return;

	struct qnode_t* i, *j;

	for( i=q->head; i!=NULL; i=j ){
		j = i->next;
		free(i->data);
		free(i);
	}
}
