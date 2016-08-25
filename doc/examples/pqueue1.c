#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <generics/priority_queue.h>

#define N 10

double drand(){
	return (double)(rand()%N) + ((double)rand())/(double)RAND_MAX;
}

int double_compare(void* a, void* b, void* arg)
{
	(void)arg;
	double double_a = *(double*)a;
	double double_b = *(double*)b;

	if( double_a > double_b )	return G_PQUEUE_FIRST_PRIORITY;
	else if( double_a < double_b )	return G_PQUEUE_SECOND_PRIORITY;
	else				return G_PQUEUE_EQUAL_PRIORITY;
}

int main()
{
	srand(time(NULL));
	pqueue_t p;
	pqueue_create(&p, sizeof(double));
	pqueue_set_compare_function(&p, double_compare, NULL);

	int i;
	for ( i = 0; i<N ; i++ ){
		double temp = drand();
		pqueue_add(&p, &temp);
		printf("added: %lf\n", temp);
	}

	for ( i = 0; i<N ; i++ ){
		double temp;
		pqueue_extract(&p, &temp);
		printf("extracted: %lf\n", temp);
	}

	pqueue_destroy(&p);
	return 0;
}
