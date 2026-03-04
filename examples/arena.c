#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

#define ARRAY_LENGTH 10
#define NEW_ARRAY_LENGTH 20

int main(void) {
  printf("=============arena example============\n");

  arena *arena;
  int *p;

  arena_create(&arena, KB(4));

  p = arena_alloc(arena, sizeof(int), alignof(int), 0);
  *p = 10;

  printf("--starting of scratch arena--\n");
  arena_start_scratch_arena(arena);

  int *array = arena_alloc(arena, sizeof(int) * ARRAY_LENGTH, alignof(int), 0);
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    array[i] = i;
  }

  printf("array contains: ");
  for (int i = 0; i < ARRAY_LENGTH; i++) {
    printf(" %d", array[i]);
  }

  printf("\nreallocating...\n");

  array = arena_realloc(arena, array, ARRAY_LENGTH * sizeof(int),
                        NEW_ARRAY_LENGTH * sizeof(int), alignof(void *), 0);
  for (int i = ARRAY_LENGTH; i < NEW_ARRAY_LENGTH; i++) {
    array[i] = i;
  }

  printf("array contains: ");
  for (int i = 0; i < NEW_ARRAY_LENGTH; i++) {
    printf(" %d", array[i]);
  }

  printf("\n");

  arena_end_scratch_arena(arena);
  printf("--ending of scratch arena--\n");

  arena_reset(arena);

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
