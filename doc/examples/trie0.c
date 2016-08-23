#include <stdio.h>
#include <string.h>
#include <generics/trie.h>

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
	trie_get_element(&t, nome, strlen(nome), &i);
	printf("[%s] = %d\n", nome, i);

	nome = "bárbara";
	trie_get_element(&t, nome, strlen(nome), &i);
	printf("[%s] = %d\n", nome, i);

	nome = "raul";
	trie_get_element(&t, nome, strlen(nome), &i);
	printf("[%s] = %d\n", nome, i);

	nome = "tutu";
	trie_get_element(&t, nome, strlen(nome), &i);
	printf("[%s] = %d\n", nome, i);

	nome = "boris";
	trie_get_element(&t, nome, strlen(nome), &i);
	printf("[%s] = %d\n", nome, i);

	trie_destroy(&t);
}
