#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list linked_list;
typedef struct linked_list_iter linked_list_iter;

int linked_list_create(linked_list **list, unsigned int data_size,
                       void (*freefn)(void **), int (*matchfn)(void *, void *));
int linked_list_add(linked_list *list, void *data);
int linked_list_find(linked_list *list, void *data, void **result);
int linked_list_remove(linked_list *list, void *data);
int linked_list_reverse(linked_list **list);
int linked_list_destroy(linked_list **list);

int linked_list_iter_create(linked_list_iter **it, linked_list *list);
int linked_list_iter_next(linked_list_iter *it, void **value);
int linked_list_iter_reset(linked_list_iter **it, linked_list *list);
int linked_list_iter_destroy(linked_list_iter **it);

#endif // LINKED_LIST_H
