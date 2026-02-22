#include "include/queue.h"
#include <stdalign.h>

#define FALSE 0

typedef struct queue_node {
  struct queue_node *next;
  void *data;
} queue_node;

struct queue {
  queue_node *head;  // front of the queue
  queue_node *tail;  // end of the queue
  arena *arena;      // memory block for allocations
  unsigned int size; // number of nodes
};

int queue_create(queue **q, arena *arena) {
  if (((*q) = arena_alloc(arena, sizeof(queue), alignof(queue), FALSE)) ==
      NULL) {
    return 1;
  }

  (*q)->arena = arena;
  (*q)->head = NULL;
  (*q)->tail = NULL;
  (*q)->size = 0;

  return 0;
}

/**
 * @brief Allocate resouece for queue node and setup
 *
 * @param arena memory block used for allocation
 * @param data the node data
 * @return the newly created queue node
 */
static queue_node *queue_node_create(arena *arena, void *data) {
  queue_node *node;
  if ((node = arena_alloc(arena, sizeof(queue_node), alignof(queue_node),
                          FALSE)) == NULL) {
    return NULL;
  }

  node->data = data;
  node->next = NULL;

  return node;
}

int queue_enqueue(queue *q, void *data) {
  if (q == NULL) {
    return 1;
  }

  if (q->size == 0) {
    q->head = queue_node_create(q->arena, data);
    q->tail = q->head;
    q->size++;
    return 0;
  }

  queue_node *temp = q->tail;
  queue_node *node = queue_node_create(q->arena, data);

  temp->next = node;
  q->tail = node;
  q->size++;

  return 0;
}

int queue_dequeue(queue *q) {
  if (q == NULL || q->head == NULL) {
    return 1;
  }

  q->head = q->head->next;
  q->size--;

  return 0;
}

int queue_is_empty(queue *q) {
  if (q == NULL) {
    return 1;
  }

  return !(q->size == 0);
}

int queue_peek(queue *q, void **data) {
  if (q->size == 0) {
    return 1;
  }

  *data = q->head->data;

  return 0;
}

int queue_get_size(queue *q) {
  if (q == NULL) {
    return -1;
  }

  return q->size;
}
