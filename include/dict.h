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

#ifndef __DICT_H__
#define __DICT_H__

#include "gerror.h"
#include "vector.h"
#include "map.h"
#include "string.h"

typedef struct dict_t {
	size_t member_size;
	map_t map;
} dict_t;

gerror_t dict_create(struct dict_t* d, size_t member_size);
gerror_t dict_destroy(struct dict_t* d);
gerror_t dict_insert(struct dict_t* d, const char* key, void* elem);
gerror_t dict_remove(struct dict_t* d, const char* key, void* elem);
gerror_t dict_at(struct dict_t* d, const char* key, void* elem);

#endif
