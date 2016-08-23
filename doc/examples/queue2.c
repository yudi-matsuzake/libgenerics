#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <generics/queue.h>

int main()
{
	srand(time(NULL));

	int i;
	queue_t q;
	queue_create(&q, sizeof(int));

	i = 1;
	queue_enqueue(&q, &i);
	i = 2;
	queue_enqueue(&q, &i);
	i = 3;
	queue_enqueue(&q, &i);
	i = 4;
	queue_enqueue(&q, &i);

	int n;
	if(queue_remove(&q, q.head->next, &n) == GERROR_OK)
		printf("removed %d\n", n);
	if(queue_remove(&q, q.head, &n) == GERROR_OK)
		printf("removed %d\n", n);
	if(queue_remove(&q, q.tail, &n) == GERROR_OK)
		printf("removed %d\n", n);
	if(queue_remove(&q, q.head, &n) == GERROR_OK)
		printf("removed %d\n", n);
	
	queue_destroy(&q);
	return 0;
}
