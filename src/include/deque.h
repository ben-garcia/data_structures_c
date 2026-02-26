#ifndef DEQUE_H
#define DEQUE_H

#include "arena.h"

typedef struct deque deque;

/**
 * @brief Allocate resource and setup
 *
 * @param d the deque to initialize
 * @param arena memory block used for allocations 
 * @return 0 on success, 1 otherwise
 */
int deque_create(deque **d, arena *arena);

/**
 * @brief Add 'data' at the front/head of the deque
 *
 * @param d the queue to modify
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int deque_add_front(deque *d, void *data);

/**
 * @brief Add 'data' at the back/tail of the deque
 *
 * @param d the queue to modify
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int deque_add_back(deque *d, void *data);

/**
 * @brief Remove from the front/head
 *
 * @param d the queue to modify
 * @return 0 on success, 1 otherwise
 */
int deque_remove_front(deque *d);

/**
 * @brief Remove from the back/tail
 *
 * @param d the queue to modify
 * @return 0 on success, 1 otherwise
 */
int deque_remove_back(deque *d);

/**
 * @brief Return the data at the front/head of the deque
 *
 * @param d the queue
 * @param data the buffer to store the data
 * @return 0 on success, 1 otherwise
 */
int deque_peek_front(deque *d, void **data);

/**
 * @brief Return the data at the back/tail of the deque
 *
 * @param d the queue
 * @param data the buffer to store the data
 * @return 0 on success, 1 otherwise
*/
int deque_peek_back(deque *d, void **data);

/**
 * @brief Return the size of the deque
 *
 * @param d the deque
 * @return the size of the queue, -1 otherwise
 */
int deque_size(deque *d);

/**
 * @brief Check if 'd' is empty
 *
 * @param d the deque to check
 * @return 0 indicates 'd' is empty, 1 otherwise
 */
int deque_is_empty(deque *d);

#endif // DEQUE_H
