#include "include/string_view.h"

#include <stdalign.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0

struct string_view {
  const char *data;
  unsigned int size;
  arena *arena; // memory block for allocations
};

int string_view_create(string_view **view, const char *source, arena *arena) {
  unsigned long source_length = strlen(source);
  if (((*view) = arena_alloc(arena, sizeof(string_view), alignof(string_view),
                             FALSE)) == NULL) {
    return 1;
  }

  (*view)->data = (source_length > 0) ? source : NULL;
  (*view)->size = source_length;
  (*view)->arena = arena;

  return 0;
}

int string_view_split(string_view *view, string_view *output,
                      const char delimiter) {
  if (view->data == NULL || view->size == 0) {
    output->data = NULL;
    output->size = 0;

    return 1;
  }

  output->data = view->data;
  output->size = view->size;

  for (unsigned int i = 0; i < view->size; i++) {
    if (view->data[i] == delimiter) {
      view->data += i + 1;
      view->size -= i + 1;

      output->size = i;

      return 0;
    }
  }

  // delimiter was not found.
  view->data = NULL;
  view->size = 0;

  return 0;
}

int string_view_trim(string_view *view) {
  // string view must be defined and not empty
  if (view->data == NULL || view->size == 0) {
    return 1;
  }

  // trim left
  while (view->data[0] == ' ') {
    view->size--;
    view->data++;
  }

  // trim right
  while (view->data[view->size - 1] == ' ') {
    view->size--;
  }

  return 0;
}

int string_view_to_string(string_view *view, char **str) {
  // string view is empty.
  if (view->data == NULL || view->size == 0) {
    *str = NULL;
    view->size = 0;

    return 1;
  }

  if (((*str) = arena_alloc(view->arena, view->size + 1, sizeof(char *),
                            FALSE)) == NULL) {
    return 1;
  }

  memcpy(*str, view->data, view->size);
  (*str)[view->size] = '\0';

  return 0;
}

int string_view_starts_with(string_view *view, const char *prefix) {
  if (view->data == NULL || view->size == 0) {
    return 1;
  }

  if (strncmp(view->data, prefix, strlen(prefix)) != 0) {
    return 1;
  }

  return 0;
}

int string_view_ends_with(string_view *view, const char *suffix) {
  unsigned int suffix_length = strlen(suffix);
  if (view->data == NULL || view->size == 0 || suffix_length == 0) {
    return 1;
  }

  if (strncmp(&view->data[view->size - suffix_length], suffix, suffix_length) !=
      0) {
    return 1;
  }

  return 0;
}

int string_view_compare(string_view *first, string_view *second) {
  if (first->size != second->size) {
    return 1;
  }

  return strncmp(first->data, second->data, first->size);
}

int string_view_size(const string_view *view) {
  if (view == NULL || view->data == NULL) {
    return -1;
  }

  return view->size;
}

char *string_view_data(const string_view *view) {
  if (view->data == NULL || view->size == 0) {
    return NULL;
  }

  return (char *)view->data;
}
