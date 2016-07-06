#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "queue.h"

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

	int* ptr;
	ptr = queue_remove(&q, q.head->next);
	printf("removed %d\n", *ptr);
	free(ptr);
	
	ptr = queue_remove(&q, q.head);
	printf("removed %d\n", *ptr);
	free(ptr);

	ptr = queue_remove(&q, q.tail);
	printf("removed %d\n", *ptr);
	free(ptr);

	ptr = queue_remove(&q, q.head);
	printf("removed %d\n", *ptr);
	free(ptr);

	queue_destroy(&q);
	return 0;
}
