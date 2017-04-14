#include <stdio.h>
#include <stdlib.h>
#include <generics/map.h>

#define UNUSED(X) ((void)X)

double dzrand()
{
	return (double) rand() / (double) RAND_MAX;
}

double drand()
{
	return	((double) rand() / (double) RAND_MAX)
		*(double) rand();
}

int compare_string_function(void* a, void* b, void* arg)
{
	UNUSED(arg);

	char* a_str = *(char**)a;
	char* b_str = *(char**)b;

	int r = strcmp(a_str, b_str);
	rbcomp_t result;

	if(r < 0)
		result = G_RB_FIRST_IS_SMALLER;
	else if(r == 0)
		result = G_RB_EQUAL;
	else
		result = G_RB_FIRST_IS_GREATER;

	return result;
}

int main()
{

	map_t m;
	map_create(&m, sizeof(char*), sizeof(double));
	map_set_compare_function(&m, compare_string_function, NULL);

	char* keys[] = { "hi", "bye", "barbara", "boris" };
	size_t keys_len = sizeof(keys)/sizeof(char*);

	size_t i;
	for(i = 0; i<keys_len; i++){
		double d = drand();
		map_insert(&m, &keys[i], &d);
		printf("mapping %s -> %lf\n", keys[i], d);
	}

	for(i = 0; i<keys_len; i++){
		double d;
		map_at(&m, &keys[i], &d);
		printf("mapped %s -> %lf\n", keys[i], d);
	}

	map_destroy(&m);

	return 0;
}
