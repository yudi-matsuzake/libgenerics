#include <stdio.h>
#include <string.h>
#include <generics/trie.h>
#include <generics/queue.h>

void print_lexicography_node(struct tnode_t *node, struct queue_t* q)
{
	if(node && q){

		if(node->value){

			struct qnode_t* qnode;
			printf("[");
			for(qnode=q->head; qnode!=NULL; qnode=qnode->next){
				printf("%c", *(char*)qnode->data);
			}
			printf("]");
			printf(" -> %d\n", *(int*)node->value);
		}

		int i;
		for ( i = 0; i<NBYTE ; i++ ){
			char c = i;
			queue_enqueue(q, &c);
			print_lexicography_node(node->children[i], q);
			queue_remove(q, q->tail, NULL);
		}

	}
}

void print_lexicography(struct trie_t* t)
{
	queue_t q;
	queue_create(&q, sizeof(char));
	print_lexicography_node(&t->root, &q);
	queue_destroy(&q);
}

int main()
{
	trie_t t;
	trie_create(&t, sizeof(int));

	char* nome = "yudi";
	int i = 50;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "";
	i = -1;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "barbara";
	i = 100;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "raul";
	i = 10;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "tutu";
	i = 1;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "boris";
	i = 100000;
	trie_add_element(&t, nome, strlen(nome), &i);

	print_lexicography(&t);
	printf("remove: %s\n", nome);
	trie_remove_element(&t, nome, strlen(nome));
	print_lexicography(&t);

	trie_destroy(&t);
}
