#include "dynamic_array.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

#define ARRAY_LENGTH 30
#define ZERO_OUT_FALSE 0

int matchfn(void *a, void *b) { return !(*(long *)a == *(long *)b); }

int main(void) {
  printf("===========dynamic_array example==============\n");

  arena *arena;
  dynamic_array *numbers;
  dynamic_array_iterator *iterator;
  long value = 10;

  arena_create(&arena, KB(4));
  dynamic_array_create(&numbers, 32, sizeof(long), matchfn, arena);

  printf("is array empty? 0(yes), 1(no): %d\n\n",
         dynamic_array_is_empty(numbers));

  // add most of 'array_data' to the array
  for (long i = 0; i < ARRAY_LENGTH; i++) {
    dynamic_array_add(numbers, &i);
  }

  printf("array size after insertions: %d\n", dynamic_array_size(numbers));
  dynamic_array_iterator_create(&iterator, numbers); // create iterator

  printf("dynamic array contains: ");
  // iterate through the array
  while ((dynamic_array_iterator_next(iterator, &value)) == 0) {
    printf("%ld ", value);
  }

  dynamic_array_find_by_index(numbers, 14, &value);
  printf("\n\nremoving item at index: %ld\n", value);
  dynamic_array_remove_by_index(numbers, value);

  long l = 20;

  dynamic_array_find(numbers, &l, &value);
  printf("removing data: %ld\n", value);
  dynamic_array_remove(numbers, &value);

  printf("\narray size after deletions: %d\n", dynamic_array_size(numbers));

  dynamic_array_iterator_reset(iterator);

  printf("dynamic array contains: ");
  // iterate through the array
  while ((dynamic_array_iterator_next(iterator, (void **)&value)) == 0) {
    printf("%ld ", value);
  }

  printf("\n");

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
