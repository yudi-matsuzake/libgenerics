#include <stdio.h>
#include <generics/dict.h>

int main()
{

	dict_t d;
	dict_create(&d, sizeof(long));

	long i;
	i = rand();
	printf("barbara -> %ld\n", i);
	dict_insert(&d, "barbara", &i);

	i = rand();
	printf("boris -> %ld\n", i);
	dict_insert(&d, "boris", &i);

	i = rand();
	printf("linux -> %ld\n", i);
	dict_insert(&d, "linux", &i);

	dict_at(&d, "barbara", &i);
	printf("barbara = %ld\n", i);

	dict_at(&d, "boris", &i);
	printf("boris = %ld\n", i);

	dict_at(&d, "linux", &i);
	printf("linux = %ld\n", i);

	dict_destroy(&d);

	return 0;
}
