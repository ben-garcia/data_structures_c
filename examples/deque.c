#include "deque.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  arena *arena;
  arena_create(&arena, KB(4));

  printf("==============deque================\n");
  char *deque_data[] = {"e", "u", "q", "e", "d", "e", "u", "q", "e", "d"};
  deque *deque;
  deque_create(&deque, arena);

  printf("deque is empty 0(yes), 1(no): %d\n\n", deque_is_empty(deque));

  printf("adding to the back... ");
  for (int i = 0; i < 5; i++) {
    printf("%s ", deque_data[i]);
    deque_add_back(deque, deque_data[i]);
  }
  printf("\ndeque size: %d\n\n", deque_size(deque));

  printf("adding to the front... ");
  for (int i = 5; i < 10; i++) {
    printf("%s ", deque_data[i]);
    deque_add_front(deque, deque_data[i]);
  }

  printf("\ndeque size: %d\n\n", deque_size(deque));

  printf("removing from the front... ");
  for (int i = 0; i < 5; i++) {
    char *data;
    deque_peek_front(deque, (void **)&data);
    printf("%s ", data);
    deque_remove_front(deque);
  }
  
  printf("\ndeque size: %d\n\n", deque_size(deque));

  printf("removing from the back... ");
  for (int i = 5; i < 10; i++) {
    char *data;
    deque_peek_back(deque, (void **)&data);
    printf("%s ", data);
    deque_remove_back(deque);
  }

  printf("\ndeque size: %d\n", deque_size(deque));

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
