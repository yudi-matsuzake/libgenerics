#include <stdio.h>
#include <string.h>
#include <generics/priority_queue.h>

int priority_string(void* a, void* b, void* arg)
{
	(void)arg;
	char* str_a = *(char**)a;
	char* str_b = *(char**)b;

	int res = strcmp(str_a, str_b);
	if(res == 0)
		return G_PQUEUE_EQUAL_PRIORITY;
	else if(res < 0)
		return G_PQUEUE_FIRST_PRIORITY;
	else
		return G_PQUEUE_SECOND_PRIORITY;

}

int main()
{

	char* str[] = { "yudi", "barbara", "boris", "raul", "tiago", "tutu", "pedrita"};
	size_t len = sizeof(str)/sizeof(char*);

	priority_queue_t p;
	
	pqueue_create(&p, sizeof(char*));
	pqueue_set_compare_function(&p, priority_string, NULL);

	size_t i;
	for ( i = 0; i<len ; i++ ){
		pqueue_add(&p, &str[i]);
		printf("add: %s\n", str[i]);
	}

	for ( i = 0; i<len ; i++ ){
		char* temp;
		pqueue_extract(&p, &temp);
		printf("extracted: %s\n",temp);
	}
	
	pqueue_destroy(&p);
	return 0;
}
