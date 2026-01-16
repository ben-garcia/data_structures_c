#include "include/linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linked_list_node {
  struct linked_list_node *next;
  void *data;
} linked_list_node;

struct linked_list_iter {
  linked_list_node *next;
};

struct linked_list {
  linked_list_node *head;
  size_t data_size;
  void (*freefn)(void **);
  int (*matchfn)(void *, void *);
  unsigned int length;
};

int linked_list_create(linked_list **list, unsigned int data_size,
                       void (*freefn)(void **),
                       int (*matchfn)(void *, void *)) {
  if ((*list = malloc(sizeof(linked_list))) == NULL) {
    return 1;
  }

  (*list)->freefn = freefn;
  (*list)->matchfn = matchfn;
  (*list)->data_size = data_size;
  (*list)->head = NULL;
  (*list)->length = 0;

  return 0;
}

int linked_list_add(linked_list *list, void *data) {
  if (list == NULL) {
    return 1;
  }

  if (list->head == NULL) {
    if ((list->head = calloc(1, sizeof(linked_list_node))) == NULL) {
      return 1;
    }
    list->head->data = data;
    list->length = 1;

    return 0;
  }

  linked_list_node *current_head = list->head;
  linked_list_node *new_node;
  if ((new_node = malloc(sizeof(linked_list_node))) == NULL) {
    return 1;
  }

  new_node->next = current_head;
  new_node->data = data;

  list->head = new_node;
  list->length++;

  return 0;
}

int linked_list_find(linked_list *list, void *data, void **result) {
  linked_list_node *current_node = list->head;

  while (current_node != NULL) {
    if (list->matchfn != NULL) {
      if (list->matchfn(current_node->data, data)) {
        *result = current_node->data;
        return 0;
      }
    } else {
      if (current_node->data == data) {
        *result = current_node->data;
        return 0;
      }
    }

    current_node = current_node->next;
  }

  return 1;
}

int linked_list_remove(linked_list *list, void *data) {
  if (list == NULL || list->head == NULL || list->length == 0) {
    return 1;
  }

  linked_list_node *prev = NULL;
  linked_list_node *current = list->head;

  while (current != NULL) {
    linked_list_node *node = current;

    if (list->matchfn != NULL) {
      if (list->matchfn(node->data, data)) {
        if (prev == NULL) {
          // head of the list.
          list->head = current->next;
        } else if (current->next == NULL) {
          // at the end of the list.
          prev->next = NULL;
        } else {
          prev->next = current->next;
        }

        if (list->freefn != NULL) {
          list->freefn(node->data);
        }

        free(node);
        node = NULL;
        list->length--;
        return 0;
      }
    } else if (data == node->data) {
      if (prev == NULL) {
        // head of the list.
        list->head = current->next;
      } else if (current->next == NULL) {
        // at the end of the list.
        prev->next = NULL;
      } else {
        prev->next = current->next;
      }

      if (list->freefn != NULL) {
        list->freefn(node->data);
      }

      free(node);
      node = NULL;
      list->length--;
      return 0;
    }
    prev = current;
    current = current->next;
  }

  return 1;
}

/**
 * Reverse a linked list recursively
 */
// static linked_list_node *reverse(linked_list_node *head) {
//   if (head->next == NULL || head == NULL) { // base case of list with 1 node
//     return head;
//   }
//   linked_list_node *reversed_list = reverse(head->next);
//   head->next->next = head; // create link to the previous NULL pointer
//   head->next = NULL;       // break the link
//   return reversed_list;
// }

int linked_list_reverse(linked_list **list) {
  // (*list)->head = reverse((*list)->head);
  linked_list_node *previous = NULL;
  linked_list_node *current = (*list)->head;
  linked_list_node *next = NULL;

  while (current != NULL) {
    next = current->next;
    current->next = previous; // reverse the link
    previous = current;
    current = next;
  }

  (*list)->head = previous; // set the new head of the list
  return 0;
}

void *linked_list_get_node_data(linked_list_node *node) {
  if (node == NULL) {
    return NULL;
  }

  return node->data;
}

int linked_list_destroy(linked_list **list) {
  if (*list == NULL || (*list)->head == NULL) {
    return 1;
  }

  linked_list_node *current = (*list)->head;

  while (current != NULL) {
    linked_list_node *node = current;
    current = current->next;
    if ((*list)->freefn != NULL) {
      (*list)->freefn(&node->data);
    }
    free(node);
    node = NULL;
  }

  free(*list);
  *list = NULL;
  return 0;
}

int linked_list_iter_create(linked_list_iter **it, linked_list *list) {
  if (list->length == 0) {
    return 1;
  }

  if (((*it) = malloc(sizeof(linked_list_iter))) == NULL) {
    return 1;
  }

  (*it)->next = list->head;

  return 0;
}

int linked_list_iter_destroy(linked_list_iter **it) {
  if (*it == NULL) {
    return 1;
  }

  free(*it);
  *it = NULL;
  return 0;
}

int linked_list_iter_next(linked_list_iter *it, void **value) {
  if (it == NULL || it->next == NULL) {
    *value = NULL;
    return 1;
  }

  linked_list_node *current = it->next;

  it->next = current->next;
  *value = current->data;

  return 0;
}

int linked_list_iter_reset(linked_list_iter **it, linked_list *list) {
  if (list->length == 0) {
    return 1;
  }

  (*it)->next = list->head;

  return 0;
}
