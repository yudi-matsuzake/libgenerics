#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <generics/stack.h>

#define N 10

int main()
{
	srand(time(NULL));
	stack_t s;
	stack_create(&s, 0);

	printf("push\n");
	stack_push(&s, NULL);

	while( (stack_pop(&s, NULL) == GERROR_OK) ){
		printf("pop\n");

		if( rand()%10 < 6 ) {
			printf("push\n");
			stack_push(&s, NULL);
		}
	}

	stack_destroy(&s);
	return 0;
}
