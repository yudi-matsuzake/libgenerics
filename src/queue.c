#include "queue.h"

/** Creates a queue and populates the previous
  * allocated structure pointed by `q`;
  *
  * @param q		pointer to a queue structure;
  * @param member_size	size of the elements that will be
  * 			indexed by `q`
  *
  */
void queue_create(struct queue_t* q, size_t member_size)
{
	if(!q) return;

	q->member_size = member_size;
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}

/** Enqueues the element pointed by `e` in the
  * queue `q`.
  *
  * @param q	pointer to a queue structure;
  * @param e	pointer to the element that will be indexed
  * 		by q.
  *
  */
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

/** Dequeues the first element of the queue `q`
  *
  * @param q	pointer to a queue structure;
  * 
  * @return a pointer to the element that must be freed;
  */
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

/** Removes the element `node` of the queue `q`.
  * 
  * @param q	pointer to a queue structure;
  * @param node	element to be removed from the queue
  *
  * @return a pointer to the value of the node just removed
  *
  */
void* queue_remove(struct queue_t* q, struct qnode_t* node)
{
	if(!q || !node) return NULL;
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
	return ptr;
}

/** Deallocate the nodes of the queue q.
  * This function WILL NOT deallocate the pointer q.
  *
  * @param q	pointer to a queue structure;
  *
  */
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
