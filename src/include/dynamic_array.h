#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef struct dynamic_array dynamic_array;
typedef struct dynamic_array_iterator dynamic_array_iterator;

/**
 * @brief Allocate necessary resources and setup.
 *
 * @param array dynamic_array to create.
 * @param data_size Size in bytes of each value stored.
 * @param freefn Function used to deallocate user defined structure.
 * @param matchfn Function used to find a user defined struture.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_create(dynamic_array **array, unsigned int data_size,
                         void (*freefn)(void **),
                         int (*matchfn)(void *, void *));

/**
 * Add a new element to the array.
 *
 * @param array dynamic_array to modify.
 * @param item the element to add.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_add(dynamic_array *array, const void *item);

/**
 * Add a multiple elements to the array.
 *
 * @param array dynamic_array to modify.
 * @param items the elements to add.
 * @param length number of elements to add.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_add_many(dynamic_array *array, void **items,
                           unsigned int length);

/**
 * Get the value at a given index.
 *
 * @param array dynamic_array to modify.
 * @param index Index to access.
 * @param item Where to store the value.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_find(dynamic_array *array, unsigned int index, void **item);

/**
 * Get a reference to the element at a given index.
 *
 * @param array dynamic_array to modify.
 * @param index Index to access.
 * @param item Where to store the value.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_find_ref(dynamic_array *array, unsigned int index,
                           void **item);

/**
 * @brief Retrive the number of elements in the array.
 *
 * @param array the array to access.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_get_size(dynamic_array *array);

/**
 * @brief Check if array is empty.
 *
 * @param array dynamic_array to check.
 * @return 0 on success, 1 otherwise.
 */
int dynamic_array_is_empty(dynamic_array *array);

/**
 * @brief Remove an element at a given index.
 *
 * @param array dynamic_array to modify.
 * @param index Index to access.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_remove(dynamic_array *array, unsigned int index);

/**
 * @brief Reallocate to only take up the necessary memory to hold the current
 * size.
 *
 * @param array dynamic_array to modify.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_shrink_to_fit(dynamic_array *array);

/**
 * @brief Deallocate and set to NULL.
 *
 * @param array dynamic_array to deallocate.
 */
void dynamic_array_destroy(dynamic_array **array);

/**
 * @brief Allocate necessary resources and setup.
 *
 * Use to iterate through a dynamic array.
 *
 * @param it dynamic array iterator to create.
 * @param array dynamic_array array to iterate through.
 * @return 0 on success, 1 otherwise
 */
 int dynamic_array_iterator_create(dynamic_array_iterator **it, dynamic_array *array);

/**
 * @brief Get the next element of the dynamic array
 *
 * @param it dynamic array iterator
 * @param item value used to hold the next elmeent in the array.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_iterator_next(dynamic_array_iterator *it, void **item);

/**
 * @brief Reset the dynamic array iterator.
 *
 * @param it dynamic array iterator
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_iterator_reset(dynamic_array_iterator *it);

/**
 * @brief Deallocate and set to NULL.
 *
 * @param it dynamic array iterator to deallocate.
 * @return 0 on success, 1 otherwise
 */
int dynamic_array_iterator_destroy(dynamic_array_iterator **it);

#endif // DYNAMIC_ARRAY_H
