#include <stdio.h>
#include <generics/vector.h>

#define N 20

void print_v(vector_t* v){
	int i;
	printf("v(%.2d)-> ", v->buffer_size/v->member_size);

	for(i=0; i<v->size; i++){
		int tmp = ((int*)v->data)[i];
		printf(	"[%.2d]", tmp);
	}

	printf("\n");
}

int main()
{

	vector_t v;

	vector_create(&v, N, sizeof(int));

	int i;
	for(i=0; i<N; i++)
		vector_add(&v, &i);

	for ( i = 0; i<=N ; i++ ){
		print_v(&v);
		vector_resize_buffer(&v, N-i-1);
	}

	vector_destroy(&v);
	return 0;
}
