#include "string_builder.h"
#include "string_view.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  printf("=============string builder example================\n");

  arena *arena;
  string_builder *sb;
  string_view *str_view;
  char *buffer;
  
  arena_create(&arena, KB(4));

  string_builder_create(&sb, 32, arena);
  string_view_create(&str_view, " and Hello", arena);

  printf("appending 'Hello World!\n");
  string_builder_append(sb, "Hello World!");
  printf("size is: %d\n", string_builder_size(sb));

  printf("appending string view: '%s'\n", string_view_data(str_view));
  string_builder_append_view(sb, str_view);
  printf("size is: %d\n", string_builder_size(sb));

  printf("appending printf: ' C!\n");
  string_builder_append_printf(sb, "%s", " C!");
  printf("size is: %d\n", string_builder_size(sb));

  string_builder_build(sb, &buffer);

  printf("string is: '%s'\n", buffer);

  arena_destroy(&arena);

  return 0;
}
