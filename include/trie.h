#ifndef __TRIE_H__
#define __TRIE_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBYTE (0x100)

/** node of a trie_t element.
  */
typedef struct tnode_t {
	void* value;
	struct tnode_t* children[NBYTE];
} tnode_t;

/** Represents the trie structure.
  */
typedef struct trie_t {
	size_t size;
	size_t member_size;
	struct tnode_t root;
} trie_t;

void trie_create(struct trie_t* t, size_t member_size);
void trie_destroy(struct trie_t* t);
void trie_add_element(struct trie_t* t, void* string, size_t size, void* elem);
void* trie_remove_element(struct trie_t* t, void* string, size_t size);
void* trie_get_element(struct trie_t* t, void* string, size_t size);
void trie_set_element(struct trie_t* t, void* string, size_t size, void* elem);

#endif // __TRIE_H__
