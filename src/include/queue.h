#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue queue;

/**
 * @brief Allocate resource and setup 
 *
 * @param q the queue to initialize
 * @return 0 on success, 1 otherwise
 */
int queue_create(queue **q);

/**
 * @brief Insert 'data' in end/tail of the queue 
 *
 * @param q the queue to modify 
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int queue_enqueue(queue *q, void *data);

/**
 * @brief Remove 'data' in front/head without modifying the queue 
 *
 * @param q the queue to modify 
 * @return 0 on success, 1 otherwise
 */
void *queue_dequeue(queue *q);

/**
 * @brief Return the data of the front/head of the queue 
 *
 * @param q the queue 
 * @return the data at that node 
 */
void *queue_peek_front(queue *q);

/**
 * @brief Return the data of the back/tail wihtout modifying the queue
 *
 * @param q the queue 
 * @return the data at that node 
 */
void *queue_peek_back(queue *q);

/**
 * @brief Return the size of the queue 
 *
 * @param q the queue
 * @return the size of the queue 
 */
int queue_get_size(queue *q);

/**
 * @brief Check if 'q' is empty
 *
 * @param q the queue to check
 * @return 0 indicates 'q' is empty, 1 otherwise
 */
int queue_is_empty(queue *q);

/**
 * @brief Deallocate all resources 
 *
 * @param q the queue to deallocate
 * @return 0 on success, 1 otherwise
 */
int queue_destroy(queue **q);

#endif // QUEUE_H

