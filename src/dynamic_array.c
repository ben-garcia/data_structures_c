#include "dynamic_array.h"
#include "utils.h"

#include <stdalign.h>
#include <string.h>

#define ZERO_OUT_FALSE 0

struct dynamic_array {
  void *items;
  int (*matchfn)(void *, void *); // comparison function
  arena *arena;                   // memory block used for allocations
  unsigned int capacity;          // limit of items before resizing.
  unsigned int size;              // number of elements in the array.
  unsigned int data_size;         // bytes needed for every element..
};

struct dynamic_array_iterator {
  dynamic_array *array;
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

  if (((*array)->items = arena_realloc(
           (*array)->arena, (*array)->items, old_capacity * (*array)->data_size,
           (*array)->data_size * ((*array)->capacity <<= 1), alignof(void *),
           ZERO_OUT_FALSE)) == NULL) {
    return 1;
  }

  return 0;
}

int dynamic_array_create(dynamic_array **array, unsigned int initial_capacity,
                         unsigned int data_size, int (*matchfn)(void *, void *),
                         arena *arena) {

  if ((*array = arena_alloc(arena, sizeof(dynamic_array),
                            alignof(dynamic_array), ZERO_OUT_FALSE)) == NULL) {
    return 1;
  }

  (*array)->capacity = ROUND_POW2(initial_capacity);
  (*array)->data_size = data_size;
  (*array)->matchfn = matchfn;
  (*array)->arena = arena;
  (*array)->items = NULL;
  (*array)->size = 0;

  return 0;
}

int dynamic_array_add(dynamic_array *array, const void *item) {
  if (array == NULL || item == NULL) {
    return 1;
  }

  if (array->size == 0) { // array can't be empty.
    if ((array->items =
             arena_alloc(array->arena, array->capacity * array->data_size,
                         alignof(void *), ZERO_OUT_FALSE)) == NULL) {
      return 1;
    }
  }

  if (array->size == array->capacity) {
    dynamic_array_resize(&array);
  }

  memcpy((char *)array->items + (array->size * array->data_size), item,
         array->data_size);
  array->size++;

  return 0;
}

int dynamic_array_find_by_index(dynamic_array *array, unsigned int index,
                                void *item) {
  if (array == NULL || array->size == 0 || index >= array->size ||
      item == NULL) {
    return 1;
  }

  memcpy(item, (char *)array->items + (index * array->data_size),
         array->data_size);

  return 0;
}

int dynamic_array_find(dynamic_array *array, void *data, void *item) {
  if (array == NULL || array->size == 0 || data == NULL || item == NULL) {
    return 1;
  }

  for (unsigned int i = 0; i < array->size; i++) {
    if (array->matchfn((char *)array->items + (i * array->data_size), data) ==
        0) {
      memcpy(item, (char *)array->items + (i * array->data_size),
             array->data_size);
      return 0;
    }
  }

  return 1;
}

int dynamic_array_size(dynamic_array *array) {
  if (array == NULL) {
    return -1;
  }
  return array->size;
}

int dynamic_array_is_empty(dynamic_array *array) {
  if (array == NULL) {
    return 1;
  }

  return !(array->size == 0);
}

int dynamic_array_remove_by_index(dynamic_array *array, unsigned int index) {
  if (array == NULL || array->size == 0 || index >= array->size) {
    return 1;
  }

  memmove((char *)array->items + (index * array->data_size),
          (char *)array->items + ((index + 1) * array->data_size),
          array->data_size * (array->size - index - 1));

  array->size--;

  return 0;
}

int dynamic_array_remove(dynamic_array *array, void *data) {
  if (array == NULL || array->size == 0 || array->matchfn == NULL ||
      data == NULL) {
    return 1;
  }

  for (unsigned int i = 0; i < array->size; i++) {
    if (array->matchfn((char *)array->items + (i * array->data_size), data) ==
        0) {
      memmove((char *)array->items + (i * array->data_size),
              (char *)array->items + ((i + 1) * array->data_size),
              array->data_size * (array->size - i - 1));
      array->size--;
      return 0;
    }
  }

  return 1;
}

int dynamic_array_iterator_create(dynamic_array_iterator **it,
                                  dynamic_array *array) {
  if (array == NULL || array->size == 0) {
    return 1;
  }

  if ((*it = arena_alloc(array->arena, sizeof(dynamic_array_iterator),
                         alignof(dynamic_array_iterator), ZERO_OUT_FALSE)) == NULL) {
    return 1;
  }

  (*it)->array = array;
  (*it)->index = 0;

  return 0;
}

int dynamic_array_iterator_next(dynamic_array_iterator *it, void *item) {
  if (it->array->size == 0 || it->index >= it->array->size) {
    return 1;
  }

  memcpy(item, (char *)it->array->items + (it->index * it->array->data_size),
         it->array->data_size);
  it->index++;

  return 0;
}

int dynamic_array_iterator_reset(dynamic_array_iterator *it) {
  if (it == NULL) {
    return 1;
  }

  it->index = 0;
  return 0;
}
