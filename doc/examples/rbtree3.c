#include <stdio.h>
#include <generics/red_black_tree.h>

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

rbnode_t* create_node(int v, rbcolor_t c, rbnode_t* l, rbnode_t* r)
{
	rbnode_t* node = (rbnode_t*) malloc(sizeof(rbnode_t));

	node->color = c;
	node->parent = NULL;
	node->data = (void*) malloc(sizeof(int));
	memcpy(node->data, &v, sizeof(int));

	if(l){
		l->parent = node;
		node->left = l;
	}

	if(r){
		r->parent = node;
		node->right = r;
	}

	return node;
}

int main()
{

	rbtree_t rbt;
	rbtree_create(&rbt, sizeof(int));

	rbt.root = create_node(4, G_RB_BLACK,
			create_node(1, G_RB_RED,
				create_node(0, G_RB_BLACK,
					NULL,
					NULL),
				create_node(2, G_RB_BLACK,
					NULL,
					create_node(3, G_RB_RED,
						NULL,
						NULL))),
			create_node(7, G_RB_RED,
				create_node(6, G_RB_BLACK,
					NULL,
					NULL),
				create_node(9, G_RB_BLACK,
					create_node(8, G_RB_RED,
						NULL,
						NULL),
					NULL)));

	rbt.size = 9;

	print_rbt(rbt.root, 0);

	int n = 6;

	gerror_t g = rbtree_remove_item(&rbt, &n);

	printf("%s\n", gerror_to_str(g));

	print_rbt(rbt.root, 0);

	rbtree_destroy(&rbt);
	return 0;
}
