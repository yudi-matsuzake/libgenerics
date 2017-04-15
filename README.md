libgenerics
-----------

libgenerics is a minimalistic and generic library for C basic data structures.

The entire implementation was aimed at providing reusable, generic and easy-to-read code like this:

```c
#include <stdio.h>
#include <generics/queue.h>

#define N 10

int main()
{
	int i;
	queue_t q;
	queue_create(&q, sizeof(int));

	for( i=0; i<N; i++ )
		queue_enqueue(&q, &i);

	int temp;
	for( i=0; i<N/2; i++ ){
		queue_dequeue(&q, &temp);
		printf("dequeued: %d\n", temp);
	}

	queue_destroy(&q);
	return 0;
}
```

The [doxygen](http://doxygen.org/) documentation is available [here](https://github.com/yudi-matsuzake/libgenerics/blob/master/doc/doc.pdf). There's only the pdf version pre-generated, but you can generate the html and the man format with [doxygen](http://doxygen.org/).

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

You can copy the code above to a file name `main.c` and compile after the installation using:

```shell
$ gcc main.c -lgenerics
```

[You may also try another examples](https://github.com/yudi-matsuzake/libgenerics/tree/master/doc/examples).

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
