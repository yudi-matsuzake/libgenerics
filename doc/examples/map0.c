#include <stdio.h>
#include <generics/map.h>
#include <time.h>

#define N 10

void swap(long* a, long* b)
{
	long cup = *a;
	*a = *b;
	*b = cup;
}

void randomize(long r[N])
{
	long i;
	for(i = 0; i<N; i++)
		r[i] = i;

	for(i = 0; i<N; i++)
		swap(&r[i], &r[rand()%N]);

}

int main()
{
	srand(time(NULL));

	map_t m;
	map_create(&m, sizeof(long), sizeof(long));
	long r[N];
	randomize(r);

	long i;
	for(i=0; i<N; i++){
		printf("mapping %ld -> %ld\n", r[i], i);
		map_insert(&m, &r[i], &i);
	}

	randomize(r);
	for(i=0; i<N/2; i++){
		printf("overwrite mapping %ld -> %ld\n", r[i], i);
		map_insert(&m, &r[i], &i);
	}

	randomize(r);
	for(i = 0; i<N/2; i++){
		printf("removing... %ld\n", r[i]);
		map_remove(&m, &r[i], NULL);
	}

	randomize(r);

	long e;
	for(i = 0; i<N; i++){
		printf("mapped %ld -> ", r[i]);
		if(map_at(&m, &r[i], &e) == GERROR_OK)
			printf("%ld\n", e);
		else
			printf("removed\n");
	}

	map_destroy(&m);
	return 0;
}
