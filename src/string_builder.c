#include "string_builder.h"
#include "utils.h"

#include <stdalign.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FALSE 0

struct string_builder {
  char *string;          // string being built
  arena *arena;          // memory block used for allocations
  unsigned int size;     // number of characters that compose the string
  unsigned int capacity; // size of array before resizing
};

int string_builder_create(string_builder **sb, unsigned int initial_capacity,
                          arena *arena) {
  if (((*sb) = arena_alloc(arena, sizeof(string_builder),
                           alignof(string_builder), FALSE)) == NULL) {
    return 1;
  }

  (*sb)->capacity = ROUND_POW2(initial_capacity);

  if (((*sb)->string = arena_alloc(arena, sizeof(char) * (*sb)->capacity,
                                   alignof(char), FALSE)) == NULL) {
    return 1;
  }

  (*sb)->size = 0;
  (*sb)->arena = arena;

  return 0;
}

/**
 * @brief Resize the string array after capacity has been reached/exceeded.
 *
 * @param array string array to modifiy.
 * @return 0 on success, 1 otherwise
 */
static int string_builder_resize(string_builder **sb) {
  size_t old_capacity = (*sb)->capacity;

  if (((*sb)->string = arena_realloc((*sb)->arena, (*sb)->string,
                                     old_capacity * sizeof(char),
                                     sizeof(char) * ((*sb)->capacity <<= 1),
                                     alignof(char), FALSE)) == NULL) {
    return 1;
  }

  return 0;
}

int string_builder_append(string_builder *sb, const char *str) {
  int str_length = strlen(str);
  if (sb == NULL || str_length == 0) {
    return 1;
  }

  while (sb->capacity < sb->size + str_length) {
    string_builder_resize(&sb);
  }

  memcpy(sb->string + (sb->size * sizeof(char)), str,
         sizeof(char) * str_length);

  sb->size += str_length;

  return 0;
}

int string_builder_append_char(string_builder *sb, const char ch) {
  if (sb == NULL) {
    return 1;
  }

  if (sb->capacity < sb->size + sizeof(char)) {
    string_builder_resize(&sb);
  }

  memcpy(sb->string + sb->size * sizeof(char), (void *)&ch, sizeof(char));

  return 0;
}

int string_builder_append_printf(string_builder *sb, const char *format, ...) {
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

  while (sb->capacity < sb->size + num_chars) {
    string_builder_resize(&sb);
  }

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

  while (sb->capacity < sb->size + view_size) {
    string_builder_resize(&sb);
  }

  memcpy(sb->string + (sb->size * sizeof(char)), view_data,
         sizeof(char) * view_size);

  sb->size += view_size;

  return 0;
}

int string_builder_build(string_builder *sb, char **buffer) {
  int bytes = sizeof(char) * sb->size;

  if (((*buffer) = arena_alloc(sb->arena, bytes + 1, alignof(char), FALSE)) ==
      NULL) {
    return 1;
  }

  memcpy(*buffer, sb->string, bytes);
  (*buffer)[bytes] = '\0';

  return 0;
}

int string_builder_size(const string_builder *sb) {
  if (sb == NULL) {
    return -1;
  }

  return sb->size;
}
