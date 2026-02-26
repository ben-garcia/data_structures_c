#include "include/deque.h"
#include <stdalign.h>

#define FALSE 0

typedef struct deque_node {
  struct deque_node *next;
  struct deque_node *previous;
  void *data; // data at each node
} deque_node;

struct deque {
  deque_node *head;  // the front of the deque
  deque_node *tail;  // the back of the deque
  arena *arena;      // memory block for allocations
  unsigned int size; // number of nodes
};

int deque_create(deque **d, arena *arena) {
  if (((*d) = arena_alloc(arena, sizeof(deque), alignof(deque), FALSE)) ==
      NULL) {
    return 1;
  }

  (*d)->arena = arena;
  (*d)->head = NULL;
  (*d)->tail = NULL;
  (*d)->size = 0;

  return 0;
}

/**
 * Create a deque node, allocating resources
 */
static deque_node *deque_node_create(arena *arena, deque_node *next,
                                     deque_node *previous, void *data) {
  deque_node *node;
  if ((node = arena_alloc(arena, sizeof(deque_node), alignof(deque_node),
                          FALSE)) == NULL) {
    return NULL;
  }

  node->next = next;
  node->previous = previous;
  node->data = data;

  return node;
}

int deque_add_front(deque *d, void *data) {
  if (d == NULL) {
    return 1;
  }

  if (d->size == 0) {
    if ((d->head = deque_node_create(d->arena, NULL, NULL, data)) == NULL) {
      return 1;
    }

    d->tail = d->head;
    d->size++;

    return 0;
  }

  deque_node *node;
  if ((node = deque_node_create(d->arena, d->head, NULL, data)) == NULL) {
    return 1;
  }

  d->head = node;
  d->size++;

  return 0;
}

int deque_add_back(deque *d, void *data) {
  if (d == NULL) {
    return 1;
  }

  if (d->size == 0) {
    if ((d->tail = deque_node_create(d->arena, NULL, NULL, data)) == NULL) {
      return 1;
    }

    d->head = d->tail;
    d->size++;

    return 0;
  }

  deque_node *node;
  if ((node = deque_node_create(d->arena, NULL, d->tail, data)) == NULL) {
    return 1;
  }

  d->tail = node;
  d->size++;

  return 0;
}

int deque_remove_front(deque *d) {
  if (d == NULL || d->size == 0) {
    return 1;
  }

  if (d->size == 1) {
    d->head = NULL;
    d->tail = NULL;
    d->size--;
    return 0;
  }

  d->head = d->head->next;
  d->size--;

  return 0;
}

int deque_remove_back(deque *d) {
  if (d == NULL || d->size == 0) {
    return 1;
  }

  if (d->size == 1) {
    d->tail = NULL;
    d->head = NULL;
    d->size--;
    return 0;
  }

  d->tail = d->tail->previous;
  d->size--;

  return 0;
}

int deque_peek_front(deque *d, void **data) {
  if (d == NULL || d->size == 0) {
    return 1;
  }

  *data = d->head->data;

  return 0;
}

int deque_peek_back(deque *d, void **data) {
  if (d == NULL || d->size == 0) {
    return 1;
  }

  *data = d->tail->data;

  return 0;
}

int deque_size(deque *d) {
  if (d == NULL) {
    return -1;
  }

  return d->size;
}

int deque_is_empty(deque *d) {
  if (d == NULL) {
    return 1;
  }

  return !(d->size == 0);
}
