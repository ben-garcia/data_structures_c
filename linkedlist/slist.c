#include "slist.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "person.h"

struct slist_node {
  struct slist_node *next;
  void *data;
};

struct slist_iter {
  struct slist_node *next;
};

struct slist {
  struct slist_node *head;
  size_t data_size;
  void (*freefn)(void *);
  bool (*matchfn)(void *, void *);
  unsigned int length;
};

slist *slist_create(size_t data_size) {
  slist *list;
  if ((list = malloc(sizeof(*list))) == NULL) {
    return NULL;
  }

  list->freefn = NULL;
  list->matchfn = NULL;
  list->data_size = data_size;
  list->head = NULL;
  list->length = 0;

  return list;
}

slist *slist_create_from_file(const char *filename, size_t data_size,
                              void (*freefn)(void *)) {
  slist *list;

  if ((list = malloc(sizeof(*list))) == NULL) {
    return NULL;
  }

  slist_set_free_function(list, freefn);

  list->data_size = data_size;
  list->head = NULL;
  list->length = 0;

  char *contents = read_file(filename);
  char *copy = strtok(contents, "\n");

  while (copy != NULL) {
    char name[50];
    int age;
    // get up to 50 characters
    sscanf(copy, "%49[^,],%d\n", name, &age);
    slist_add(list, person_create(name, age));
    copy = strtok(NULL, "\n");
  }

  free(contents);

  contents = NULL;

  return list;
}

void slist_set_free_function(slist *list, void (*freefn)(void *)) {
  list->freefn = freefn;
}

void slist_set_match_function(slist *list, bool (*matchfn)(void *, void *)) {
  list->matchfn = matchfn;
}

slist_node *slist_add(slist *list, void *data) {
  if (list == NULL) {
    return false;
  }

  if (list->head == NULL) {
    if ((list->head = calloc(1, sizeof(slist_node))) == NULL) {
      perror("list_push: calloc failure");
      return false;
    }
    list->head->data = data;
    list->length = 1;
    return list->head;
  }

  slist_node *current_head = list->head;
  slist_node *new_node;
  if ((new_node = malloc(sizeof(slist_node))) == NULL) { // 16 bytes
    perror("list_push: malloc failure");
    return false;
  }

  new_node->next = current_head;
  new_node->data = data;

  list->head = new_node;
  list->length++;

  return new_node;
}

void *slist_find(slist *list, void *data) {
  slist_node *current_node = list->head;

  while (current_node != NULL) {
    if (list->matchfn != NULL) {
      if (list->matchfn(current_node->data, data)) {
        return current_node->data;
      }
    } else {
      if (current_node->data == data) {
        return current_node->data;
      }
    }

    current_node = current_node->next;
  }

  return NULL;
}

bool slist_remove(slist *list, void *data) {
  if (list == NULL || list->head == NULL || list->length == 0) {
    return false;
  }

  slist_node *prev = NULL;
  slist_node *current = list->head;

  while (current != NULL) {
    slist_node *node = current;

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
        return true;
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
      return true;
    }
    prev = current;
    current = current->next;
  }

  return false;
}

void *slist_get_node_data(slist_node *node) {
  if (node == NULL) {
    return NULL;
  }

  return node->data;
}

void slist_destroy(slist *list) {
  slist_node *current = list->head;

  while (current != NULL) {
    slist_node *node = current;
    current = current->next;
    if (list->freefn != NULL) {
      list->freefn(node->data);
    }
    free(node);
    node = NULL;
  }

  free(list);
  list = NULL;
}

slist_iter *slist_iter_create(slist *list) {
  if (list->length == 0) {
    return NULL;
  }

  slist_iter *it;

  if ((it = malloc(sizeof(*it))) == NULL) {
    return NULL;
  }

  it->next = list->head;

  return it;
}

void slist_iter_destroy(slist_iter *it) {
  if (it == NULL) {
    return;
  }

  free(it);
  it = NULL;
}

slist_node *slist_iter_next(slist_iter *it) {
  if (it == NULL) {
    return NULL;
  }

  slist_node *current = it->next;

  if (current != NULL) {
    it->next = current->next;
  }

  return current;
}

void slist_iter_reset(slist *list, slist_iter *it) {
  if (list->length == 0 || it == NULL) {
    return;
  }

  it->next = list->head;
}
