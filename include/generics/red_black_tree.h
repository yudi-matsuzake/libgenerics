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
#ifndef __RED_BLACK_TREE_H__
#define __RED_BLACK_TREE_H__

#include <stdlib.h>
#include <string.h>
#include "gerror.h"

/**
  * return of the redblacktree compare function
  * of type rbtree_compare_function
  */
typedef enum {
	G_RB_FIRST_IS_SMALLER = -1,
	G_RB_EQUAL,
	G_RB_FIRST_IS_GREATER
} rbcomp_t;

/** Flags for rbtree.
  */
typedef enum {
	/** rbtree left leaning, if this flag is not on,
	  * the rbtree will be right leaning
	  */
	G_RB_LEFT_LEANING	= 1,

	/** if this flag is set, when a node is equal,
	  * the value will be override and the tree will not leaning
	  * if this flag is set
	  */
	G_RB_EQUAL_OVERRIDE	= 1<<1
} rbflag_t;

typedef int (*rbtree_compare_function)(void* a, void *b, void* arg);

typedef enum {
	G_RB_RED,
	G_RB_BLACK,
	G_RB_DOUBLE_BLACK,
} rbcolor_t;

typedef struct redblacknode_t {
	struct redblacknode_t* left;
	struct redblacknode_t* right;
	struct redblacknode_t* parent;

	rbcolor_t color;
	void* data;
} redblacknode_t;

typedef struct redblacktree_t {
	size_t size;
	size_t member_size;

	rbtree_compare_function compare;
	void* compare_argument;
	long flags;
	struct redblacknode_t* root;
} redblacktree_t;

typedef struct redblacktree_t rbtree_t;
typedef struct redblacknode_t rbnode_t;

gerror_t rbtree_create(rbtree_t* rbt, size_t member_size);
gerror_t rbtree_destroy(rbtree_t* rbt);

gerror_t rbtree_set_compare_function(	rbtree_t* rbt,
					rbtree_compare_function function,
					void* argument);
gerror_t rbtree_add(rbtree_t* rbt, void* elem);
gerror_t rbtree_remove_item(rbtree_t* rbt, void* elem);
gerror_t rbtree_remove_node(rbtree_t* rbt, rbnode_t* node);

gerror_t rbtree_set_flags(rbtree_t* rbt, long flags);

gerror_t rbtree_min_node(rbtree_t* rbt, rbnode_t** node);
gerror_t rbtree_min_value(rbtree_t* rbt, void* elem);
gerror_t rbtree_max_node(rbtree_t* rbt, rbnode_t** node);
gerror_t rbtree_max_value(rbtree_t* rbt, void* elem);
gerror_t rbtree_find_node(rbtree_t* rbt, void* elem, rbnode_t** node);

#endif
