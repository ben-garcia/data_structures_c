#include "linked_list.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

#define LIST_LENGTH 10

int main(void) {
  printf("===========linked_list example=============\n");

  arena *arena;
  linked_list *floats;
  linked_list_iterator *float_it;
  float *fvalue;

  arena_create(&arena, KB(4));
  linked_list_create(&floats, NULL, arena);

  float *float_data =
      arena_alloc(arena, sizeof(double) * LIST_LENGTH, alignof(float), 0);

  float f;
  int i;
  for (i = 0, f = 0.314; f < 5; i++, f += 0.314) {
    float_data[i] = f;
    linked_list_add(floats, (void *)&float_data[i]);
  }

  printf("searching for %.3f, found 0(yes), 1(no): %d\n\n", float_data[3],
         linked_list_find(floats, float_data, (void **)&fvalue));

  linked_list_iterator_create(&float_it, floats);

  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%.3f -> ", *fvalue);
  }

  printf("NULL\n\n");

  printf("removing for %.3f, found 0(yes), 1(no): %d\n\n", float_data[3],
         linked_list_remove(floats, float_data));

  linked_list_reverse(&floats);

  linked_list_iterator_reset(&float_it, floats);

  printf("===========linked_list reversed==============\n");
  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%.3f -> ", *fvalue);
  }

  printf("NULL\n");

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
