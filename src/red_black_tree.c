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
void rbtree_insert_fixup(rbtree_t* rbt, rbnode_t* node);
void fix_case(rbtree_t* rbt, rbnode_t** node, rbnode_t* uncle, int c, int l);
rbnode_t* create_node(rbtree_t* rbt, void* elem);
void left_rotate(rbtree_t* rbt, rbnode_t* node);
void right_rotate(rbtree_t* rbt, rbnode_t* node);

/*
 * auxiliar function prototypes to delete
 */
void rbtree_transplant(rbtree_t* rbt, rbnode_t* u, rbnode_t* v);

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

		if( result == G_RB_FIRST_IS_GREATER ){
			node = node->right;
			c = RB_RIGHT;
		}else{
			node = node->left;
			c = RB_LEFT;
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
		case G_RB_EQUAL:
			*node = n;
			return GERROR_OK;
			break;
		case G_RB_FIRST_IS_GREATER:
			n = n->right;
			break;
		case G_RB_FIRST_IS_SMALLER:
			n = n->left;
			break;
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
void rbtree_insert_fixup(rbtree_t* rbt, rbnode_t* node)
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
				fix_case(rbt, &node, uncle, 0, 1);
			else if(node == node->parent->right)
				fix_case(rbt, &node, uncle, 1, 1);
			else if(node == node->parent->left)
				fix_case(rbt, &node, uncle, 2, 1);
		}else{
			/*
			 * so the uncle is the left child
			 */
			rbnode_t* uncle = node->parent->parent->left;
			if( uncle != NULL && uncle->color == G_RB_RED )
				fix_case(rbt, &node, uncle, 0, 0);
			else if(node == node->parent->left)
				fix_case(rbt, &node, uncle, 1, 0);
			else if(node == node->parent->right)
				fix_case(rbt, &node, uncle, 2, 0);
		}
	}

	rbt->root->color = G_RB_BLACK;
}

/*
 * receives a rbtree, a node and a case `c` and fix
 * the case `c`. `l` is whether the father is the left child
 */
void fix_case(rbtree_t* rbt, rbnode_t** node, rbnode_t* uncle, int c, int l)
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
rbnode_t* create_node(rbtree_t* rbt, void* elem)
{
	rbnode_t* node = (rbnode_t*) malloc( sizeof(rbnode_t) );

	node->left	= NULL;
	node->right	= NULL;
	node->parent	= NULL;
	node->color	= G_RB_RED;
	node->data	= NULL;

	if( rbt->member_size && elem ){
		node->data = (void*) malloc( sizeof(rbt->member_size) );
		memcpy(node->data, elem, rbt->member_size);
	}


	return node;
}

/*
 * performs the left rotation. This is the classical binary
 * search tree left rotation function.
 */
void left_rotate(rbtree_t* rbt, rbnode_t* node)
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
void right_rotate(rbtree_t* rbt, rbnode_t* node)
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

	v->parent = u->parent;
}
