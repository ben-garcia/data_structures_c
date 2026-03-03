#include "avl_tree.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int comparefn(const void *a, const void *b) { return *(long *)a - *(long *)b; }

int main(void) {
  arena *arena;
  avl_tree_iterator *avl_it;

  arena_create(&arena, KB(4));

  printf("===========AVL tree example=========\n");
  long tree_data[9] = {2, 1, 7, 4, 5, 5, 3, 8, 15};
  avl_tree *tree;

  avl_tree_create(&tree, comparefn, arena);

  printf("inserting... ");
  for (int i = 0; i < 8; i++) {
    long *data_ptr = arena_alloc(arena, sizeof(long), alignof(long), 0);
    *data_ptr = tree_data[i];
    printf("%ld ", *data_ptr);
    avl_tree_insert(tree, (void *)data_ptr);
  }

  printf("\ntree size: %d\n", avl_tree_size(tree));

  avl_tree_iterator_create(&avl_it, tree);
  long *node_data;

  printf("AVL tree contains: ");
  while (avl_tree_iterator_next(avl_it, (void **)&node_data) == 0) {
    printf("%ld ", *node_data);
  }
  printf("\n\ndeleting... %ld %ld\n", tree_data[8], tree_data[0]);
  avl_tree_delete(tree, (void *)&tree_data[8]); // 15

  printf("searching for (%ld), found 0(yes), 1(no): %d\n", tree_data[0],
         avl_tree_search(tree, &tree_data[0]));

  avl_tree_delete(tree, (void *)&tree_data[0]); // 2

  avl_tree_iterator_reset(&avl_it);

  printf("AVL tree contains: ");
  while (avl_tree_iterator_next(avl_it, (void **)&node_data) == 0) {
    printf("%ld ", *node_data);
  }

  printf("\n");

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
