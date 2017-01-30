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

typedef enum {
	G_RB_FIRST_IS_SMALLER = -1,
	G_RB_EQUAL,
	G_RB_FIRST_IS_GREATER
} rbcomp_t;

typedef int (*rbtree_compare_function)(void* a, void *b, void* arg);

typedef enum {
	G_RB_RED,
	G_RB_BLACK
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
gerror_t rbtree_remove(rbtree_t* rbt, void* elem);

#endif
