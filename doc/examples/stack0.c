#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stack.h"

#define N 10

int main()
{
	srand(time(NULL));
	stack_t s;
	stack_create(&s, sizeof(int));

	int i;

	for(i=0; i<N; i++){
		int temp = rand()%N; 
		printf("push: %d\n", temp);
		stack_push(&s, &temp);
	}

	int* data;
	while( (data = stack_pop(&s)) ){
		int poped = *data;
		printf("pop: %d\n", poped);
		free(data);
	}

	stack_destroy(&s);

	return 0;
}
