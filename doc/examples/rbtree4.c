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

int main()
{

	/*
	 * default flags
	 */
	rbtree_t rbt;
	rbtree_create(&rbt, sizeof(int));

	int n = 1;

	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);

	print_rbt(rbt.root, 0);
	rbtree_destroy( &rbt );

	/*
	 * right leaning flag
	 */
	rbtree_create(&rbt, sizeof(int));
	rbtree_set_flags(&rbt, rbt.flags & ~G_RB_LEFT_LEANING);

	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);

	print_rbt(rbt.root, 0);
	rbtree_destroy( &rbt );

	/*
	 * override flag
	 */
	rbtree_create(&rbt, sizeof(int));
	rbtree_set_flags(&rbt, G_RB_EQUAL_OVERRIDE);

	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);
	rbtree_add(&rbt, &n);

	print_rbt(rbt.root, 0);
	rbtree_destroy( &rbt );

	return 0;
}
