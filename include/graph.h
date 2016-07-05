#ifndef __GRAPH_T_H__
#define __GRAPH_T_H__
#include <string.h>
#include <assert.h>
#include "queue.h"

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

#endif // __GRAPH_T_H__
