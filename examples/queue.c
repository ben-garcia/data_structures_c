#include "queue.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  printf("=============queue====================\n");

  arena *arena;
  queue *queue;
  char *queue_data[] = {"q", "u", "e", "u", "e"};

  arena_create(&arena, KB(4));
  queue_create(&queue, arena);

  for (int i = 0; i < 5; i++) {
    printf("enqueing %s\n", queue_data[i]);
    queue_enqueue(queue, queue_data[i]);
  }

  printf("\nstack size: %d\n\n", queue_size(queue));

  while (queue_is_empty(queue) != 0) {
    void *data;
    queue_peek(queue, &data);
    printf("dequeing %c\n", *(char *)data);
    queue_dequeue(queue);
  }

  printf("\n");

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
