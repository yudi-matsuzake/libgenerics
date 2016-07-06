#include <stdio.h>
#include <string.h>
#include "trie.h"

int main()
{
	trie_t t;
	trie_create(&t, sizeof(int));

	char* nome = "yudi";
	int i = 50;
	trie_add_element(&t, nome, strlen(nome), &i);

	nome = "bárbara";
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

	nome = "yudi";
	int* n = (int*)trie_get_element(&t, nome, strlen(nome));
	printf("[%s] = %d\n", nome, *n);

	nome = "bárbara";
	n = (int*)trie_get_element(&t, nome, strlen(nome));
	printf("[%s] = %d\n", nome, *n);

	nome = "raul";
	n = (int*)trie_get_element(&t, nome, strlen(nome));
	printf("[%s] = %d\n", nome, *n);

	nome = "tutu";
	n = (int*)trie_get_element(&t, nome, strlen(nome));
	printf("[%s] = %d\n", nome, *n);

	nome = "boris";
	n = (int*)trie_get_element(&t, nome, strlen(nome));
	printf("[%s] = %d\n", nome, *n);

	trie_destroy(&t);
}
