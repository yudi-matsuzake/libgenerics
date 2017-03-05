#include <stdio.h>
#include <time.h>
#include <generics/red_black_tree.h>

#define N 10

void print_rbt (rbnode_t* node, int t)
{
	if(node != NULL){
		int i;
		for( i = 0; i<t; i++ ){
			printf("|\t");
		}
		printf("%d (%s)\n", *((int*)node->data), (node->color == G_RB_BLACK)?"black":"red");
		
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

void find_int(rbtree_t* rbt, int elem)
{
	rbnode_t* node;
	gerror_t g = rbtree_find_node(rbt, &elem, &node);

	if(g == GERROR_OK){
		printf("%d found in node addr [%p]\n", elem, node);
		printf("color: %s\n", (node->color == G_RB_RED)?
							"red":"black");
		if(node->parent) printf("father: %d\n", *(int*)node->parent->data);
		else printf("father: null\n");
	}else{
		printf("%s\n", gerror_to_str(g));
	}
}

void swap(int* a, int* b)
{
	int cup = *a;
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

void randomize(int* v, int n)
{
	int i;
	for( i = 0; i<n; i++ ){
		swap(&v[i], &v[rand()%n]);
	}
}

void vprint(int* v, int n)
{
	int i;
	for( i = 0; i<n; i++ ){
		printf("%d ", v[i]);
	}
	printf("\n");
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

int main()
{

	srand(time(NULL));

	rbtree_t rbt;
	rbtree_create(&rbt, sizeof(int));
	int i;

	/*
	 * insert
	 */
	int to_insert[N];

	inicialize(to_insert, N);
	randomize(to_insert, N);
	vprint(to_insert, N);

	for( i = 0; i<N; i++ ){
		rbtree_add( &rbt, &(to_insert[i]) );
	}

	print_rbt ( rbt.root, 0 );

	/*
	 * search functions
	 */
	int n;
	gerror_t g;
	
	/* search a element that is in the tree */
	n = rand()%N;
	printf("finding %d\n", n);
	find_int(&rbt, n);
	
	/* search a element that is not in the tree */
	n = rand()%N + N;
	printf("finding %d\n", n);
	find_int(&rbt, n);

	/* query the minimal value */
	printf("min value: ");
	if(( g = rbtree_min_value(&rbt, &n) ) == GERROR_OK)
		printf("%d\n", n);
	else
		printf("%s\n", gerror_to_str(g));

	/* query the maximal value */
	printf("max value: ");
	if(( g = rbtree_max_value(&rbt, &n) ) == GERROR_OK)
		printf("%d\n", n);
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
		printf("remove %d...\n", to_insert[i]);
		rbtree_remove_item(&rbt, &to_insert[i]);
		printf("removed! (size: %zu)\n", rbt.size);
		print_rbt(rbt.root, 0);
		rbtree_check(&rbt);
	}

	rbtree_destroy( &rbt );

	return 0;
}
