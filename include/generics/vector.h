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
#ifndef __VECTOR_T_H__
#define __VECTOR_T_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gerror.h"

typedef struct vector_t {
	void* data;
	size_t size;
	size_t buffer_size;
	size_t member_size;
} vector_t;

gerror_t vector_create (vector_t* v, size_t initial_size, size_t member_size);
gerror_t vector_destroy (vector_t* v);
gerror_t vector_resize_buffer (vector_t* v, size_t new_size);
gerror_t vector_at (vector_t* v, size_t index, void* elem);
gerror_t vector_append (vector_t* v0, vector_t* v1);
gerror_t vector_append_buffer (vector_t* v, void* buffer, size_t size);
void* vector_ptr_at (vector_t* v, size_t index);
gerror_t vector_set_elem_at (vector_t* v, size_t index, void* elem);
gerror_t vector_add (vector_t* v, void* elem);
void vector_set_min_buf_siz(size_t new_min_buf_size);
size_t vector_get_min_buf_siz(void);

#endif
