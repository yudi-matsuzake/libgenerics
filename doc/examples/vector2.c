#include <stdio.h>
#include <generics/vector.h>

#define N 50

int main()
{

	vector_t v;
	size_t new_buf_siz = 32;
	printf("Set the min_buf_siz from %lu to %lu\n",
			vector_get_min_buf_siz(), new_buf_siz);
	vector_set_min_buf_siz(new_buf_siz);

	vector_create(&v, 0, sizeof(int));

	int i;
	for ( i = 0; i<N ; i++ ){
		vector_add(&v, &i);
		printf("add: vec[%2d](bufsize: %3lu) = %2d\n",
				i,
				v.buffer_size,
				((int*)v.data)[i]);
	}

	for ( i = 0; i<N ; i++ ){
		int tmp;
		vector_at(&v, i, &tmp);
		printf("v[%2d] = %2d\n", i, tmp);
	}

	vector_destroy(&v);

	return 0;
}
