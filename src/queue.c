#include "include/queue.h"
#include <stdlib.h>

typedef struct queue_node {
  struct queue_node *next;
  void *data;
} queue_node;

struct queue {
  queue_node *head;           // front of the queue
  queue_node *tail;           // end of the queue
  void (*freefn)(void *data); // deallocation function for custom data types
  unsigned int size;
};

int queue_create(queue **q, void (*freefn)(void *data)) {
  if (((*q) = malloc(sizeof(queue))) == NULL) {
    return 1;
  }

  (*q)->freefn = freefn;
  (*q)->head = NULL;
  (*q)->tail = NULL;
  (*q)->size = 0;

  return 0;
}

/**
 * @brief Allocate resouece for queue node and setup
 *
 * @param data the node data
 * @return the newly created queue node
 */
static queue_node *queue_node_create(void *data) {
  queue_node *node;
  if ((node = malloc(sizeof(queue))) == NULL) {
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
    q->head = queue_node_create(data);
    q->tail = q->head;
    q->size++;
    return 0;
  }

  queue_node *temp = q->tail;
  queue_node *node = queue_node_create(data);

  temp->next = node;
  q->tail = node;
  q->size++;

  return 0;
}

int queue_dequeue(queue *q) {
  if (q == NULL || q->head == NULL) {
    return 1;
  }

  queue_node *temp = q->head;

  q->head = q->head->next;

  if (q->freefn != NULL) {
    q->freefn(temp->data);
  }

  free(temp);
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

int queue_destroy(queue **q) {
  if ((*q) == NULL) { // q must be defined
    return 1;
  }

  queue_node *current = (*q)->head;

  while (current != NULL) {
    queue_node *temp = current;

    current = current->next;
    if ((*q)->freefn != NULL) {
      (*q)->freefn(temp->data);
    }

    free(temp);
  }

  free(*q);
  *q = NULL;

  return 0;
}
