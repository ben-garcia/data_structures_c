#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "arena.h"

typedef struct avl_tree avl_tree;
typedef struct avl_tree_iterator avl_tree_iterator;

/**
 * @brief Create the AVL 'tree'
 *
 * @param tree the tree to initialize
 * @param comparefn comparison function
 *        MUST return 0 if a == b,
 *             negative number if a < b
 *             positive number if a > b,
 * @param arena memory block for allocations
 * @return 0 on success, 1 otherwise
 */
int avl_tree_create(avl_tree **tree,
                    int (*comparefn)(const void *a, const void *b),
                    arena *arena);

/**
 * @brief Search the 'tree' for 'data'
 *
 * @param tree the AVL tree to search
 * @param data the item to search for
 * @return 0 on success, 1 otherwise
 */
int avl_tree_search(avl_tree *tree, void *data);

/**
 * @brief Insert a new node to the 'tree' with 'data'
 *
 * @param tree the AVL tree to modify
 * @param data the item to insert
 * @return 0 on success, 1 otherwise
 */
int avl_tree_insert(avl_tree *tree, void *data);

/**
 * @brief Delete a node from the 'tree' with 'data'
 *
 * @param tree the AVL tree to modify
 * @param data the item to delete
 * @return 0 on success, 1 otherwise
 */
int avl_tree_delete(avl_tree *tree, void *data);

/**
 * @brief Return the size of the 'tree'
 *
 * @param tree the AVL tree
 * @return the size of AVL 'tree'
 */
unsigned int avl_tree_size(avl_tree *tree);

/**
 * @brief Deallocate the resources
 *
 * @param tree AVL tree
 * @return 0 on success, 1 otherwise
 */
int avl_tree_destroy(avl_tree **tree);

/**
 * Allocate necessary resources and setup.
 *
 * Use to iterate through an AVL tree.
 *
 * @param it AVL tree iterator to create.
 * @param tree AVL tree to iterate through.
 * @return 0 on success, 1 otherwise
 */
int avl_tree_iterator_create(avl_tree_iterator **it, avl_tree *tree);

/**
 * Get the next data in the AVL tree.
 *
 * @param it AVL tree iterator
 * @param data value used to hold the next data in the AVL tree
 * @return 0 on success, 1 otherwise
 */
int avl_tree_iterator_next(avl_tree_iterator *it, void **data);

/**
 * Reset the AVL tree iterator.
 *
 * Use before iterating AVL tree for a second time.
 *
 * @param it AVL tree iterator
 * @return 0 on success, 1 otherwise
 */
int avl_tree_iterator_reset(avl_tree_iterator **it);

/**
 * Deallocate and set to NULL.
 *
 * @param it AVL tree iterator to deallocate.
 * @return 0 on success, 1 otherwise
 */
int avl_tree_iterator_destroy(avl_tree_iterator **it);

#endif // AVL_TREE_H
