#include <stdio.h>
#include <generics/vector.h>

#define N 50

int main()
{

	vector_t v;

	vector_create(&v, 0, sizeof(int));

	int i;
	for ( i = 0; i<N ; i++ ){
		vector_add(&v, &i);
		printf("add: vec[%2d](bufsize: %3d) = %2d\n",
				i,
				v.buffer_size,
				((int*)v.data)[i]);
	}

	for ( i = 0; i<N ; i++ ){
		int* tmp = (int*)vector_at(&v, i);
		printf("v[%2d] = %2d\n", i, *tmp);
	}

	vector_destroy(&v);

	return 0;
}
