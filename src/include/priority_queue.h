#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "arena.h"

typedef struct priority_queue priority_queue;

/*
 * @brief Allocate resource and setup 'pq' priority queue
 *
 * @param pq priority queue to initialize 
 * @param initial_capacity total number of items before resizing 
 * @param comparefn comparison function
 *        MUST return 0 if a == b,
 *             negative number if a < b
 *             positive number if a > b,
 * @param freefn deallocate function
 * @param arena block of memory to use for allocations and dealcations
 * @return 0 on success, 1 otherwise
 */
int priority_queue_create(priority_queue **pq, unsigned int initial_capacity,
                          int (*comparefn)(const void *a, const void *b),
                          void (*freefn)(void *data), arena *arena);
/**
 * @brief Insert 'data' into the priority queue
 *
 * @param pq the priority_queue to modify 
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int priority_queue_insert(priority_queue *pq, void *data);

/**
 * @brief Return the data at the root of the priority queue 
 *
 * @param pq the priority_queue to check 
 * @param data buffer to store root data
 * @return 0 on success, 1 otherwise 
 */
int priority_queue_peek(priority_queue *pq, void **data);

/**
 * @brief Delete the root node data
 *
 * @param pq the priority_queue to modify 
 * @return 0 on success, 1 otherwise
 */
int priority_queue_delete(priority_queue *pq);

/**
 * @brief Check if 'pq' is empty
 *
 * @param pq the priority_queue to modify 
 * @return 0 indicates 'pq' is empty, 1 otherwise
 */
int priority_queue_is_empty(priority_queue *pq);

/**
 * @brief Deallocate all resources 
 *
 * @param pq the priority_queue to modify 
 * @return 0 on success, 1 otherwise
 */
int priority_queue_destroy(priority_queue **pq);

#endif // PRIORITY_QUEUE_H
