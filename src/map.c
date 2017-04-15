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

#include "map.h"

/** Default compare function for map.
  * This functions only looks to the key.
  *
  * @see rbtree_compare_function
  */
int map_default_compare_function (void* a, void* b, void* arg);

/** Creates a map of key size `key_size` and mapped member with size `member_size`
  * and populates the previous allocated structure `m`.
  * TODO: A more datailed description of map_create.
  *
  * @param m		pointer to a previous allocated `map_t` structure;
  * @param key_size	size of the key in bytes
  * @param member_size	size of the members in bytes
  *
  * @see rbtree_create
  *
  * @return	GERROR_OK in case of success
  */
gerror_t map_create (struct map_t* m, size_t key_size, size_t member_size)
{
	if(!m) return GERROR_NULL_STRUCTURE;

	m->key_size = key_size;
	m->member_size = member_size;

	gerror_t g = rbtree_create(&m->map, key_size + member_size);
	if(g != GERROR_OK)
		return g;

	g = rbtree_set_flags(&m->map, G_RB_EQUAL_OVERRIDE);

	return rbtree_set_compare_function(&m->map, map_default_compare_function, m);
}

/** Destroys and deallocates inner attributes of `m`.
  * TODO: A more datailed description of map_destroy.
  *
  * @param m	pointer to a previous allocated `map_t` structure;
  *
  * @see rbtree_destroy
  *
  * @return	GERROR_OK in case of success
  */
gerror_t map_destroy (struct map_t* m)
{
	if(!m) return GERROR_NULL_STRUCTURE;

	m->key_size = 0;
	m->member_size = 0;

	return rbtree_destroy(&m->map);
}

/** Inserts a element `elem` in the key `key` in the map `m`.
  * TODO: A more datailed description of map_insert.
  *
  * @param m	pointer to a previous allocated `map_t` structure;
  * @param key	pointer to the key used to map the `elem`
  * @param elem	pointer to the element to be inserted
  *
  * @see rbtree_add
  *
  * @return	GERROR_OK in case of success
  */
gerror_t map_insert (struct map_t* m, void* key, void* elem)
{
	if(!m) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;
	if(!elem) return GERROR_NULL_ELEMENT_POINTER;

	void* pair = (void*) malloc(m->key_size + m->member_size);
	memcpy(pair, key, m->key_size);
	memcpy(pair + m->key_size, elem, m->member_size);

	gerror_t g = rbtree_add(&m->map, pair);

	free(pair);
	return g;
}

/** Removes the element mapped by `key`.
  * TODO: A more datailed description of map_remove.
  *
  * @param m	pointer to a previous allocated `map_t` structure;
  * @param key	pointer to the key
  * @param elem	pointer to a location of memory where the `elem`
  *		will be returned
  *
  * @see rbtree_find_node
  * @see rbree_remove_node
  *
  * @return	GERROR_OK in case of success
  */
gerror_t map_remove (struct map_t* m, void* key, void* elem)
{
	if(!m) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	rbnode_t* node;
	gerror_t g = rbtree_find_node(&m->map, key, &node);
	if(g != GERROR_OK)
		return g;

	if(elem)
		memcpy(elem, node->data + m->key_size, m->member_size);

	return rbtree_remove_node(&m->map, node);
}

/** Writes the elem mapped by `key` in the memory pointed by `elem`.
  * TODO: A more datailed description of map_at.
  *
  * @param m	pointer to a previous allocated `map_t` structure;
  * @param key	pointer to the key
  * @param elem	pointer to a local in memory to write
  * 		the element at `key`
  *
  * @see
  *
  * @return	GERROR_OK in case of success
  * 		GERROR_NULL_KEY in case that `key` is null
  */
gerror_t map_at (struct map_t* m, void* key, void* elem)
{
	if(!m) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	rbnode_t* node;

	gerror_t g = rbtree_find_node(&m->map, key, &node);
	if(g != GERROR_OK)
		return g;

	if(elem && node && node->data)
		memcpy(elem, node->data + m->key_size, m->member_size);

	return GERROR_OK;
}

/** Set the default compare function of `m` keys.
  * TODO: A more datailed description of map_set_compare_function.
  *
  * @param m			pointer to a previous allocated `map_t` structure;
  * @param compare_function	compare function to use in rbtree
  * @param arg			argument to use in comparison function
  *
  * @see rbtree_set_compare_function
  *
  * @return	GERROR_OK in case of success
  */
gerror_t map_set_compare_function (
		struct map_t* m,
		rbtree_compare_function compare_function,
		void* arg)
{
	if(!m) return GERROR_NULL_STRUCTURE;

	return rbtree_set_compare_function(&m->map, compare_function, arg);
}

/** Get the pair key:value and write in memory pointed by
  * `rkey` and `relem`.
  * TODO: A more datailed description of map_get_pair.
  *
  * @param m		pointer to a previous allocated `map_t` structure;
  * @param key		pointer to the key
  * @param rkey		pointer to the memory location to write the key
  * @param relem	pointer to the memory location to write the elem
  *
  * @return	GERROR_OK in case of success
  * 		GERROR_NULL_KEY in case that `key` is null
  */
gerror_t map_get_pair (
		struct map_t* m,
		void* key,
		void* rkey,
		void* relem)
{
	if(!m) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	rbnode_t* node;

	gerror_t g = rbtree_find_node(&m->map, key, &node);
	if(g != GERROR_OK)
		return g;

	if(rkey && node && node->data)
		memcpy(rkey, node->data, m->key_size);

	if(relem && node && node->data)
		memcpy(relem, node->data + m->key_size, m->member_size);

	return GERROR_OK;
}

/*
 * default compare function of map
 * only work for ints, only look the keys
 */
int map_default_compare_function (void* a, void* b, void* arg)
{
	struct map_t* m = (struct map_t*) arg;
	size_t size = m->key_size;

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
