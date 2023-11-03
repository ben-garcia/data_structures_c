#include "vector.h"

#include <stdio.h>
#include <string.h>

#define VECTOR_INTIAL_CAPACITY 16

struct vector {
  void **items;
  void (*freefn)(void *);
  bool (*matchfn)(void *, void *);
  long capacity;    /* limit of items */
  long size;        /* current number of items */
  size_t data_size; /* bytes needed for every item */
};

struct vector_iter {
  void **items;
  int size;
  int index;
};

vector *vector_create(size_t data_size) {
  vector *vec;
  if ((vec = malloc(sizeof(*vec))) == NULL) {
    return NULL;
  }

  vec->data_size = data_size;
  vec->size = 0;
  vec->freefn = NULL; 
  vec->capacity = VECTOR_INTIAL_CAPACITY;
  vec->items = NULL;

  return vec;
}

static void vector_resize(vector *vector) {
  size_t old_capcity = vector->capacity;
  vector->items =
      realloc(vector->items, vector->data_size * (vector->capacity <<= 1));
  /* since realloc doesn't zero out new space allocated,
   * it is done manually.
   */
  memset(vector->items + old_capcity, 0,
         vector->data_size * (vector->capacity - old_capcity));
}

void vector_set_free_function(vector *vector, void (*freefn)(void *)) {
  vector->freefn = freefn;
}

void vector_set_match_function(vector *vector, bool (*matchfn)(void *, void *)) {
  vector->matchfn = matchfn;
}

void *vector_find(vector *vector, void *item) {
  if (vector->size == 0) {
    return NULL;
  }

  for (int i = 0; i < vector->capacity; i++) {
    if (vector->matchfn != NULL) {
      if (vector->matchfn(vector->items[i], item))
        return vector->items[i];
    } else {
      if (vector->items[i] == item)
        return vector->items[i];
    }
  }

  return NULL;
}

void vector_push(vector *vector, void *item) {
  if (vector == NULL) {
    return;
  }

  if (vector->size == 0) {
    vector->items = calloc(vector->capacity, vector->data_size);
  }

  if (vector->size == vector->capacity) {
    vector_resize(vector);
  }

  vector->items[vector->size++] = item;
}

void vector_pop(vector *vector, void *item) {
  if (vector == NULL) {
    return;
  }

  for (size_t i = 0; i < vector->size; i++) {
    if (vector->items[i] == item) {
      vector->size--; /* decrement count */
      memmove(vector->items + i, vector->items + i + 1,
              vector->data_size * (vector->size - i));

      vector->items[vector->size] = 0;
      return;
    }
  }

  return;
}

void vector_destroy(vector *vector) {
  if (vector == NULL) {
    return;
  }

  for (int i  = 0; i < vector->size; i++) {
    if (vector->freefn != NULL) {
      vector->freefn(vector->items[i]);
    }
  }

  free(vector->items);
  free(vector);
}

vector_iter *vector_iter_create(vector *vector) {
  vector_iter *it;

  if ((it = malloc(sizeof(*it))) == NULL) {
    return NULL;
  }

  it->items = vector->items;
  it->size = vector->size;
  it->index = 0;

  return it;
}

void vector_iter_destroy(vector_iter *it) {
  free(it);
}

void *vector_iter_next(vector_iter *it) {
  if (it->size == 0) {
    return NULL;
  }

  if (it->index >= it->size - 1) {
    return NULL;
  }

  it->index++;

  return it->items[it->index];
}

void vector_iter_reset(vector_iter *it) {
  it->index = 0;
}
