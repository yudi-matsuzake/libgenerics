#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <generics/queue.h>

#define N 10

typedef struct cord{
	int x, y;
}cord;

int main()
{
	srand(time(NULL));

	int i;
	queue_t q, d, c;
	queue_create(&q, sizeof(int));
	queue_create(&d, sizeof(double));
	queue_create(&c, sizeof(struct cord));

	for( i=0; i<N; i++ ){
		printf("enqueue(q:%zu): %d\n", q.size, i);
		queue_enqueue(&q, &i);

		double f = 1/(double)(i+1);
		printf("enqueue(d:%zu): %lf\n", d.size, f);
		queue_enqueue(&d, &f);

		struct cord cord = { rand()%N, rand()%N };
		printf("enqueue(c:%zu): [%d, %d]\n", c.size, cord.x, cord.y);
		queue_enqueue(&c, &cord);

	}

	printf("---------\n");
	for( i=0; i<N/2; i++ ){
		int n;
		queue_dequeue(&q, &n);
		printf("dequeue(q): %d\n", n);

		double f;
		queue_dequeue(&d, &f);
		printf("dequeue(d): %lf\n", f);

		struct cord cord;
		queue_dequeue(&c, &cord);
		printf("dequeue(c): [%d, %d]\n", cord.x, cord.y);
	}

	queue_destroy(&q);
	queue_destroy(&d);
	queue_destroy(&c);

	return 0;
}
