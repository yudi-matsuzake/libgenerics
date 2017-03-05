#include <stdio.h>
#include <time.h>
#include <generics/red_black_tree.h>

#define N 10
#define UNUSED(X) ((void)X)


void print_rbt (rbnode_t* node, int t)
{
	if(node != NULL){
		int i;
		for( i = 0; i<t; i++ ){
			printf("|\t");
		}
		printf("%s (%s)\n", *((char**)node->data), (node->color == G_RB_BLACK)?"black":"red");
		
		print_rbt(node->left, t+1);
		print_rbt(node->right, t+1);
	}else{
		int i;
		for( i = 0; i<t; i++ ){
			printf("|\t");
		}
		printf("null\n");
	}
}

int is_a_valid_rbtree(rbnode_t* node, int n, int* count)
{

	if(node){
		int im_black = node->color == G_RB_BLACK;
		n += im_black;

		if(!is_a_valid_rbtree(node->left, n, count))
			return 0;

		if(!is_a_valid_rbtree(node->right, n, count))
			return 0;

		return 1;
	}

	n++;
	if(*count == 0){
		*count = n;
	}

	return n == *count;
}


void swap(char** a, char** b)
{
	char* cup = *a;
	*a = *b;
	*b = cup;
}

void inicialize(int* v, int n)
{
	int i;
	for( i = 0; i<n; i++ ){
		v[i] = i;
	}
}

void randomize(char** v, int n)
{
	int i;
	for( i = 0; i<n; i++ ){
		swap(&v[i], &v[rand()%n]);
	}
}

void vprint(char** v, int n)
{
	int i;
	for( i = 0; i<n-1; i++ ){
		printf("%s, ", v[i]);
	}
	printf("%s\n", v[i]);
}

void rbtree_check(rbtree_t* rbt)
{
	int n = 0;
	if(is_a_valid_rbtree(rbt->root, 0, &n)){
		printf("It is a valid red-black tree\n");
	}
	else{
		printf("It is not a valid red-black tree\n");
		exit(EXIT_FAILURE);
	}
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

	srand(time(NULL));

	rbtree_t rbt;
	rbtree_create(&rbt, sizeof(char*));
	rbtree_set_compare_function(&rbt, compare_string_function, NULL);

	int i;

	/*
	 * insert
	 */
	char* to_insert[N] = {
		"yudi",
		"boris",
		"barbara",
		"pedrita",
		"brazil",
		"english",
		"wilson",
		"root",
		"linux",
		"zoeira"
	};

	randomize(to_insert, N);
	vprint(to_insert, N);

	for( i = 0; i<N; i++ ){
		rbtree_add( &rbt, &(to_insert[i]) );
	}

	print_rbt ( rbt.root, 0 );

	/*
	 * search functions
	 */
	char* str;
	gerror_t g;
	
	/* query the minimal value */
	printf("min value: ");
	if(( g = rbtree_min_value(&rbt, &str ) == GERROR_OK))
		printf("%s\n", str);
	else
		printf("%s\n", gerror_to_str(g));

	/* query the maximal value */
	printf("max value: ");
	if(( g = rbtree_max_value(&rbt, &str) ) == GERROR_OK)
		printf("%s\n", str);
	else
		printf("%s\n", gerror_to_str(g));

	/*
	 * verify tree
	 */
	rbtree_check(&rbt);

	/*
	 * removing ...
	 */
	randomize(to_insert, N);

	printf("remove: ");
	vprint(to_insert, N);

	for( i = 0; i<N; i++ )
	{
		printf("remove %s...\n", to_insert[i]);
		rbtree_remove_item(&rbt, &(to_insert[i]));
		printf("removed! (size: %zu)\n", rbt.size);
		print_rbt(rbt.root, 0);
		rbtree_check(&rbt);
	}

	rbtree_destroy( &rbt );

	return 0;
}
