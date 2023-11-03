#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct vector vector;
typedef struct vector_iter vector_iter;

/* vector prototypes */
vector *vector_create(size_t data_size);
void vector_push(vector *vector, void *item);
void vector_set_free_function(vector *vector, void (*freefn)(void *));
void vector_set_match_function(vector *vector, bool (*matchfn)(void *, void *));
void *vector_find(vector *vector, void *item);
void vector_pop(vector *vector, void *item);
void vector_destroy(vector *vector);

/* iterator prototypes */
vector_iter *vector_iter_create(vector *vector);
void vector_iter_destroy(vector_iter *it);
void *vector_iter_next(vector_iter *it);
void vector_iter_reset(vector_iter *it);

#endif
