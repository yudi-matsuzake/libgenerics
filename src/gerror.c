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
#include "gerror.h"

char* gerror_to_string[GERROR_N_ERROR] = {
	"Success",
	"Null pointer to structure",
	"Null pointer to the head of structure",
	"Null pointer to the node",
	"Attempt to remove an element but the structure is empty",
	"Attempt to add a edge with inexistent vertex",
	"Attempt to access a position out of the container or buffer",
};

char* gerror_to_str (gerror_t g)
{
	if( g < GERROR_N_ERROR )
		return gerror_to_string[g];

	return NULL;
}
