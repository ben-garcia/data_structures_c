#include "string_view.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  printf("=============string view example============\n");

  arena *arena;

  string_view *view;
  string_view *view2;

  char *view_buffer;
  char *view2_buffer;

  arena_create(&arena, KB(4));

  string_view_create(&view, " this:is:a:test", arena);
  string_view_create(&view2, "", arena);

  printf("trimming view that contains: '%s'\n", string_view_data(view));
  string_view_trim(view);

  printf("spliting view by ':'\n\n");
  while (string_view_split(view, view2, ':') == 0) {
    printf("does '%s', start with 't': 0(yes), 1(no): %d\n",
           string_view_data(view), string_view_starts_with(view, "t"));
    printf("does '%s', end with with 's': 0(yes), 1(no): %d\n",
           string_view_data(view), string_view_starts_with(view, "s"));
    printf("view size is %d\n\n", string_view_size(view));
  }

  printf("comparing '%s' and '%s': 0(yes), 1(no): %d\n", string_view_data(view),
         string_view_data(view2), string_view_compare(view, view2));

  string_view_to_string(view, &view_buffer);
  string_view_to_string(view2, &view2_buffer);

  printf("view_buffer: %s\n", view_buffer);
  printf("view_buffer2: %s\n", view2_buffer);

  // de-allocate
  arena_destroy(&arena);
  return 0;
}
