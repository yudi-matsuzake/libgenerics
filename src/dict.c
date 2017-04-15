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

#include "dict.h"

#define UNUSED(X) ((void)X)

int compare_string_function(void* a, void* b, void* arg);
void dict_free_node(redblacknode_t* node);

/** Creates a dict and populates the structure `d`.
  * TODO: A more datailed description of dict_create.
  *
  * @param d		previous allocated `dict_t` structure
  * @param member_size	size of the member
  *
  * @see map_create
  * @see map_set_compare_function
  *
  * @return	GERROR_OK in case of success;
  * 		GERROR_NULL_STRUCTURE in case the structure `d`
  * 		is pointed to null
  */
gerror_t dict_create (struct dict_t* d, size_t member_size)
{
	if(!d) return GERROR_NULL_STRUCTURE;

	d->member_size = member_size;
	map_create(&d->map, sizeof(char*), d->member_size);
	map_set_compare_function(
			&d->map,
			compare_string_function,
			NULL);

	return GERROR_OK;
}

/** Deallocates and cleanup the `dict_t` structure.
  * TODO: A more datailed description of dict_destroy.
  *
  * @param d TODO
  *
  * @see map_destroy
  *
  * @return	GERROR_OK in case of success;
  * 		GERROR_NULL_STRUCTURE in case the structure `d`
  * 		is pointed to null
  */
gerror_t dict_destroy (struct dict_t* d)
{
	if(!d) return GERROR_NULL_STRUCTURE;

	struct redblacknode_t* node = d->map.map.root;

	dict_free_node(node);

	return map_destroy(&d->map);
}

/** Inserts the element `elem` and mapped it with `key` in `d`.
  * TODO: A more datailed description of dict_insert.
  *
  * @param d	previous allocated `dict_t` structure
  * @param key	the string with '\0' terminator to map `elem`
  * @param elem	the element to be mapped
  *
  * @see map_insert
  *
  * @return	GERROR_OK in case of success;
  * 		GERROR_NULL_STRUCTURE in case the structure `d`
  * 		is pointed to null
  */
gerror_t dict_insert (struct dict_t* d, const char* key, void* elem)
{
	if(!d) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	size_t keylen = strlen(key);
	size_t alloc_key_siz = keylen+1;
	char* alloc_key = (char*) malloc(sizeof(char)*alloc_key_siz);

	strncpy(alloc_key, key, alloc_key_siz);
	return map_insert(&d->map, &alloc_key, elem);
}

/** Removes the elem mapped by `key` and write in the memory location
  * pointed by `elem` if is different of null.
  * TODO: A more datailed description of dict_remove.
  *
  * @param d	previous allocated `dict_t` structure
  * @param key	pointer to the key
  * @param elem	pointer to a local in memory to write
  * 		the removed element
  *
  * @return	GERROR_OK in case of success;
  * 		GERROR_NULL_STRUCTURE in case the structure `d`
  * 		GERROR_NULL_KEY in case that `key` is null
  */
gerror_t dict_remove (struct dict_t* d, const char* key, void* elem)
{
	if(!d) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	char* rkey;
	gerror_t g = map_get_pair(&d->map, &key, &rkey, elem);

	if(g != GERROR_OK)
		return g;

	free(rkey);

	return GERROR_OK;
}

/** Find the element pointed by `key` and writes in the memory location
  * pointed by `elem`.
  * TODO: A more datailed description of dict_at.
  *
  * @param d	previous allocated `dict_t` structure
  * @param key	pointer to the key
  * @param elem	pointer to a local in memory to write the returning element
  *
  * @see map_at
  *
  * @return	GERROR_OK in case of success;
  * 		GERROR_NULL_STRUCTURE in case the structure `d`
  * 		GERROR_NULL_KEY in case that `key` is null
  */
gerror_t dict_at (struct dict_t* d, const char* key, void* elem)
{
	if(!d) return GERROR_NULL_STRUCTURE;
	if(!key) return GERROR_NULL_KEY;

	return map_at(&d->map, &key, elem);
}


/*
 * recursively frees the node data `node`
 */
void dict_free_node(redblacknode_t* node)
{
	if(node != NULL){
		dict_free_node(node->left);
		dict_free_node(node->right);
		char* str;

		memcpy(&str, node->data, sizeof(char*));

		if(str)
			free(str);
	}
}

/*
 * compare function dict
 */
int compare_string_function(void* a, void* b, void* arg)
{
	UNUSED(arg);

	char* a_str = *(char**)a;
	char* b_str = *(char**)b;

	int r = strcmp(a_str, b_str);
	rbcomp_t result;

	if(r < 0)
		result = G_RB_FIRST_IS_SMALLER;
	else if(r == 0)
		result = G_RB_EQUAL;
	else
		result = G_RB_FIRST_IS_GREATER;

	return result;
}
