#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list linked_list;
typedef struct linked_list_iterator linked_list_iterator;

int linked_list_create(linked_list **list, void (*freefn)(void **),
                       int (*matchfn)(void *, void *));
int linked_list_add(linked_list *list, void *data);
int linked_list_find(linked_list *list, void *data, void **result);
int linked_list_remove(linked_list *list, void *data);
int linked_list_reverse(linked_list **list);
int linked_list_destroy(linked_list **list);

int linked_list_iterator_create(linked_list_iterator **it, linked_list *list);
int linked_list_iterator_next(linked_list_iterator *it, void **value);
int linked_list_iterator_reset(linked_list_iterator **it, linked_list *list);
int linked_list_iterator_destroy(linked_list_iterator **it);

#endif // LINKED_LIST_H
