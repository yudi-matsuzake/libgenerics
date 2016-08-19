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
tnode_t* trie_get_node_or_allocate ( struct trie_t* t, void* string, size_t size);

#endif
