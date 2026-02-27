#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "arena.h"

typedef struct linked_list linked_list;
typedef struct linked_list_iterator linked_list_iterator;

/*
 * @brief Allocate resource and setup 'list'
 *
 * @param list linked list to initialize
 * @param matchfn comparison function used for 'linked_list_find'
 *        MUST return 0 if a == b,
 *             negative number if a < b
 *             positive number if a > b,
 * @param arena block of memory to use for allocations and dealcations
 * @return 0 on success, 1 otherwise
 */
int linked_list_create(linked_list **list, int (*matchfn)(void *a, void *b),
                       arena *arena);

/* @brief Insert 'data' into the 'list'
 *
 * @param list linked list to modify
 * @param data the data to insert
 * @return 0 on success, 1 otherwise
 */
int linked_list_add(linked_list *list, void *data);

/* @brief Search 'data' from the 'list' and store in 'result'
 *
 * @note uses matchfn to locate 'data'
 *
 * @param list linked list to modify
 * @param data the data to search
 * @param result where the element(NULL if not found
  @return 0 on success, 1 otherwise
 */
int linked_list_find(linked_list *list, void *data, void **result);

/* @brief Remove 'data' from the 'list'
 *
 * @note uses matchfn to locate 'data'
 *
 * @param list linked list to modify
 * @param data the data to remove
 * @return 0 on success, 1 otherwise
 */
int linked_list_remove(linked_list *list, void *data);

/* @brief Reverse the 'list'
 *
 * Useful to iterate through in the order items were added
 *
 * @param list linked list to modify
 * @return number of elements, -1 otherwise 
 */
int linked_list_reverse(linked_list **list);

/**
 * @brief Retrive the number of nodes in 'list'.
 *
 * @param list the linked list to check.
 * @return 0 on success, 1 otherwise
 */
int linked_list_size(linked_list *list);

/**
 * @brief Allocate necessary resources and setup.
 *
 * Use to iterate through a linked list.
 *
 * @param it linked list iterator to create.
 * @param list linked list to iterate through.
 * @return 0 on success, 1 otherwise
 */
int linked_list_iterator_create(linked_list_iterator **it, linked_list *list);

/**
 * @brief Get the next element and store it in 'value'
 *
 * @param it dynamic array iterator
 * @param value used to hold the next element in the list.
 * @return 0 on success, 1 otherwise
 */
int linked_list_iterator_next(linked_list_iterator *it, void **value);

/**
 * @brief Reset the linked list iterator.
 *
 * @param it linked list iterator to modify
 * @param list the linked list to iterate
 * @return 0 on success, 1 otherwise
 */
int linked_list_iterator_reset(linked_list_iterator **it, linked_list *list);

#endif // LINKED_LIST_H
