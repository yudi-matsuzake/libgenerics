#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <generics/priority_queue.h>

#define N 15

void print(pqueue_t* p)
{
	size_t i;
	printf("(tam: %lu)", p->size);
	for ( i = 0; i<p->size ; i++ ){
		int temp;
		vector_at(&p->queue, i, &temp);
		printf("[%d]", temp);
	}
	printf("\n");
}

int main()
{
	srand(time(NULL));
	pqueue_t p;
	pqueue_create(&p, sizeof(int));

	int i;
	for ( i = 0; i<N ; i++ ){
		int temp = rand()%N;
		pqueue_add(&p, &temp);
		printf("added: %d\n", temp);
	}

	for ( i = 0; i<N ; i++ ){
		int temp;
		pqueue_extract(&p, &temp);
		printf("[%lu]extracted: %d\n", p.size, temp);
	}

	pqueue_destroy(&p);
	return 0;
}
