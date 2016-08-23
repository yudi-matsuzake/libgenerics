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
#include "graph.h"

/** Creates a graph and populates the previous
  * allocated structure pointed by `g`;
  *
  * @param g		pointer to a graph structure;
  * @param member_size	size of the elements that will be
  * 			indexed by `g`
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `g` is a NULL
  */
gerror_t graph_create(graph_t* g, size_t size, size_t member_size)
{
	if(!g) return GERROR_NULL_STRUCTURE;
	g->V = size;
	g->E = 0;
	g->member_size = member_size;

	g->adj = (queue_t*) malloc(sizeof(queue_t)*size);
	size_t i;
	for(i=0; i<size; i++)
		queue_create(&g->adj[i], sizeof(int));

	if( g->member_size ){
		g->label = malloc(g->member_size*size);
		memset(g->label, 0, size*g->member_size);
	}else{
		g->label = NULL;
	}

	return GERROR_OK;
}

/** Adds an edge on the graph `g` from the vertex `from`
  * to the vertex `to`. Where `from` and `to` are indexes
  * of these vertex.
  *
  * @param g		pointer to a graph structure;
  * @param from		index of the first vertex;
  * @param to		index of the incident vertex.
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_TRY_ADD_EDGE_NO_VERTEX in case that
  * 		`from` or `to` not exists in the graph
  *
  */
gerror_t graph_add_edge(graph_t* g, size_t from, size_t to)
{
	if(!g) return GERROR_NULL_STRUCTURE;
	if(from > g->V || to > g->V) return GERROR_TRY_ADD_EDGE_NO_VERTEX;
	queue_enqueue(&g->adj[from], &to);
	g->E++;

	return GERROR_OK;
}

/** Gets the label of the vertex in the `index` position
  * of the graph `g`.
  *
  * @param g		pointer to a graph structure;
  * @param index	index of the vertex;
  * @param label	pointer to the memory allocated that
  * 			will be write with the label in `index`
  * 
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `g` is a NULL
  *
  */
gerror_t graph_get_label_at(graph_t* g, size_t index, void* label)
{
	if(!g) return GERROR_NULL_STRUCTURE;
	if (index >= g->V) return GERROR_ACCESS_OUT_OF_BOUND;

	if(g->label && g->member_size && label){
		void* ptr = g->label + (index * g->member_size);
		memcpy(label, ptr, g->member_size);
	}

	return GERROR_OK;
}

/** Sets the label at the `index` to `label`.
  *
  * @param g		pointer to a graph structure;
  * @param index	index of the vertex;
  * @param label	the new label of the vertex positioned
  * 			in `index`
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_ACCESS_OUT_OF_BOUND in case that `index`
  * 		is out of bound
  */
gerror_t graph_set_label_at(graph_t* g, size_t index, void* label)
{
	if(!g) return GERROR_NULL_STRUCTURE; 
	if(index >= g->V) return GERROR_ACCESS_OUT_OF_BOUND;

	if(g->member_size){
		void* at = g->label + (index * g->member_size);
		memcpy(at, label, g->member_size);
	}

	return GERROR_OK;
}

/** Deallocates the structures in `g`.
  * This function WILL NOT deallocate the pointer `g`.
  *
  * @param g		pointer to a graph structure;
  *
  * @return	GERROR_OK in case of success operation;
  * 		GERROR_NULL_STRUCURE in case `g` is a NULL
  *
  */
gerror_t graph_destroy(graph_t* g)
{
	if(!g) return GERROR_NULL_STRUCTURE;
	g->member_size = 0;

	size_t i;
	for(i=0; i<g->V; i++){
		queue_destroy(&g->adj[i]);
	}
	
	free(g->adj);
	if(g->label)
		free(g->label);
	g->V = g->E = 0;

	return GERROR_OK;
}
