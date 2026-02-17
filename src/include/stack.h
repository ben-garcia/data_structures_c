#ifndef STACK_H
#define STACK_H

typedef struct stack stack;

/**
 * @brief Allocate resource and setup 
 *
 * @param s the stack to initialize
 * @param freefn deallocation fuction for custom data types
 * @return 0 on success, 1 otherwise
 */
int stack_create(stack **s, void (*freefn)(void *data));

/**
 * @brief Insert 'data' on to the stack 
 *
 * @param s the stack to modify 
 * @param data the data to add
 * @return 0 on success, 1 otherwise
 */
int stack_push(stack *s, void *data);

/**
 * @brief Remove the item from the top of the stack
 *
 * @param s the stack to modify 
 * @return 0 on success, 1 otherwise
 */
int stack_pop(stack *s);

/**
 * @brief Return the size of the stack 
 *
 * @param s the stack to initialize
 * @return the size of the stack
 */
int stack_get_size(stack *s);

/**
 * @brief Check if 's' is empty
 *
 * @param s the stack to check
 * @return 0 indicates 's' is empty, 1 otherwise
 */
int stack_is_empty(stack *s);

/**
 * @brief Return the data of the top of the stack 
 *
 * @param s the stack 
 * @param data the 'data' associated with the top of the stack 
 * @return 0 on success, 1 otherwise
 */
int stack_peek(stack *s, void **data);

/**
 * @brief Deallocate all resources 
 *
 * @param s the stack to deallocate
 * @return 0 on success, 1 otherwise
 */
int stack_destroy(stack **s);

#endif // STACK_H
