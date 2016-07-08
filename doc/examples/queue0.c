#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <generics/queue.h>

#define N 10

int main()
{
	srand(time(NULL));

	int i;
	queue_t q;
	queue_create(&q, sizeof(int));

	for( i=0; i<N; i++ ){
		printf("enqueue(q:%zu): %d\n", q.size, i);
		queue_enqueue(&q, &i);
	}

	printf("---------\n");
	for( i=0; i<N/2; i++ ){
		int* n = queue_dequeue(&q);
		printf("dequeue(q): %d\n", *n);
		free(n);
	}

	queue_destroy(&q);
	return 0;
}
