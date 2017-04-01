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
#ifndef __GERROR_H__
#define __GERROR_H__
#include <stdio.h>
#include <stdlib.h>

typedef enum gerror_t{
	GERROR_OK,
	GERROR_NULL_STRUCTURE,
	GERROR_NULL_HEAD,
	GERROR_NULL_NODE,
	GERROR_NULL_POINTER_TO_BUFFER,
	GERROR_TRY_REMOVE_EMPTY_STRUCTURE,
	GERROR_TRY_ADD_EDGE_NO_VERTEX,
	GERROR_ACCESS_OUT_OF_BOUND,
	GERROR_INCOMPATIBLE_VECTOR_APPEND_SIZE,
	GERROR_NULL_RETURN_POINTER,
	GERROR_EMPTY_STRUCTURE,
	GERROR_NULL_ELEMENT_POINTER,
	GERROR_REMOVE_ELEMENT_NOT_FOUNDED,
	GERROR_ELEMENT_NOT_FOUNDED,
	GERROR_N_ERROR
} gerror_t;

char* gerror_to_str (gerror_t g);

void gerror_abort_message (
		gerror_t g,
		const char* call,
		const char* file,
		int line_number);

#define _g_assert(X, F, LINE)			\
{						\
	gerror_t __gerror__;			\
	if((__gerror__ = X) != GERROR_OK){	\
		gerror_abort_message(		\
				__gerror__,	\
				#X,		\
				F,		\
				LINE);		\
	}					\
}

#define g_assert(X) _g_assert(X, __FILE__, __LINE__)

#endif
