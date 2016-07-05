#include "graph.h"

void graph_create(graph_t* g, size_t size, size_t member_size)
{
	g->V = size;
	g->E = 0;
	g->member_size = member_size;

	g->adj = (queue_t*) malloc(sizeof(queue_t)*size);
	size_t i;
	for(i=0; i<size; i++)
		queue_create(&g->adj[i], sizeof(int));

	g->label = malloc(g->member_size*size);
	memset(g->label, 0, size*g->member_size);
}

void graph_add_edge(graph_t* g, size_t from, size_t to)
{
	if(!g || from > g->V || to > g->V) return;
	queue_enqueue(&g->adj[from], &to);
	g->E++;
}

void* graph_get_label_at(graph_t* g, size_t index)
{
	if(!g || index >= g->V) return NULL;
	return g->label + (index * g->member_size);
}

void graph_set_label_at(graph_t* g, size_t index, void* label)
{
	if(!g || index >= g->V) return;

	void* at = graph_get_label_at(g, index);
	memcpy(at, label, g->member_size);
}

void graph_destroy(graph_t* g)
{
	g->member_size = 0;

	size_t i;
	for(i=0; i<g->V; i++){
		queue_destroy(&g->adj[i]);
	}

	free(g->label);
	g->V = g->E = 0;
}
