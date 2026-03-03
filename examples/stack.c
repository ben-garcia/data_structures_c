#include "stack.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  printf("=============stack example============\n");

  arena *arean;
  arena_create(&arean, KB(4));

  char *stack_data[] = {"s", "t", "a", "c", "k"};
  stack *stack;

  stack_create(&stack, arean);

  for (int i = 0; i < 5; i++) {
    printf("pushing %s\n", stack_data[i]);
    stack_push(stack, stack_data[i]);
  }

  printf("\nstack size: %d\n\n", stack_size(stack));

  while (stack_is_empty(stack) != 0) {
    void *data;
    stack_peek(stack, &data);
    stack_pop(stack);
    printf("poping %s\n", (char *)data);
  }

  printf("\n");

  // de-allocate
  arena_destroy(&arean);

  return 0;
}
