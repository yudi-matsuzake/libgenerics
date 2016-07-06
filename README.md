libgenerics
-----------

libgenerics is a minimalistic and generic library for C basic data structures.

The entire implementation was aimed at providing two _features_:

1. fun;

2. reusable, generic and easy-to-read code like this:

```c
#include <stdio.h>
#include "queue.h"

#define N 10

int main()
{
	int i;
	queue_t q;
	queue_create(&q, sizeof(int));

	for( i=0; i<N; i++ )
		queue_enqueue(&q, &i);


	for( i=0; i<N/2; i++ )
		free(queue_dequeue(&q));


	queue_destroy(&q);
	return 0;
}
```

The doxygen documentation is available ![here](https://github.com/yudi-matsuzake/libgenerics/blob/master/doc/doc.pdf).

Checklist
---------

- [x] queue
	- [x] enqueue
	- [x] dequeue
	- [x] create
	- [x] destroy
	- [x] remove
- [ ] stack
	- [x] push
	- [x] pop
	- [x] create
	- [x] destroy
	- [ ] remove
- [x] trie
	- [x] create
	- [x] destroy
	- [x] add element
	- [x] remove element
	- [x] get element
	- [x] set element
- [ ] red-black tree
- [ ] graph
	- [x] create
	- [x] destroy
	- [x] add edge by index
	- [ ] add edge by labels
	- [x] add label
	- [x] set label
	- [ ] add vertex
	- [ ] remove edge
	- [ ] remove vertex
	- [ ] algorithms
		- [ ] depth first search 
		- [ ] breadth first search
		- [ ] euclidean path
		- [ ] dijkstra algorithm
