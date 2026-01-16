#include "include/string_builder.h"
#include "include/dynamic_array.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct string_builder {
  dynamic_array *string; // string being built
};

int string_builder_create(string_builder **sb) {
  if (((*sb) = malloc(sizeof(string_builder))) == NULL) {
    return 1;
  }

  return dynamic_array_create(&(*sb)->string, sizeof(char), NULL, NULL);
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

  if ((buffer = malloc(size)) == NULL) {
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
  if (buffer != NULL) {
    free(buffer);
  }
  return result;
}

int string_builder_append_view(string_builder *sb, string_view view) {
  if (sb == NULL || view.data == NULL || view.length == 0) {
    return 1;
  }

  return dynamic_array_add_many(sb->string, (void**)view.data, view.length);
}

int string_builder_build(string_builder *sb, char **buffer) {
  if ((dynamic_array_shrink_to_fit(sb->string)) != 0) {
    return 1;
  }

  int bytes = sizeof(char) * dynamic_array_get_size(sb->string);

  if (((*buffer) = malloc(bytes + 1)) == NULL) {
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

int string_builder_destroy(string_builder **sb) {
  if (*sb == NULL) {
    return 1;
  }
  dynamic_array_destroy(&(*sb)->string);

  free(*sb);
  (*sb) = NULL;

  return 0;
}
