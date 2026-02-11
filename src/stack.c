#include "include/stack.h"
#include <stdlib.h>

typedef struct stack_node {
  struct stack_node *next;
  void *data;
} stack_node;

struct stack {
  stack_node *head; // linked list head
  unsigned int size;
};

int stack_create(stack **s) {
  if ((*s = malloc(sizeof(stack))) == NULL) {
    return 1;
  }

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
static stack_node *stack_node_create(void *data) {
  stack_node *node;
  if ((node = malloc(sizeof(stack_node))) == NULL) {
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
    s->head = stack_node_create(data);
    s->size++; // increment size
    return 0;
  }

  stack_node *temp = s->head;
  stack_node *node = stack_node_create(data);

  node->next = temp;
  s->head = node;
  s->size++; // increment size

  return 0;
}

void *stack_pop(stack *s) {
  if (s == NULL || s->head == NULL) {
    return NULL; // s must be defined and size > 0
  }

  stack_node *node = s->head;

  if (s->size == 1) {
    void *temp = node->data;
    s->head = NULL;
    s->size--;
    free(node);
    return temp;
  }

  void *temp = node->data;
  s->head = node->next;
  s->size--;
  free(node);

  return temp;
}

int stack_get_size(stack *s) { return s->size; }

int stack_is_empty(stack *s) { return !(s->size == 0); }

void *stack_peek(stack *s) { return s->head->data; }

int stack_destroy(stack **s) {
  if (*s == NULL) { // nothing to do 
    return 1;
  } else if ((*s)->head == NULL) { // size > 0, do nothing
    free(*s);
    *s = NULL;
    return 1;
  }

  stack_node *current = (*s)->head;
  while (current != NULL) {
    stack_node *node = current;
    current = current->next;
    free(node);
  }

  free(*s);
  *s = NULL;

  return 0;
}
