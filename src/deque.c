#include "include/deque.h"
#include <stdlib.h>

typedef struct deque_node {
  struct deque_node *next;
  struct deque_node *previous;
  void *data; // data at each node
} deque_node;

struct deque {
  deque_node *head;           // the front of the deque
  deque_node *tail;           // the back of the deque
  void (*freefn)(void *data); // deallocation function for custom data types
  unsigned int size;          // number of nodes
};

int deque_create(deque **d, void (*freefn)(void *data)) {
  if (((*d) = malloc(sizeof(deque))) == NULL) {
    return 1;
  }

  (*d)->freefn = freefn;
  (*d)->head = NULL;
  (*d)->tail = NULL;
  (*d)->size = 0;

  return 0;
}

/**
 * Create a deque node, allocating resources
 */
static deque_node *deque_node_create(deque_node *next, deque_node *previous,
                                     void *data) {
  deque_node *node;
  if ((node = malloc(sizeof(deque_node))) == NULL) {
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
    if ((d->head = deque_node_create(NULL, NULL, data)) == NULL) {
      return 1;
    }

    d->tail = d->head;
    d->size++;

    return 0;
  }

  deque_node *node;
  if ((node = deque_node_create(d->head, NULL, data)) == NULL) {
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
    if ((d->tail = deque_node_create(NULL, NULL, data)) == NULL) {
      return 1;
    }

    d->head = d->tail;
    d->size++;

    return 0;
  }

  deque_node *node;
  if ((node = deque_node_create(NULL, d->tail, data)) == NULL) {
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
    if (d->freefn != NULL) {
      d->freefn(d->head->data);
    }
    free(d->head);
    d->head = NULL;
    d->tail = NULL;
    d->size--;
    return 0;
  }

  deque_node *node = d->head;
  d->head = d->head->next;
  if (d->freefn != NULL) {
    d->freefn(node->data);
  }

  free(node);
  d->size--;

  return 0;
}

int deque_remove_back(deque *d) {
  if (d == NULL || d->size == 0) {
    return 1;
  }

  if (d->size == 1) {
    if (d->freefn != NULL) {
      d->freefn(d->tail->data);
    }

    free(d->tail);
    d->tail = NULL;
    d->head = NULL;
    d->size--;
    return 0;
  }

  deque_node *node = d->tail;
  d->tail = d->tail->previous;

  if (d->freefn != NULL) {
    d->freefn(node->data);
  }

  free(node);
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

int deque_get_size(deque *d) {
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

int deque_destroy(deque **d) {
  if (*d == NULL) {
    return 1;
  }

  deque_node *current = (*d)->head;

  while (current != NULL) {
    deque_node *node = current;
    if ((*d)->freefn != NULL) {
      (*d)->freefn(node->data);
    }

    free(node);
    current = current->next;
  }

  free(*d);
  *d = NULL;

  return 0;
}
