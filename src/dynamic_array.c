#include "include/dynamic_array.h"

#include <stdlib.h>
#include <string.h>

struct dynamic_array {
  void **items;
  void (*freefn)(void **);
  int (*matchfn)(void *, void *);
  unsigned int capacity;  // limit of items before resizing.
  unsigned int size;      // number of elements in the array.
  unsigned int data_size; // bytes needed for every element..
};

struct dynamic_array_iter {
  void **items;
  unsigned data_size;
  unsigned size;
  unsigned index; // current element iterator is pointing to.
};

/**
 * @brief Resize the dynamic array after capacity has been reached/exceeded.
 *
 * @param array dynamic array to modifiy.
 * @return 0 on success, 1 otherwise
 */
static int dynamic_array_resize(dynamic_array **array) {
  size_t old_capacity = (*array)->capacity;

  if (((*array)->items =
           realloc((*array)->items,
                   (*array)->data_size * ((*array)->capacity <<= 1))) == NULL) {
    return 1;
  }

  // since realloc doesn't zero out new space allocated,
  // it is done manually.
  memset((void *)(*array)->items + (old_capacity * (*array)->data_size), 0,
         (*array)->capacity - old_capacity);

  return 0;
}

int dynamic_array_create(dynamic_array **array, unsigned int data_size,
                         void (*freefn)(void **),
                         int (*matchfn)(void *, void *)) {

  if ((*array = malloc(sizeof(dynamic_array))) == NULL) {
    return 1;
  }

  (*array)->capacity = 16; // initial capacity
  (*array)->data_size = data_size;
  (*array)->freefn = freefn;
  (*array)->matchfn = matchfn;
  (*array)->items = NULL;
  (*array)->size = 0;

  return 0;
}

int dynamic_array_add(dynamic_array *array, const void *item) {
  // array must be defined.
  if (array == NULL || item == NULL) {
    return 1;
  }

  // array can't be empty.
  if (array->size == 0) {
    array->items = calloc(array->capacity, array->data_size);
  }

  if (array->size == array->capacity) {
    dynamic_array_resize(&array);
  }

  array->items[array->size] = (void *)item;
  array->size++;

  return 0;
}

int dynamic_array_add_many(dynamic_array *array, void **items,
                           unsigned int length) {
  // array must be defined.
  if (array == NULL || (*items) == NULL) {
    return 1;
  }

  if (dynamic_array_is_empty(array)) {
    array->items = calloc(array->capacity, array->data_size);
  }

  // make sure array has the necessary memory.
  while (array->capacity < array->size + length) {
    dynamic_array_resize(&array);
  }

  memcpy((void *)array->items + array->size * array->data_size, items,
         array->data_size * length);

  array->size += length;

  return 0;
}

int dynamic_array_find(dynamic_array *array, unsigned int index, void **item) {
  // array must be defined.
  if (array == NULL || dynamic_array_is_empty(array) || index >= array->size) {
    return 1;
  }

  memcpy(*item, array->items + index, array->data_size);

  return 0;
}

int dynamic_array_find_ref(dynamic_array *array, unsigned int index,
                           void **item) {
  // array must be defined.
  if (array == NULL || array->size == 0 || index >= array->size) {
    return 1;
  }

  *item = (void *)array->items + index * array->data_size;

  return 0;
}

int dynamic_array_get_size(dynamic_array *array) {
  if (array == NULL) {
    return -1;
  }
  return array->size;
}

int dynamic_array_is_empty(dynamic_array *array) { return array->size == 0; }

int dynamic_array_remove(dynamic_array *array, unsigned int index) {
  // array must be defined.
  if (array == NULL || dynamic_array_is_empty(array) || index >= array->size) {
    return 1;
  }

  array->size--;
  memmove(array->items + index, array->items + index + 1,
          array->data_size * array->size);

  return 0;
}

int dynamic_array_shrink_to_fit(dynamic_array *array) {
  // Array must be defined.
  if (array == NULL || array->size == array->capacity) {
    return 1;
  }

  array->items = realloc(array->items, array->size * array->data_size);
  array->capacity = array->size;

  return 0;
}

void dynamic_array_destroy(dynamic_array **array) {
  if (*array == NULL) {
    return;
  }

  if ((*array)->freefn != NULL) {
    for (unsigned int i = 0; i < (*array)->size; i++) {
      (*array)->freefn(&((*array)->items[i]));
    }
  }

  free((*array)->items);
  (*array)->items = NULL;
  free(*array);
  *array = NULL;
}

int dynamic_array_iter_create(dynamic_array_iter **it, dynamic_array *array) {
  if ((*it = malloc(sizeof(dynamic_array_iter))) == NULL) {
    return 1;
  }

  (*it)->items = array->items;
  (*it)->size = array->size;
  (*it)->data_size = array->data_size;
  (*it)->index = 0;

  return 0;
}

int dynamic_array_iter_next(dynamic_array_iter *it, void **item) {
  if (it->size == 0 || it->index >= it->size) {
    return 1;
  }

  // NOTE: A reference is returned.
  *item = it->items[it->index];
  it->index++;

  return 0;
}

int dynamic_array_iter_reset(dynamic_array_iter *it) {
  if (it == NULL) {
    return 1;
  }

  it->index = 0;
  return 0;
}

int dynamic_array_iter_destroy(dynamic_array_iter **it) {
  if (*it == NULL) {
    return 1;
  }

  free(*it);
  *it = NULL;
  return 0;
}
