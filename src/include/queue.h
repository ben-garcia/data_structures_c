#ifndef QUEUE_H
#define QUEUE_H

#include "arena.h"

typedef struct queue queue;

/**
 * @brief Allocate resource and setup
 *
 * @param q the queue to initialize
 * @param arena memory block for allocations
 * @return 0 on success, 1 otherwise
 */
int queue_create(queue **q, arena *arena);

/**
 * @brief Insert 'data' in end/tail of the queue
 *
 * @param q the queue to modify
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int queue_enqueue(queue *q, void *data);

/**
 * @brief Remove 'data' in front/head
 *
 * @param q the queue to modify
 * @return 0 on success, 1 otherwise
 */
int queue_dequeue(queue *q);

/**
 * @brief Return the data at the front/head of the queue
 *
 * @param q the queue
 * @param data the buffer to store the data at the front/head of the queue
 * @return 0 on success, 1 otherwise
 */
int queue_peek(queue *q, void **data);

/**
 * @brief Return the size of the queue
 *
 * @param q the queue
 * @return the size of the queue, -1 otherwise
 */
int queue_get_size(queue *q);

/**
 * @brief Check if 'q' is empty
 *
 * @param q the queue to check
 * @return 0 indicates 'q' is empty, 1 otherwise
 */
int queue_is_empty(queue *q);

#endif // QUEUE_H
