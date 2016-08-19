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
#ifndef __GRAPH_T_H__
#define __GRAPH_T_H__
#include <string.h>
#include "queue.h"

/** Graph structure and elements.
  */
typedef struct graph_t{
	size_t V;
	size_t E;
	size_t member_size;
	struct queue_t* adj;
	void* label;
}graph_t;

void graph_create(graph_t* g, size_t size, size_t member_size);
void graph_add_edge(graph_t* g, size_t from, size_t to);
void* graph_get_label_at(graph_t* g, size_t index);
void graph_set_label_at(graph_t* g, size_t index, void* label);
void graph_destroy(graph_t* g);

#endif
