#include <stdio.h>
#include <generics/graph.h>

#define N 4

void printf_graph(graph_t* g)
{

	size_t i;

	for(i=0; i<g->V; i++)
	{
		int label;
		graph_get_label_at(g, i, &label);

		printf("[%d]->", label);
		qnode_t* j;

		for(j=g->adj[i].head; j!=NULL; j = j->next){
			int adj_index = *(int*)j->data;
			graph_get_label_at(g, adj_index, &label);

			printf("%d%s", label, (j->next)?", ":"");
		}
		printf("\n");
	}

}

int main()
{

	graph_t g;
	graph_create(&g, N, sizeof(int));

	graph_add_edge(&g, 0, 1);
	graph_add_edge(&g, 2, 1);
	graph_add_edge(&g, 3, 0);
	graph_add_edge(&g, 3, 1);
	graph_add_edge(&g, 3, 2);

	int i;
	for(i=1; i<=N; i++)
		graph_set_label_at(&g, i-1, &i);

	printf_graph(&g);

	graph_destroy(&g);
	return 0;
}
