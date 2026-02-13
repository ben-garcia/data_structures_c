#include "include/avl_tree.h"
#include "include/dynamic_array.h"
#include "include/hash_table.h"
#include "include/linked_list.h"
#include "include/queue.h"
#include "include/stack.h"
#include "include/string_builder.h"
#include "include/string_view.h"

#include <stdio.h>
#include <stdlib.h>

void free_function(void **item) { free(*item); }
void free_long(void *data) { free(data); }
int comparefn(const void *a, const void *b) { return *(long *)a - *(long *)b; }

int main(void) {
  // dynamic array of longs
  dynamic_array *numbers;
  dynamic_array_create(&numbers, sizeof(long), free_function, NULL);
  for (long i = 1; i <= 100; i++) {
    long *num = malloc(sizeof(long));
    *num = i;
    dynamic_array_add(numbers, num);
  }
  dynamic_array_iterator *numbers_it;
  long *value = NULL;
  dynamic_array_iterator_create(&numbers_it, numbers);
  printf("===========dynamic_array(numbers)==============\n");
  while ((dynamic_array_iterator_next(numbers_it, (void **)&value)) == 0) {
    printf("%ld ", *value);
  }
  printf("\n\n");

  // string builder
  string_builder *sb;
  string_view str_view = {.data = " and Hello", .length = 10};
  char *buffer;
  string_builder_create(&sb);
  string_builder_append(sb, "Hello World!");
  string_builder_append_view(sb, str_view);
  string_builder_append_fmt_str(sb, "%s", " C!");
  string_builder_build(sb, &buffer);

  // string view
  string_view view = {.data = buffer, .length = 11};
  string_view view2 = {.data = "Testing", .length = 4};
  char *view_buffer;
  char *view2_buffer;
  string_view_to_string(&view, &view_buffer);
  string_view_to_string(&view2, &view2_buffer);

  // dynamic array of strings
  dynamic_array *strings;
  dynamic_array_create(&strings, sizeof(char *), NULL, NULL);
  dynamic_array_add(strings, buffer);
  dynamic_array_add(strings, view_buffer);
  dynamic_array_add(strings, view2_buffer);
  dynamic_array_iterator *strings_it;
  dynamic_array_iterator_create(&strings_it, strings);
  char *str;
  printf("===========dynamic_array(strings)==============\n");
  while (dynamic_array_iterator_next(strings_it, (void **)&str) == 0) {
    printf("'%s'\n", str);
  }
  printf("\n");

  // linked list
  linked_list *floats;
  linked_list_create(&floats, free_function, NULL);
  for (float f = 0.314; f < 10; f += 0.314) {
    float *f_ptr = malloc(sizeof(float));
    *f_ptr = f;
    linked_list_add(floats, f_ptr);
  }

  linked_list_iterator *float_it;
  float *fvalue;
  linked_list_iterator_create(&float_it, floats);
  printf("===========linked_list(floats)==============\n");
  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%f -> ", *fvalue);
  }
  printf("NULL\n\n");

  linked_list_reverse(&floats);

  linked_list_iterator_reset(&float_it, floats);
  printf("===========linked_list(floats) reversed==============\n");
  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%f -> ", *fvalue);
  }
  printf("NULL\n\n");

  // hash_table
  hash_table *chars;
  hash_table_create(&chars, sizeof(char), NULL, free_function);
  for (char c = 'a'; c <= 'z'; c++) {
    char *c_ptr = malloc(sizeof(char) + 1);
    snprintf(c_ptr, 2, "%c", c);
    hash_table_insert(chars, c_ptr, c_ptr);
  }
  hash_table_iterator *chars_it;
  hash_table_entry *entry;
  hash_table_iterator_create(&chars_it, chars);
  printf("===========hash_table(char)==============\n");
  while ((hash_table_iterator_next(chars_it, &entry)) == 0) {
    char *cvalue;
    char *ckey;
    hash_table_get_entry_key(entry, &ckey);
    hash_table_get_entry_value(entry, (void **)&cvalue);
    printf("{%s: %c} ", ckey, *cvalue);
  }
  printf("\n\n");

  // AVL tree
  printf("=============AVL tree=================\n");
  long tree_data[9] = {2, 1, 7, 4, 5, 5, 3, 8, 15};
  avl_tree *tree;

  avl_tree_create(&tree, comparefn, free_long);

  printf("inserting... ");
  for (int i = 0; i < 8; i++) {
    printf("%ld ", tree_data[i]);
    long *data_ptr = malloc(sizeof(long));
    *data_ptr = tree_data[i];
    if (avl_tree_insert(tree, (void *)data_ptr) != 0) {
      free(data_ptr); // insertion failed, no duplicates
    }
  }
  printf("\n");

  avl_tree_iterator *avl_it;

  avl_tree_iterator_create(&avl_it, tree);
  long *node_data;

  printf("AVL tree contains: ");
  while (avl_tree_iterator_next(avl_it, (void **)&node_data) == 0) {
    printf("%ld ", *node_data);
  }
  printf("\ndeleting... %ld %ld\n", tree_data[8], tree_data[0]);
  avl_tree_delete(tree, (void *)&tree_data[8]); // 15
  avl_tree_delete(tree, (void *)&tree_data[0]); // 2

  avl_tree_iterator_reset(&avl_it);

  printf("AVL tree contains: ");
  while (avl_tree_iterator_next(avl_it, (void **)&node_data) == 0) {
    printf("%ld ", *node_data);
  }
  printf("\n\n");

  printf("=============stack====================\n");
  char *stack_data[] = {"s", "t", "a", "c", "k"};
  stack *my_stack;

  stack_create(&my_stack);

  for (int i = 0; i < 5; i++) {
    printf("pushing %s\n", stack_data[i]);
    stack_push(my_stack, stack_data[i]);
  }

  printf("\n");

  while (stack_is_empty(my_stack) != 0) {
    printf("poping %s\n", (char *)stack_pop(my_stack));
  }

  printf("\n");

  printf("=============queue====================\n");
  char *queue_data[] = {"q", "u", "e", "u", "e"};
  queue *my_queue;

  queue_create(&my_queue);

  for (int i = 0; i < 5; i++) {
    printf("enqueing %s\n", queue_data[i]);
    queue_enqueue(my_queue, queue_data[i]);
  }

  printf("\n");

  while (queue_is_empty(my_queue) != 0) {
    printf("dequeing %s\n", (char *)queue_dequeue(my_queue));
  }

  printf("\n");

  // de-allocate
  dynamic_array_destroy(&numbers);
  dynamic_array_destroy(&strings);
  dynamic_array_iterator_destroy(&numbers_it);
  dynamic_array_iterator_destroy(&strings_it);
  string_builder_destroy(&sb);
  linked_list_destroy(&floats);
  linked_list_iterator_destroy(&float_it);
  hash_table_destroy(&chars);
  hash_table_iterator_destroy(&chars_it);
  avl_tree_destroy(&tree);
  avl_tree_iterator_destroy(&avl_it);
  free(buffer);
  free(view_buffer);
  free(view2_buffer);
  stack_destroy(&my_stack);
  queue_destroy(&my_queue);

  return 0;
}
