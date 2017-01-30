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

int is_a_valid_rbtree(rbnode_t* node, int* count)
{
	if( node == NULL ){
		int left_count;
		int right_count;
		if(!is_a_valid_rbtree(node->left, &left_count)){
			return 0;
		}
		if(!is_a_valid_rbtree(node->right, &right_count)){
			return 0;
		}

		*count += left_count + right_count + (node->color == G_RB_BLACK);
		return left_count == right_count;
	}

	return 1;
}

void swap(int* a, int* b)
{
	int cup = *a;
	*a = *b;
	*b = cup;
}

int main()
{

	srand(time(NULL));

	rbtree_t rbt;

	rbtree_create(&rbt, sizeof(int));
	int i;

	int to_insert[N];
	for( i = 0; i<N; i++ )
		to_insert[i] = i;
	for( i = 0; i<N; i++ )
		swap(&(to_insert[i]), &(to_insert[rand()%N]));
	for( i = 0; i<N; i++ )
		printf("%d ", to_insert[i]);
	printf("\n");

	for( i = 0; i<N; i++ ){
		rbtree_add( &rbt, &(to_insert[i]) );
	}
	print_rbt ( rbt.root, 0 );

	int n = 0;
	printf("It %s a valid red-black tree\n", (is_a_valid_rbtree((rbt.root), &n))?"is":"is not");

	rbtree_destroy( &rbt );

	return 0;
}
