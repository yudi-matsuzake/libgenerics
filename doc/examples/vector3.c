#include <stdio.h>
#include <generics/vector.h>

int main()
{

	vector_t buffer;
	gerror_t result;
	if((result = vector_create(&buffer, 0, sizeof(char))) != GERROR_OK){
		fprintf(stderr, "Cannot create vector\n");
		fprintf(stderr, "Error: %s\n", gerror_to_str(result));
		return 1;
	}

	char buffer_temp[BUFSIZ];
	size_t n_data;

	while( (n_data = fread(buffer_temp, sizeof(char), BUFSIZ, stdin)) )
		vector_append_buffer(&buffer, buffer_temp, n_data);

	vector_append( &buffer, &buffer );

	fprintf(stdout, "output duplicated (%lu bytes):\n", buffer.size);
	fwrite(buffer.data, buffer.member_size, buffer.size, stdout);

	vector_destroy( &buffer );

	return 0;
}
