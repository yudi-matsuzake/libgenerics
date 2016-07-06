#include "graph.h"

/** Creates a graph and populates the previous
  * allocated structure pointed by `g`;
  *
  * @param g		pointer to a graph structure;
  * @param member_size	size of the elements that will be
  * 			indexed by `g`
  *
  */
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

/** Adds an edge on the graph `g` from the vertex `from`
  * to the vertex `to`. Where `from` and `to` are indexes
  * of these vertex.
  *
  * @param g		pointer to a graph structure;
  * @param from		index of the first vertex;
  * @param to		index of the incident vertex.
  *
  */
void graph_add_edge(graph_t* g, size_t from, size_t to)
{
	if(!g || from > g->V || to > g->V) return;
	queue_enqueue(&g->adj[from], &to);
	g->E++;
}

/** Gets the label of the vertex in the `index` position
  * of the graph `g`.
  *
  * @param g		pointer to a graph structure;
  * @param index	index of the vertex;
  * 
  * @return	pointer to the label of the vertex positioned
  * 		in `index`.
  *
  */
void* graph_get_label_at(graph_t* g, size_t index)
{
	if(!g || index >= g->V) return NULL;
	return g->label + (index * g->member_size);
}

/** Sets the label at the `index` to `label`.
  *
  * @param g		pointer to a graph structure;
  * @param index	index of the vertex;
  * @param label	the new label of the vertex positioned
  * 			in `index`
  *
  */
void graph_set_label_at(graph_t* g, size_t index, void* label)
{
	if(!g || index >= g->V) return;

	void* at = graph_get_label_at(g, index);
	memcpy(at, label, g->member_size);
}

/** Deallocates the structures in `g`.
  * This function WILL NOT deallocate the pointer `g`.
  *
  * @param g		pointer to a graph structure;
  *
  */
void graph_destroy(graph_t* g)
{
	g->member_size = 0;

	size_t i;
	for(i=0; i<g->V; i++){
		queue_destroy(&g->adj[i]);
	}
	
	free(g->adj);
	free(g->label);
	g->V = g->E = 0;
}
