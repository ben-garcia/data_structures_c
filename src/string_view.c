#include "include/string_view.h"

#include <stdlib.h>
#include <string.h>

string_view string_view_create(const char *source) {
  unsigned int source_length = strlen(source);
  string_view view = {0};
  if (source_length < 1) {
    view = (string_view){.data = NULL, .length = 0};
  }
  view = (string_view){.data = source, .length = strlen(source)};

  return view;
}

int string_view_split(string_view *view, string_view *output,
                      const char delimiter) {
  if (view->data == NULL || view->length == 0) {
    output->data = NULL;
    output->length = 0;

    return 1;
  }

  output->data = view->data;
  output->length = view->length;

  for (unsigned int i = 0; i < view->length; i++) {
    if (view->data[i] == delimiter) {
      view->data += i + 1;
      view->length -= i + 1;

      output->length = i;

      return 0;
    }
  }

  // delimiter was not found.
  view->data = NULL;
  view->length = 0;

  return 0;
}

int string_view_trim(string_view *view) {
  // string view must be defined and not empty
  if (view->data == NULL || view->length == 0) {
    return 1;
  }

  // trim left
  while (view->data[0] == ' ') {
    view->length--;
    view->data++;
  }

  // trim right
  while (view->data[view->length - 1] == ' ') {
    view->length--;
  }

  return 0;
}

int string_view_to_string(string_view *view, char **str) {
  // string view is empty.
  if (view->data == NULL || view->length == 0) {
    *str = NULL;
    view->length = 0;

    return 1;
  }

  if (((*str) = malloc(view->length + 1)) == NULL) {
    return 1;
  }

  memcpy(*str, view->data, view->length);
  (*str)[view->length] = '\0';

  return 0;
}

int string_view_starts_with(string_view *view, const char *prefix) {
  if (view->data == NULL || view->length == 0) {
    return 1;
  }

  if (strncmp(view->data, prefix, strlen(prefix)) != 0) {
    return 1;
  }

  return 0;
}

int string_view_ends_with(string_view *view, const char *suffix) {
  unsigned int suffix_length = strlen(suffix);
  if (view->data == NULL || view->length == 0 || suffix_length == 0) {
    return 1;
  }

  if (strncmp(&view->data[view->length - suffix_length], suffix,
              suffix_length) != 0) {
    return 1;
  }

  return 0;
}

int string_view_compare(string_view first, string_view second) {
  if (first.length != second.length) {
    return 1;
  }

  return strncmp(first.data, second.data, first.length);
}
