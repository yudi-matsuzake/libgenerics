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

	for( i=0; i<N; i++ )
		queue_enqueue(&q, &i);


	/* treating errors */
	int dequeued;
	gerror_t g;
	for( i=-1; i<=N; i++){
		if( i==-1 )
			g = queue_dequeue(NULL, &dequeued);
		else
			g = queue_dequeue(&q, &dequeued);

		/*
		 * see also all GERROR_ MACROS
		 * defined in generics/gerror.h
		 */
		if(g == GERROR_OK)
			printf("dequeued: %d\n", dequeued);
		printf("gerror: %s\n", gerror_to_str(g));
	}

	queue_destroy(&q);
	return 0;
}
