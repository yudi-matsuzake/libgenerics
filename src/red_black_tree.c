/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#include "red_black_tree.h"

typedef enum{
	RB_LEFT,
	RB_RIGHT
} rbc_t ;

rbnode_t* rbnode_destroy(rbnode_t* node);

/*
 * auxiliar function prototypes to insert
 */
static void rbtree_insert_fixup(rbtree_t* rbt, rbnode_t* node);
static void fix_insert_case(rbtree_t* rbt, rbnode_t** node, rbnode_t* uncle, int c, int l);
static rbnode_t* create_node(rbtree_t* rbt, void* elem);
static void left_rotate(rbtree_t* rbt, rbnode_t* node);
static void right_rotate(rbtree_t* rbt, rbnode_t* node);

/*
 * auxiliar function prototypes to delete
 */
void rbtree_transplant(rbtree_t* rbt, rbnode_t* u, rbnode_t* v);
void rbtree_delete_fixup(rbtree_t* rbt, rbnode_t* node);
int rbtree_identify_case(rbnode_t* node, rbc_t* side);
rbnode_t* rbtree_create_double_black();
rbnode_t* rbtree_find_minimal_node(rbnode_t* node);
void rbtree_remove_double_black(rbtree_t* rbt, rbnode_t* db);
int rbnode_is_red(rbnode_t* node);
int rbnode_is_black(rbnode_t* node);


/*
 * default compare function prototype
 */
int rbtree_default_compare_function(void* a, void* b, void* arg);

/** Populates the `rbt` structure and inicialize it.
  * A red n black tree needs a rbtree_compare_function. The default function
  * will only work for char, int and long. If you need a double or float
  * you need to implement the compare function and set with the
  * function `rbtree_set_compare_function`
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param member_size	size in bytes of the indexed elements
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL
  */
gerror_t rbtree_create(rbtree_t* rbt, size_t member_size)
{
	if(!rbt) return GERROR_NULL_STRUCTURE;

	rbt->member_size = member_size;
	rbt->size = 0;
	rbt->compare = rbtree_default_compare_function;
	rbt->compare_argument = &(rbt->member_size);
	rbt->root = NULL;

	rbt->flags = G_RB_LEFT_LEANING;

	return GERROR_OK;
}


/** Destroy (i.e. deallocates) the `rbt` structure fields.
  *
  * @param p	previous allocated pqueue_t struct
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL
  */
gerror_t rbtree_destroy (rbtree_t* rbt)
{
	if(!rbt) return GERROR_NULL_STRUCTURE;

	rbt->member_size = 0;
	rbt->compare = NULL;
	rbt->compare_argument = NULL;
	rbt->size = 0;
	rbt->root = rbnode_destroy(rbt->root);

	return GERROR_OK;
}


/** Change the default comparison function of the red n black tree `rbt`
  * for `function` with the argument `argument`.
  *
  * @param rbt		pointer to a previous allocated `rbtree_t` structure
  * @param function	comparison function callback that has
  * 			the following prototype:
  * 			int compare(void* a, void* b)
  * 			the a and b are the arguments
  * 			returns -1 if `a` is smaller than b
  * 			returns 0 if `a` is equal than `b`
  * 			return 1 if `a` is bigger than `b`
  * @param argument	pointer to the argument to the comparison function
  *
  * @see rbcomp_t
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `t` is a NULL
  */
gerror_t rbtree_set_compare_function(
		rbtree_t* rbt,
		rbtree_compare_function function,
		void* argument)
{
	if(!rbt) return GERROR_NULL_STRUCTURE;

	rbt->compare = function;
	rbt->compare_argument = argument;

	return GERROR_OK;
}

/** Add an element pointed by `elem` with size `rbt->member_size`
  * in the rbtree.
  *
  * @param rbt	previous allocated rbtree_t structure
  * @param elem	pointer to the elem to be copied to the structure
  *
  * @return	GERROR_OK in case of sucess operation;
  * 		GERROR_NULL_STRUCTURE in case `rbt` is null
  */
gerror_t rbtree_add(rbtree_t* rbt, void* elem)
{
	if(!rbt) return GERROR_NULL_STRUCTURE;

	/*
	 * find the node parent of `elem`
	 */
	rbnode_t* parent_node = NULL;
	rbnode_t* node = rbt->root;
	rbc_t c = RB_LEFT;

	while( node != NULL ){
		parent_node = node;

		rbcomp_t result = rbt->compare(elem, node->data, rbt->compare_argument);

		switch(result){
		case G_RB_FIRST_IS_GREATER:
			node = node->right;
			c = RB_RIGHT;
			break;
		case G_RB_FIRST_IS_SMALLER:
			node = node->left;
			c = RB_LEFT;
			break;
		case G_RB_EQUAL:
			if(rbt->flags & G_RB_EQUAL_OVERRIDE){
				if(elem)
					memcpy(node->data, elem, rbt->member_size);
				return GERROR_OK;
			}else if(rbt->flags & G_RB_LEFT_LEANING){
				node = node->left;
				c = RB_LEFT;
			}else{
				node = node->right;
				c = RB_RIGHT;
			}
			break;
		default:
			return GERROR_COMPARE_FUNCTION_WRONG_RETURN;
		}

	}

	/*
	 * insert the node
	 */
	node = create_node(rbt, elem);

	if(parent_node == NULL)
		rbt->root = node;
	else if(c == RB_LEFT)
		parent_node->left = node;
	else
		parent_node->right = node;

	node->parent = parent_node;

	/*
	 * fixup
	 */
	rbtree_insert_fixup(rbt, node);

	rbt->size++;

	return GERROR_OK;
}

/** Finds and remove the first element that match with `elem`.
  *
  * @param rbt		previous allocated rbtree_t structure
  * @param elem		pointer element to be removed
  *
  * @return	GERROR_OK in case of sucess operation;
  * 		GERROR_NULL_STRUCTURE in case `rbt` is null;
  * 		GERROR_NULL_ELEMENT_POINTER in case `elem` is
  * 		pointing to null;
  * 		GERROR_TRY_REMOVE_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  *
  */
gerror_t rbtree_remove_item (rbtree_t* rbt, void* elem)
{
	if(!rbt)	return GERROR_NULL_STRUCTURE;
	if(!elem)	return GERROR_NULL_ELEMENT_POINTER;
	if(!rbt->size)	return GERROR_TRY_REMOVE_EMPTY_STRUCTURE;

	rbnode_t* node = NULL;

	if(rbtree_find_node(rbt, elem, &node) == GERROR_OK)
		return rbtree_remove_node(rbt, node);

	return GERROR_REMOVE_ELEMENT_NOT_FOUNDED;
}

/** Removes the node `node` of the `rbt` tree.
  *
  * @param rbt	previous allocated rbtree_t structure
  * @param node	pointer to a `rbnode_t` structure.
  *
  * @return	GERROR_OK in case of sucess operation;
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		GERROR_NULL_ELEMENT_POINTER in case `elem` is
  * 		pointing to null;
  * 		GERROR_TRY_REMOVE_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  */
gerror_t rbtree_remove_node (rbtree_t* rbt, rbnode_t* node)
{
	if(!rbt)	return GERROR_NULL_STRUCTURE;
	if(!node)	return GERROR_NULL_ELEMENT_POINTER;
	if(!rbt->size)	return GERROR_TRY_REMOVE_EMPTY_STRUCTURE;

	rbnode_t*	to_delete	= node;
	rbnode_t*	to_fix		= NULL;
	rbcolor_t	original_color	= node->color;

	/*
	 * if the node has no children
	 */
	if(node->right == NULL && node->left == NULL){
		to_fix = rbtree_create_double_black();
		rbtree_transplant(rbt, node, to_fix);

	/*
	 * if the node does not have the right child
	 */
	}else if(node->left == NULL){
		to_fix = node->right;
		rbtree_transplant(rbt, node, node->right);

	/*
	 * if the node does not have the left child
	 */
	}else if(node->right == NULL){
		to_fix = node->left;
		rbtree_transplant(rbt, node, node->left);

	/*
	 * if the node has both children
	 */
	}else{

		/*
		 * find the minimal node in the right subtree
		 */
		to_delete = rbtree_find_minimal_node(node->right);
		original_color = to_delete->color;

		/*
		 * create double black node if
		 * the minimal node has no children
		 * and point `to_fix` to the right
		 * child of `to_delete`
		 *
		 * this is because `to_fix` needs
		 * to points where the node was removed
		 * for the fixup function
		 */
		if(to_delete->right == NULL){
			to_fix = rbtree_create_double_black();
			to_delete->right = to_fix;
			to_fix->parent = to_delete;
		}else{
			to_fix = to_delete->right;
		}

		/*
		 * linking
		 */
		if(to_delete->parent == node){
			to_fix->parent = to_delete;
		}else{
			rbtree_transplant(rbt, to_delete, to_delete->right);
			to_delete->right = node->right;
			to_delete->right->parent = to_delete;
		}

		rbtree_transplant(rbt, node, to_delete);
		to_delete->left = node->left;
		to_delete->left->parent = to_delete;
		to_delete->color = node->color;


	}

	if(original_color == G_RB_BLACK)
		rbtree_delete_fixup(rbt, to_fix);

	/*
	 * delete the double black node
	 */
	if(to_fix != NULL && to_fix->color == G_RB_DOUBLE_BLACK)
		rbtree_remove_double_black(rbt, to_fix);

	free(node->data);
	free(node);

	rbt->size--;

	return GERROR_OK;
}

/** Set the flags of the rbtree.
  * TODO: A more datailed description of rbtree_set_flags.
  *
  * @param rbt		previous allocated rbtree_t structure
  * @param flags	flags to set the 
  *
  * @see	rbflag_t
  *
  * @return	GERROR_OK in case of sucess operation;
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL
  */
gerror_t rbtree_set_flags (redblacktree_t* rbt, long flags)
{
	if(!rbt) return GERROR_NULL_STRUCTURE;

	rbt->flags = flags;

	return GERROR_OK;
}

/** Find a node with the value pointed by `elem` and write the pointer to
  * `node`.
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param elem		pointer element to find
  * @param node		pointer to the return node pointer;
  * 			this pointer could be null
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_ELEMENT_POINTER in case `elem` is
  * 		null;
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		By the end of the function, the `*node` will point
  * 		to the found element or NULL otherwise
  */
gerror_t rbtree_find_node (rbtree_t* rbt, void* elem, rbnode_t** node)
{
	if(!rbt)	return GERROR_NULL_STRUCTURE;
	if(!elem)	return GERROR_NULL_ELEMENT_POINTER;

	rbnode_t* n = rbt->root;
	*node = NULL;

	while(n != NULL){
		rbcomp_t result = rbt->compare(elem, n->data, rbt->compare_argument);

		switch(result){
		case G_RB_FIRST_IS_SMALLER:
			n = n->left;
			break;
		case G_RB_FIRST_IS_GREATER:
			n = n->right;
			break;
		case G_RB_EQUAL:
			*node = n;
			return GERROR_OK;
			break;
		default:
			return GERROR_COMPARE_FUNCTION_WRONG_RETURN;
		}
	}

	return GERROR_ELEMENT_NOT_FOUNDED;
}

/** Find the minimal value in rbt and write the pointer to
  * `node`.
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param node		pointer to the return node pointer;
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		GERROR_NULL_RETURN_POINTER in case that `node`
  * 		By the end of the function, the `*node` will point
  * 		to the found element or NULL otherwise
  */
gerror_t rbtree_min_node (rbtree_t* rbt, rbnode_t** node)
{
	if(!rbt)	return GERROR_NULL_STRUCTURE;
	if(!rbt->size)	return GERROR_EMPTY_STRUCTURE;
	if(!node)	return GERROR_NULL_RETURN_POINTER;

	rbnode_t* n = rbt->root;
	rbnode_t* temp_node = rbt->root;

	while(n != NULL){
		temp_node = n;
		n = n->left;
	}

	*node = temp_node;

	return GERROR_OK;
}

/** Find the maximal value in rbt and write the pointer to
  * `node`.
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param node		pointer to the return node pointer;
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		GERROR_NULL_RETURN_POINTER in case that `node`
  * 		By the end of the function, the `*node` will point
  * 		to the found element or NULL otherwise
  */
gerror_t rbtree_max_node (rbtree_t* rbt, rbnode_t** node)
{
	if(!rbt)	return GERROR_NULL_STRUCTURE;
	if(!rbt->size)	return GERROR_EMPTY_STRUCTURE;
	if(!node)	return GERROR_NULL_RETURN_POINTER;

	rbnode_t* n = rbt->root;
	rbnode_t* temp_node = rbt->root;

	while(n != NULL){
		temp_node = n;
		n = n->right;
	}

	*node = temp_node;

	return GERROR_OK;
}

/*
 * destroy a node and it's children's
 * recursively
 */
rbnode_t* rbnode_destroy(rbnode_t* node)
{
	if(node){
		rbnode_destroy(node->left);
		rbnode_destroy(node->right);
		free(node->data);
		free(node);
	}
	return NULL;
}

/** Find the minimal value in rbt and write the value to
  * `elem`.
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param elem		pointer to a local in memory to write
  * 			the minimal value
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		GERROR_NULL_RETURN_POINTER in case that `node`
  * 		By the end of the function, the `*node` will point
  * 		to the found element or NULL otherwise
  */
gerror_t rbtree_min_value (rbtree_t* rbt, void* elem)
{
	if(!elem) return GERROR_NULL_RETURN_POINTER;

	rbnode_t* node;
	gerror_t result = rbtree_min_node(rbt, &node);

	if( result != GERROR_OK )
		return result;

	memcpy(elem, node->data, rbt->member_size);

	return GERROR_OK;
}

/** Find the maximal value in rbt and write the value to
  * `elem`.
  *
  * @param rbt		previous allocated rbtree_t struct
  * @param elem		pointer to a local in memory to write
  * 			the maximal value
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_EMPTY_STRUCTURE in case the
  * 		`rbt` structure is empty
  * 		GERROR_NULL_STRUCURE in case `rbt` is a NULL;
  * 		GERROR_NULL_RETURN_POINTER in case that `node`
  * 		By the end of the function, the `*node` will point
  * 		to the found element or NULL otherwise
  */
gerror_t rbtree_max_value (rbtree_t* rbt, void* elem)
{
	if(!elem) return GERROR_NULL_RETURN_POINTER;

	rbnode_t* node;
	gerror_t result = rbtree_max_node(rbt, &node);

	if( result != GERROR_OK )
		return result;

	memcpy(elem, node->data, rbt->member_size);

	return GERROR_OK;
}

/*
 * Classic red n black insert fixup function
 */
static void rbtree_insert_fixup(rbtree_t* rbt, rbnode_t* node)
{
	while( node->parent != NULL && node->parent->color == G_RB_RED ){

		/*
		 * the parent of the node is the left child
		 */
		if( node->parent == node->parent->parent->left ){

			/*
			 * so the uncle is the right child
			 */
			rbnode_t* uncle = node->parent->parent->right;

			if( uncle != NULL && uncle->color == G_RB_RED )
				fix_insert_case(rbt, &node, uncle, 0, 1);
			else if(node == node->parent->right)
				fix_insert_case(rbt, &node, uncle, 1, 1);
			else if(node == node->parent->left)
				fix_insert_case(rbt, &node, uncle, 2, 1);
		}else{
			/*
			 * so the uncle is the left child
			 */
			rbnode_t* uncle = node->parent->parent->left;
			if( uncle != NULL && uncle->color == G_RB_RED )
				fix_insert_case(rbt, &node, uncle, 0, 0);
			else if(node == node->parent->left)
				fix_insert_case(rbt, &node, uncle, 1, 0);
			else if(node == node->parent->right)
				fix_insert_case(rbt, &node, uncle, 2, 0);
		}
	}

	rbt->root->color = G_RB_BLACK;
}

/*
 * receives a rbtree, a node and a case `c` and fix
 * the case `c`. `l` is whether the father is the left child
 */
static void fix_insert_case(rbtree_t* rbt, rbnode_t** node, rbnode_t* uncle, int c, int l)
{
	switch(c){
	/*
	 * case 0
	 * recoloring because uncle and parent are
	 * red
	 */
	case 0:
		(*node)->parent->color = G_RB_BLACK;
		if(uncle) uncle->color = G_RB_BLACK;
		(*node)->parent->parent->color = G_RB_RED;
		*node = (*node)->parent->parent;
		break;
	/*
	 * case 1
	 * left rotate
	 * because the uncle is black, the parent
	 * is red and the node is the right child
	 * note: the case 1 will end up in case 2,
	 * so the break statement is no used in
	 * this case
	 */
	case 1:
		*node = (*node)->parent;
		if(l)
			left_rotate(rbt, *node);
		else
			right_rotate(rbt, *node);
	/*
	 * case 2
	 * recoloring and right rotate 
	 * because the uncle is black, the parent
	 * is red and the node is the left child
	 */
	case 2:
		(*node)->parent->color = G_RB_BLACK;
		(*node)->parent->parent->color = G_RB_RED;
		if(l)
			right_rotate(rbt, (*node)->parent->parent);
		else
			left_rotate(rbt, (*node)->parent->parent);
		break;
	}
}

/*
 * allocates and create a node
 */
static rbnode_t* create_node(rbtree_t* rbt, void* elem)
{
	rbnode_t* node = (rbnode_t*) malloc( sizeof(rbnode_t) );

	node->left	= NULL;
	node->right	= NULL;
	node->parent	= NULL;
	node->color	= G_RB_RED;
	node->data	= NULL;

	if( rbt->member_size && elem ){
		node->data = (void*) malloc(rbt->member_size);
		memcpy(node->data, elem, rbt->member_size);
	}


	return node;
}

/*
 * performs the left rotation. This is the classical binary
 * search tree left rotation function.
 */
static void left_rotate(rbtree_t* rbt, rbnode_t* node)
{

	rbnode_t* temp_node = node->right;
	node->right = temp_node->left;

	if(temp_node->left != NULL)
		temp_node->left->parent = node;

	temp_node->parent = node->parent;

	/*
	 * checks if the `node` was the root of rbtree and
	 * update the new root to `temp_node` or link to
	 * the correct children side;
	 */
	if(node->parent == NULL)
		rbt->root = temp_node;
	else if(node->parent->left == node)
		node->parent->left = temp_node;
	else
		node->parent->right = temp_node;

	temp_node->left = node;
	node->parent = temp_node;
}

/*
 * performs the right rotation. This is the classical binary
 * search tree right rotation function.
 */
static void right_rotate(rbtree_t* rbt, rbnode_t* node)
{

	rbnode_t* temp_node = node->left;
	node->left = temp_node->right;

	if(temp_node->right != NULL)
		temp_node->right->parent = node;

	temp_node->parent = node->parent;

	/*
	 * checks if the `node` was the root of rbtree and
	 * update the new root to `temp_node` or link to
	 * the correct children side;
	 */
	if(node->parent == NULL)
		rbt->root = temp_node;
	else if(node->parent->left == node)
		node->parent->left = temp_node;
	else
		node->parent->right = temp_node;

	temp_node->right = node;
	node->parent = temp_node;
}

/*
 * the default comparison function. Just compare
 * like to long
 */
int rbtree_default_compare_function(void* a, void* b, void* arg)
{
	size_t size = *(size_t*)arg;

	long la = 0;
	long lb = 0;

	switch(size){
	case sizeof(int):
		la = *(int*)a;
		lb = *(int*)b;
		break;
	case sizeof(char):
		la = *(char*)a;
		lb = *(char*)b;
		break;
	case sizeof(long):
		la = *(long*)a;
		lb = *(long*)b;
		break;
	}
	return (la > lb) - (la < lb);
}

/*
 * transplant function, transplants node u by node v in the tree
 */
void rbtree_transplant(rbtree_t* rbt, rbnode_t* u, rbnode_t* v)
{
	if(u->parent == NULL)
		rbt->root = v;
	else if(u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;

	if(v) v->parent = u->parent;
}


/*
 * returns 1 if the node is red and 0 otherwise
 */
int rbnode_is_red(rbnode_t* node)
{
	return !rbnode_is_black(node);
}

/*
 * returns 1 if the node is black and 0 otherwise
 */
int rbnode_is_black(rbnode_t* node)
{
	return node == NULL || node->color != G_RB_RED;
}

/*
 * Classic red n black delete fixup function
 */
void rbtree_delete_fixup(rbtree_t* rbt, rbnode_t* node)
{

	while(node != rbt->root && rbnode_is_black(node)){

		rbc_t side;

		int c;
		switch( c = rbtree_identify_case(node, &side) ){
		case 2:
			/*
			 * parent becomes red
			 * sibling becomes black
			 * left rotate the parent
			 * "no" push the problem up
			 * "no" between quotes because
			 * the problem is pushed up only for
			 * a non-double-black node
			 */
			if(side == RB_LEFT){
				node->parent->color = G_RB_RED;
				node->parent->right->color = G_RB_BLACK;
				left_rotate(rbt, node->parent);

				if(node->parent->right == NULL)
					node = node->parent;

			}else{
				node->parent->color = G_RB_RED;
				node->parent->left->color = G_RB_BLACK;
				right_rotate(rbt, node->parent);

				if(node->parent->left == NULL)
					node = node->parent;
			}

			break;

		case 3:

			/*
			 * sibling becomes red
			 * push the problem upwards
			 */
			if(side == RB_LEFT)
				node->parent->right->color = G_RB_RED;
			else
				node->parent->left->color = G_RB_RED;

			node = node->parent;
			break;
		case 4:
			/*
			 * parent becomes black
			 * sibling becomes red
			 */
			node->parent->color = G_RB_BLACK;

			if(side == RB_LEFT){
				if(node->parent->right != NULL)
					node->parent->right->color = G_RB_RED;
			}
			else{
				if(node->parent->left != NULL)
					node->parent->left->color = G_RB_RED;
			}

			/*this is a terminal case*/
			node = rbt->root;
			break;
		case 5:
			/*
			 * sibling becomes black
			 * siblings's left child becomes red
			 * push the problem upwards
			 * right-rotation on the sibling
			 */
			if(side == RB_LEFT){
				node->parent->right->color = G_RB_RED;
				node->parent->right->left->color = G_RB_BLACK;
				right_rotate(rbt, node->parent->right);
			}else{
				node->parent->left->color = G_RB_RED;
				node->parent->left->right->color = G_RB_BLACK;
				left_rotate(rbt, node->parent->left);
			}

			break;
		case 6:
			/*
			 * sibling take the color of the parent
			 * parent becomes black
			 * right child of the sibling becomes black
			 * left-rotate in the parent
			 */
			if(side == RB_LEFT){
				node->parent->right->color = node->parent->color;
				node->parent->color = G_RB_BLACK;
				node->parent->right->right->color = G_RB_BLACK;
				left_rotate(rbt, node->parent);
			}else{
				node->parent->left->color = node->parent->color;
				node->parent->color = G_RB_BLACK;
				node->parent->left->left->color = G_RB_BLACK;
				right_rotate(rbt, node->parent);
			}

			/*this is a terminal case*/
			node = rbt->root;
			break;
		case 0:
			node = rbt->root;
			break;
		}

	}

	if(node->color != G_RB_DOUBLE_BLACK)
		node->color = G_RB_BLACK;

}


/*
 * allocates and populate a double black node based
 * by `node` and return the pointer
 */
rbnode_t* rbtree_create_double_black()
{
	rbnode_t* db = (rbnode_t*) malloc(sizeof(rbnode_t));

	db->parent	= NULL;
	db->right	= NULL;
	db->left	= NULL;
	db->data	= NULL;
	db->color	= G_RB_DOUBLE_BLACK;

	return db;
}

/*
 * remove and deallocates the double_black node
 */
void rbtree_remove_double_black(rbtree_t* rbt, rbnode_t* db)
{
	if(db == rbt->root){
		rbt->root = NULL;
	}else{
		if(db->parent->left == db){
			db->parent->left = NULL;
		}else{
			db->parent->right = NULL;
		}
	}

	free(db);
}

/*
 * give a node, find a minimal of the subtree
 */
rbnode_t* rbtree_find_minimal_node(rbnode_t* node)
{
	rbnode_t* minimal = NULL;
	while(node != NULL){
		minimal = node;
		node = node->left;
	}

	return minimal;
}

/*
 * identify the case to fix
 */
int rbtree_identify_case (rbnode_t* node, rbc_t* side)
{
	if(node == node->parent->left){
		*side = RB_LEFT;
		rbnode_t* sibling = node->parent->right;

		if(rbnode_is_black(node->parent)){

			if(rbnode_is_red(sibling)){
				return 2;
			}else{
				if(rbnode_is_black(sibling->right)){
					if(rbnode_is_black(sibling->left))
						return 3;
					else
						return 5;
				}

				if(rbnode_is_red(sibling->right))
					return 6;
			}

		}else{
			if(rbnode_is_black(sibling)){
				if( sibling == NULL ||
				   (rbnode_is_black(sibling->right) &&
				    rbnode_is_black(sibling->left))){
					return 4;
				}else if(rbnode_is_red(sibling->left) &&
					 rbnode_is_black(sibling->right)){
					return 5;
				}else if(rbnode_is_red(sibling->right)){
					return 6;
				}
			}
		}

	}else{
		*side = RB_RIGHT;

		rbnode_t* sibling = node->parent->left;

		if(rbnode_is_black(node->parent)){

			if(rbnode_is_red(sibling)){
				return 2;
			}else{
				if(rbnode_is_black(sibling->left)){
					if(rbnode_is_black(sibling->right))
						return 3;
					else
						return 5;
				}

				if(rbnode_is_red(sibling->left))
					return 6;
			}

		}else{
			if(rbnode_is_black(sibling)){
				if( sibling == NULL ||
				   (rbnode_is_black(sibling->right) &&
				    rbnode_is_black(sibling->left))){
					return 4;
				}else if(rbnode_is_red(sibling->right) &&
					 rbnode_is_black(sibling->left)){
					return 5;
				}else if(rbnode_is_red(sibling->left)){
					return 6;
				}
			}
		}

	}

	return 0;
}
