#include "include/string_builder.h"
#include "include/dynamic_array.h"

#include <stdalign.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0

struct string_builder {
  dynamic_array *string; // string being built
  arena *arena;          // memory block used for allocations
};

int string_builder_create(string_builder **sb, unsigned int initial_capacity,
                          arena *arena) {
  if (((*sb) = arena_alloc(arena, sizeof(string_builder),
                           alignof(string_builder), FALSE)) == NULL) {
    return 1;
  }

  (*sb)->arena = arena;

  return dynamic_array_create(&(*sb)->string, initial_capacity, sizeof(char),
                              NULL, arena);
}

int string_builder_append(string_builder *sb, const char *str) {
  return dynamic_array_add_many(sb->string, (void **)str, strlen(str));
}

int string_builder_append_char(string_builder *sb, const char ch) {
  return dynamic_array_add(sb->string, &ch);
}

int string_builder_append_fmt_str(string_builder *sb, const char *format, ...) {
  int result = 0;
  // number of characters the formatted string will contain
  // excluding the null byte.
  int num_chars = 0;
  // number bytes to allocate.
  int size = 0;
  // temp buffer to store formatted string.
  char *buffer = NULL;
  va_list args;

  va_start(args, format);
  num_chars = vsnprintf(buffer, size, format, args);
  va_end(args);

  size = num_chars + 1; // extra byte for '\0'

  if ((buffer = arena_alloc(sb->arena, size, alignof(char), FALSE)) == NULL) {
    result = 1;
    goto exit;
  }

  va_start(args, format);
  num_chars = vsnprintf(buffer, size, format, args);
  va_end(args);

  if ((string_builder_append(sb, buffer)) != 0) {
    result = 1;
    goto exit;
  }

exit:
  return result;
}

int string_builder_append_view(string_builder *sb, const string_view *view) {
  int view_size = string_view_size(view);
  char *view_data = string_view_data(view);

  if (sb == NULL || view_data == NULL || view_size == 0) {
    return 1;
  }

  return dynamic_array_add_many(sb->string, (void **)view_data, view_size);
}

int string_builder_build(string_builder *sb, char **buffer) {
  int bytes = sizeof(char) * dynamic_array_get_size(sb->string);

  if (((*buffer) = arena_alloc(sb->arena, bytes + 1, alignof(char), FALSE)) ==
      NULL) {
    return 1;
  }

  void *item = NULL;
  dynamic_array_find_ref(sb->string, 0, &item);

  memcpy(*buffer, item, bytes);
  (*buffer)[bytes] = '\0';

  return 0;
}

int string_builder_is_empty(string_builder *sb) {
  return dynamic_array_is_empty(sb->string);
}
