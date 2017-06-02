#include <stdio.h>
#include <generics/dict.h>

int main()
{

	dict_t d;
	dict_create(&d, sizeof(long), G_DICT_CASE_INSENSITIVE);

	long i;
	i = rand();
	printf("barbara -> %ld\n", i);
	dict_insert(&d, "barbara", &i);

	i = rand();
	printf("Barbara -> %ld\n", i);
	dict_insert(&d, "Barbara", &i);

	i = rand();
	printf("BoriS -> %ld\n", i);
	dict_insert(&d, "BoriS", &i);

	i = rand();
	printf("boris -> %ld\n", i);
	dict_insert(&d, "boris", &i);

	i = rand();
	printf("linux -> %ld\n", i);
	dict_insert(&d, "linux", &i);

	i = rand();
	printf("LINUX -> %ld\n", i);
	dict_insert(&d, "LINUX", &i);

	dict_at(&d, "barbara", &i);
	printf("barbara = %ld\n", i);

	dict_at(&d, "boris", &i);
	printf("boris = %ld\n", i);

	dict_at(&d, "linux", &i);
	printf("linux = %ld\n", i);

	dict_destroy(&d);

	return 0;
}
