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

#ifndef __MAP_H__
#define __MAP_H__

#include "red_black_tree.h"
#include "gerror.h"

typedef struct map_t {
	size_t key_size;
	size_t member_size;

	redblacktree_t map;
} map_t;

gerror_t map_create(struct map_t* m, size_t key_size, size_t member_size);
gerror_t map_destroy(struct map_t* m);
gerror_t map_insert(struct map_t* m, void* key, void* elem);
gerror_t map_remove(struct map_t* m, void* key, void* elem);
gerror_t map_at(struct map_t* m, void* key, void* elem);

gerror_t map_set_compare_function(
		struct map_t* m,
		rbtree_compare_function compare_function,
		void* arg);

#endif
