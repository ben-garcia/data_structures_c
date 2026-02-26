#include "include/stack.h"
#include <stdalign.h>

#define FALSE 0

typedef struct stack_node {
  struct stack_node *next;
  void *data;
} stack_node;

struct stack {
  stack_node *head; // linked list head
  arena *arena;     // memory block for allocations
  unsigned int size;
};

int stack_create(stack **s, arena *arena) {
  if ((*s = arena_alloc(arena, sizeof(stack), alignof(stack), FALSE)) == NULL) {
    return 1;
  }

  (*s)->arena = arena;
  (*s)->head = NULL;
  (*s)->size = 0;

  return 0;
}

/**
 * @brief Allocate resouece for stack node and setup
 *
 * @param data the node data
 * @return the newly created stack node
 */
static stack_node *stack_node_create(arena *arena, void *data) {
  stack_node *node;
  if ((node = arena_alloc(arena, sizeof(stack_node), alignof(stack_node),
                          FALSE)) == NULL) {
    return NULL;
  }

  node->data = data;
  node->next = NULL;

  return node;
}

int stack_push(stack *s, void *data) {
  if (s == NULL) { // s must be defined
    return 1;
  }

  if (s->head == NULL) { // size == 0
    s->head = stack_node_create(s->arena, data);
    s->size++; // increment size
    return 0;
  }

  stack_node *temp = s->head;
  stack_node *node = stack_node_create(s->arena, data);

  node->next = temp;
  s->head = node;
  s->size++; // increment size

  return 0;
}

int stack_pop(stack *s) {
  if (s == NULL || s->head == NULL) {
    return 1; // s must be defined and size > 0
  }

  stack_node *node = s->head;
  s->head = node->next;
  s->size--;

  return 0;
}

int stack_size(stack *s) { return s->size; }

int stack_is_empty(stack *s) { return !(s->size == 0); }

int stack_peek(stack *s, void **data) {
  if (s == NULL || s->size == 0) {
    return 1;
  }

  *data = s->head->data;

  return 0;
}
