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
#include "vector.h"

#define VECTOR_MIN_SIZ 8
size_t	vector_min_siz = VECTOR_MIN_SIZ;

/** Populate the `vetor_t` structure pointed by `v`
  * and allocates `member_size`*`initial_size` for initial buffer_size.
  *
  * @param v			a pointer to `vector_t` structure
  *				already allocated;
  * @param inicial_buf_size	number of the members of the initial
  * 				allocated buffer;
  * @param member_size		size of every member indexed by `v`.
  */
void vector_create (vector_t* v, size_t initial_buf_siz, size_t member_size)
{
	if(!v) return;

	v->size = 0;
	v->member_size = member_size;
	if ( initial_buf_siz < vector_min_siz )
		v->buffer_size = initial_buf_siz*member_size;
	else
		v->buffer_size = initial_buf_siz*member_size;

	v->data = malloc(v->buffer_size);
}

/** Destroy the structure `vector_t` pointed by `v`.
  *
  * @param v	a pointer to `vector_t` structure
  */
void vector_destroy (vector_t* v)
{
	if( v )
		free(v->data);
}

/** Returns the `vector_min_siz`: a private variable that holds
  * the minimal number of elements that `vector_t` will index.
  * This variable is important for avoid multiple small resizes
  * in the `vector_t` container.
  *
  * @return vector_min_siz
  */
size_t vector_get_min_buf_siz (void)
{
	return vector_min_siz;
}

/** Set the `vector_min_siz`: a private variable that holds
  * the minimal number of elements that `vector_t` will index.
  * This variable is important for avoid multiple small resizes
  * in the `vector_t` container.
  *
  * @param new_min_buf_siz the new size of `vector_min_siz`
  */
void vector_set_min_buf_siz (size_t new_min_buf_siz)
{
	vector_min_siz = new_min_buf_siz;
}

/** Resize the buffer in the `vector_t` strucuture
  * pointed by `v`.
  *
  * @param v		a pointer to `vector_t`
  *			structure.
  * @param new_size	the new size of the `v`
  */
void vector_resize_buffer (vector_t* v, size_t n_elements)
{
	if(!v) return;

	size_t new_proposed_size = n_elements*v->member_size;
	size_t new_actual_size;

	/*
	 * update the size if need
	 */
	if( n_elements < v->size )
		v->size = n_elements;

	/*
	 * resize the buffer if the size proposed is bigger.
	 * if the size proposed is not much bigger than member
	 * size so a bigger resize value is set. This is to
	 * prevent multiple small resizes.
	 *
	 * if the size proposed is less than buffer the buffer
	 * is resized if the less is bigger than vector_min_siz
	 * else vector_min_siz will be the newsize.
	 */
	size_t min_resize = vector_min_siz*v->member_size;
	if( new_proposed_size >= v->buffer_size ){
		size_t buffer_size_diff = new_proposed_size - v->buffer_size;
		if(	buffer_size_diff < min_resize )
			new_actual_size = v->buffer_size + min_resize;
		else
			new_actual_size = new_proposed_size;
	}else{
		if(new_proposed_size < min_resize )
			new_actual_size = min_resize;
		else
			new_actual_size = new_proposed_size;
	}
	if( new_actual_size != v->buffer_size ){
		v->data = realloc(v->data, new_actual_size);
		v->buffer_size = new_actual_size;
	}
}

/** Get the element in the `index` position indexed by
  * the `vector_t` structure pointed by `v`.
  *
  * @param v		a pointer to `vector_t`
  * @param index	index of the position
  *
  * @return a pointer to the member at `index`
  */
void* vector_at (vector_t* v, size_t index)
{
	size_t at = index*v->member_size;
	if( !v || !v->buffer_size || at >= v->buffer_size)
		return NULL;
	else
		return v->data + at;
}

/** set the element at `index` pointed by `v` with the
  * element pointed by `elem`.
  *
  * @param v		a pointer to `vector_t`
  * @param index	index of the position
  * @param elem		the element to be set in `v`
  */
void vector_set_elem_at (vector_t* v, size_t index, void* elem)
{
	void* ptr = vector_at(v, index);
	if( ptr )
		memcpy(ptr, elem, v->member_size);
}

/** adds the `elem` in the structure `vector_t` pointed
  * by `v`.
  *
  * @param v	a pointer to `vector_t`
  * @param elem	the element to be add in `v`
  */
void vector_add (vector_t* v, void* elem)
{
	if( !v || !elem ) return;
	size_t room_size = v->buffer_size/v->member_size;

	if( room_size < (v->size+1) )
		vector_resize_buffer(v, v->size+1);
	vector_set_elem_at(v, v->size, elem);
	v->size++;
}
