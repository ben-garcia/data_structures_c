#ifndef AVL_TREE_H
#define AVL_TREE_H

typedef struct avl_tree avl_tree;

/**
 * @brief Create the AVL 'tree'
 *
 * @param tree the tree to initialize
 * @param comparefn comparison function
 *        MUST return 0 if a == b,
 *             negative number if a < b
 *             positive number if a > b,
 * @return 0 on success, 1 otherwise
 */
int avl_tree_create(avl_tree **tree, int (*comparefn)(const void *a, const void *b));

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
unsigned int avl_tree_get_size(avl_tree *tree);

void avl_tree_print(avl_tree *tree);

/**
 * @brief Deallocate the resources
 *
 * @param tree AVL tree
 * @return 0 on success, 1 otherwise
*/
int avl_tree_destroy(avl_tree **tree);

#endif // AVL_TREE_H
