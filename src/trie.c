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
#include "trie.h"

/*
 * Auxiliar function;
 * find the node mapped by `string`, if necessary, allocates
 *
 * @return the node mapped with string and allocate
 */
tnode_t* node_at_and_allocate ( struct trie_t* t, void* string, size_t size)
{
	struct tnode_t* node = &t->root;
	size_t i;
	char *ptr = string;

	for(i=0; i<size; i++){
		int byte = (int)ptr[i];
		if ( node->children[byte] == NULL ){
			node->children[byte] = (tnode_t*)malloc(sizeof(tnode_t));
			node->children[byte]->value = NULL;

			int j;
			for(j=0; j<NBYTE; j++)
				node->children[byte]->children[i] = NULL;

		}

		node = node->children[byte];
	}
	return node;
}

/*
 * Auxiliar function;
 * find the node mapped by `string`
 *
 * @return	the node mapped with `string` or null case the mapped
 * 		does not exists.
 */
tnode_t* node_at ( struct trie_t* t, void* string, size_t size)
{
	struct tnode_t* node = &t->root;
	size_t i;
	char *ptr = string;

	for(i=0; i<size; i++){
		int byte = (int)ptr[i];
		if ( node->children[byte] == NULL )
			return NULL;

		node = node->children[byte];
	}
	return node;
}

/** Inicialize structure `t` with `member_size` size.
  * The t has to be allocated.
  *
  * @param t		pointer to the allocated struct trie_t;
  * @param member_size	size in bytes of the indexed elements
  * 			by the trie.
  */
void trie_create (struct trie_t* t, size_t member_size)
{
	if(!t) return;

	t->size = 0;
	t->member_size = member_size;
	t->root.value = NULL;
	
	int i;
	for(i=0; i<NBYTE; i++)
		t->root.children[i] = NULL;
}

/*
 * auxiliar function.
 * destroy a node recursively.
 */
void trie_destroy_tnode (struct tnode_t* node)
{
	if( node ){
		int i;
		for(i=0; i<NBYTE; i++)
			trie_destroy_tnode(node->children[i]);

		if( node->value )
			free(node->value);

		free(node);
	}
}

/** Destroy the members pointed by `t`.
  * The structure is not freed.
  *
  * @param t pointer to the structure
  */
void trie_destroy (struct trie_t* t)
{
	if(!t) return;

	int i;
	
	if(t->root.value)
		free(t->root.value);
	
	for(i=0; i<NBYTE; i++){
		trie_destroy_tnode(t->root.children[i]);
		t->root.children[i] = NULL;
	}
	t->size = 0;
	t->member_size = 0;
}

/** Adds the `elem` and maps it with the `string` with size `size`.
  * This function overwrite any data left in the trie mapped with string.
  *
  * @param t		pointer to the trie structure;
  * @param string	pointer to the string of bytes to map elem;
  * @param size		size of the string of bytes
  * @param elem		pointer to the element to add
  */
void trie_add_element (struct trie_t* t, void* string, size_t size, void* elem)
{
	if(!t || !string || !elem) return;
	struct tnode_t* node = node_at_and_allocate(t, string, size);
	node->value = malloc(sizeof(t->member_size));
	memcpy(node->value, elem, t->member_size);
	t->size++;
}

/** Removes the element mapped by `string`.
  *
  * @param t		pointer to the structure trie_t;
  * @param string	pointer to the string of bytes to map elem;
  * @param size		size of the string of bytes.
  *
  * @return pointer to the removed element
  */
void* trie_remove_element (struct trie_t* t, void* string, size_t size)
{
	if(!t || !string) return NULL;

	struct tnode_t* node = node_at(t, string, size);
	if(!node) return NULL;

	void* removed_value = node->value;

	node->value = NULL;

	t->size--;
	return removed_value;
}

/** Returns the element mapped by `string`.
  *
  * @param t		pointer to the structure;
  * @param string	pointer to the string of bytes to map elem;
  * @param size		size of the string of bytes.
  *
  * @return The removed element mapped by `string`.
  */
void* trie_get_element (struct trie_t* t, void* string, size_t size)
{
	if(!t || !string) return NULL;
		
	return node_at(t, string, size)->value;
}

/** Sets the value mapped by `string`.
  * Encapsulates the remove and add functions.
  *
  * @param t		pointer to the structure;
  * @param string	pointer to the string of bytes to map elem;
  * @param size		size of the string of bytes.
  * @param elem		pointer to the element to add
  */
void trie_set_element (struct trie_t* t, void* string, size_t size, void* elem)
{

	if(!t || !string) return;
		
	struct tnode_t* node = node_at(t, string, size);
	if(node){
		free(node->value);
		node->value = elem;
	}

}
