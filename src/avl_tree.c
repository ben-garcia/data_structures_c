#include "include/avl_tree.h"

#include <stdio.h>
#include <stdlib.h>

// Return the maximum of a and b
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
// Return the height of tree n
#define GET_HEIGHT(n) (((n) == NULL) ? 0 : ((n)->height))
// Return the balance factor of tree n
#define GET_BALANCE_FACTOR(n)                                                  \
  (((n) == NULL) ? 0 : (GET_HEIGHT((n)->left) - GET_HEIGHT((n)->right)))

struct avl_tree {
  struct avl_tree_node *root;
  /**
   * Comparison function
   * Must return 0 if 1 == b
   *             < 0 if a < b
   *             > 0 if a > b
   */
  int (*comparefn)(const void *a, const void *b);
  unsigned int size; // number of nodes
};

typedef struct avl_tree_node {
  struct avl_tree_node *left;
  struct avl_tree_node *right;
  void *data;
  int height;
} avl_tree_node;

struct avl_tree_iterator {
  avl_tree *tree;
  void **nodes;
  unsigned int current_index;
  unsigned int nodes_current_size;
};

/**
 * Create a AVL Tree node
 *
 * @param data the data the node will hold
 * @return the newly created node
 */
static avl_tree_node *avl_tree_node_create(void *data) {
  avl_tree_node *node = malloc(sizeof(avl_tree_node));
  if (node == NULL) {
    return NULL;
  }

  node->data = data;
  node->left = NULL;
  node->right = NULL;
  node->height = 1;

  return node;
}

/**
 * Perform a right rotation on node x
 *
 * Example:
 *  '*' indicates node to be rotated
 *
 *      3*          2
 *     /          /  \
 *    2    --->  1    3
 *   /
 *  1
 */
static avl_tree_node *right_rotation(avl_tree_node *y) {
  avl_tree_node *x = y->left;
  avl_tree_node *T2 = x->right;

  // rotate
  x->right = y;
  y->left = T2;

  // update heights
  y->height = MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right)) + 1;
  x->height = MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right)) + 1;

  return x;
}

/**
 * Perform a left rotation on node x
 *
 * Example:
 *  '*' indicates node to be rotated
 *
 *    1*             2
 *     \           /  \
 *      2  --->   1    3
 *       \
 *        3
 */
static avl_tree_node *left_rotation(avl_tree_node *x) {
  avl_tree_node *y = x->right;
  avl_tree_node *T2 = y->left;

  // rotate
  y->left = x;
  x->right = T2;

  // update heights
  x->height = MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right)) + 1;
  y->height = MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right)) + 1;

  return y;
}

/**
 * Add a new node to the 'node'
 *
 * @param node parent node
 * @param data the item to add to the node
 * @param is_duplicate flag used to indicate if data is a already a node
 * @param comparefn comparison function
 */
static avl_tree_node *
insert_node(avl_tree_node *node, void *data, int *is_duplicate,
            int (*comparefn)(const void *a, const void *b)) {
  if (node == NULL) {
    return avl_tree_node_create(data);
  }

  int compare_result = comparefn(data, node->data);
  if (compare_result == 0) {
    *is_duplicate = 0;
    return node; // No duplicates or updates allowed
  } else if (compare_result < 0) {
    node->left = insert_node(node->left, data, is_duplicate, comparefn);
  } else if (compare_result > 0) {
    node->right = insert_node(node->right, data, is_duplicate, comparefn);
  }

  node->height = MAX(GET_HEIGHT(node->left), GET_HEIGHT(node->right)) + 1;

  // Check for unbalance and update
  int balance_factor = GET_BALANCE_FACTOR(node);
  // left left
  if (balance_factor > 1 && comparefn(data, node->left->data) < 0) {
    return right_rotation(node);
  }

  // right right
  if (balance_factor < -1 && comparefn(data, node->right->data) > 0) {
    return left_rotation(node);
  }

  /**
   * left right rotation
   *
   * Example:
   *  '*' indicates node to be rotated
   *
   *     left        right     final
   *       7           7*
   *      /           /
   *     4*   --->   5   --->   5
   *      \         /          / \
   *       5       4          4   7
   */
  if (balance_factor > 1 && comparefn(data, node->left->data) > 0) {
    node->left = left_rotation(node->left);
    return right_rotation(node);
  }

  /**
   * right left rotation
   *
   * Example:
   *  '*' indicates node to be rotated
   *
   *     right    left        final
   *   4          4*
   *    \          \
   *     7*  --->   5   --->     5
   *    /            \          / \
   *   5              7        4   7
   */
  if (balance_factor < -1 && comparefn(data, node->right->data) < 0) {
    node->right = right_rotation(node->right);
    return left_rotation(node);
  }

  return node;
}

/**
 * Find the node with the minimal value stating at 'node'
 *
 * @param node the node to begin the search
 * @return node with the minimum value
 */
static avl_tree_node *get_min_value_node(avl_tree_node *node) {
  avl_tree_node *current = node;

  while (current->left != NULL) {
    current = current->left;
  }

  return current;
}

/**
 *  Delete a node from the 'node'
 *
 *  @param node parent node
 *  @param data the item to delete to the node
 *  @param is_found flag that indicates if a node with 'data' is found
 *  @param comparefn comparison function
 *  @return 'node' param
 */
static avl_tree_node *
delete_node(avl_tree_node *node, void *data, int *is_found,
            int (*comparefn)(const void *a, const void *b)) {
  if (node == NULL) {
    *is_found = 0; // node not found
    return node;
  }

  if (data < node->data) {
    node->left = delete_node(node->left, data, is_found, comparefn);
  }

  else if (data > node->data) {
    node->right = delete_node(node->right, data, is_found, comparefn);
  }

  else {
    if ((node->left == NULL) || (node->right == NULL)) {
      avl_tree_node *temp = node->left ? node->left : node->right;

      if (temp == NULL) {
        temp = node;
        node = NULL;
      } else {
        *node = *temp;
      }
      free(temp);
    } else {
      avl_tree_node *temp = get_min_value_node(node->right);

      node->data = temp->data;

      node->right = delete_node(node->right, temp->data, is_found, comparefn);
    }
  }

  if (node == NULL) {
    return node;
  }

  // Update the node height
  node->height = MAX(GET_HEIGHT(node->left), GET_HEIGHT(node->right)) + 1;

  // Check the balance and update
  int balance_factor = GET_BALANCE_FACTOR(node);
  // left left
  if (balance_factor > 1 && GET_BALANCE_FACTOR(node->left) >= 0) {
    return right_rotation(node);
  }

  // right right
  if (balance_factor < -1 && GET_BALANCE_FACTOR(node->right) <= 0) {
    return left_rotation(node);
  }

  // left right
  if (balance_factor > 1 && GET_BALANCE_FACTOR(node->left) < 0) {
    node->left = left_rotation(node->left);
    return right_rotation(node);
  }

  // right left
  if (balance_factor < -1 && GET_BALANCE_FACTOR(node->right) > 0) {
    node->right = right_rotation(node->right);
    return left_rotation(node);
  }

  return node;
}

/**
 * Search the 'node' for 'data'
 *
 * @param node the root node to search from
 * @param data the item that is being search for
 * @param comparefn the comparison function
 * @return the data at that node
 */
static void *search(avl_tree_node *node, void *data,
                    int (*comparefn)(const void *a, const void *b)) {
  if (node == NULL) {
    return NULL;
  }

  int result = comparefn(data, node->data);

  if (result == 0) {
    return node->data;
  } else if (result < 0) {
    return search(node->left, data, comparefn);
  } else if (result > 0) {
    return search(node->right, data, comparefn);
  }

  return NULL;
}

/**
 * Deallocate all nodes in the tree
 *
 * @param node the root node to start from
 */
static void destroy(avl_tree_node *node) {
  if (node == NULL) {
    return;
  }

  destroy(node->left);
  destroy(node->right);
  free(node);
}

int avl_tree_create(avl_tree **tree,
                    int (*comparefn)(const void *a, const void *b)) {
  if (comparefn == NULL) {
    return 1;
  }
  if ((*tree = malloc(sizeof(avl_tree_node))) == NULL) {
    return 1;
  }

  (*tree)->comparefn = comparefn;
  (*tree)->root = NULL;
  (*tree)->size = 0;

  return 0;
}

unsigned int avl_tree_get_size(avl_tree *tree) { return tree->size; }

int avl_tree_insert(avl_tree *tree, void *data) {
  int result = 1;

  tree->root = insert_node(tree->root, data, &result, tree->comparefn);

  if (result == 1) {
    tree->size++; // only increment size for every unique entry
  }

  return !result;
}

int avl_tree_search(avl_tree *tree, void *data) {
  void *result = search(tree->root, data, tree->comparefn);
  if (result == NULL) {
    return 1;
  }

  return 0;
}

int avl_tree_delete(avl_tree *tree, void *data) {
  int result = 1;

  tree->root = delete_node(tree->root, data, &result, tree->comparefn);

  if (result == 1) {
    tree->size--; // only decrement size if key is found
  }

  return !result;
}

int avl_tree_destroy(avl_tree **tree) {
  if (*tree == NULL) {
    return 1;
  }

  destroy((*tree)->root);

  free(*tree);
  *tree = NULL;

  return 0;
}

static void build_iteratorator_array(avl_tree_node *node, avl_tree_iterator *it) {
  if (node == NULL) {
    return;
  }

  build_iteratorator_array(node->left, it);
  it->nodes[it->nodes_current_size++] = node->data;
  build_iteratorator_array(node->right, it);
}

int avl_tree_iterator_create(avl_tree_iterator **it, avl_tree *tree) {
  if (tree == NULL || tree->size == 0) {
    return 1;
  }

  if (((*it) = malloc(sizeof(avl_tree_iterator))) == NULL) {
    return 1;
  }

  if (((*it)->nodes = malloc(sizeof(void *) * tree->size)) == NULL) {
    return 1;
  }

  (*it)->tree = tree;
  (*it)->current_index = 0;
  (*it)->nodes_current_size = 0;

  build_iteratorator_array(tree->root, *it);

  return 0;
}

int avl_tree_iterator_next(avl_tree_iterator *it, void **data) {
  if (it == NULL) {
    return 1;
  }

  if (it->current_index >= it->tree->size) { // out of bounds check
    return 1;
  }

  *data = it->nodes[it->current_index++];

  return 0;
}

int avl_tree_iterator_reset(avl_tree_iterator **it) {
  if (it == NULL) {
    return 1;
  }

  (*it)->current_index = 0;
  (*it)->nodes_current_size = 0;

  build_iteratorator_array((*it)->tree->root, *it);

  return 0;
}

int avl_tree_iterator_destroy(avl_tree_iterator **it) {
  if (*it == NULL) {
    return 1;
  }

  free((*it)->nodes);
  free(*it);
  *it = NULL;

  return 0;
}
