#include <stdio.h>
#include <generics/vector.h>

int main()
{

	vector_t v;
	g_assert(vector_create(&v, 0, sizeof(int)));

	int n;
	g_assert(vector_at(&v, 666, &n));

	g_assert(vector_destroy(&v));
	return 0;
}
