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

Installation
------------

```shell
$ make
$ sudo make install
```

To make sure everything went well you may compile the examples:

```
$ make build/examples
```

Uninstall:

```
$ make uninstall
```

UNLICENSE
---------

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

For more information, please refer to [<http://unlicense.org/>](http://unlicense.org/)

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
- [x] vector
	- [x] create
	- [x] destroy
	- [x] resize
	- [x] add
	- [x] at
	- [x] set_elem_at
- [ ] priority queue
