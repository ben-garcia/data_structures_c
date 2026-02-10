#include "include/avl_tree.h"
#include "include/dynamic_array.h"
#include "include/hash_table.h"
#include "include/linked_list.h"
#include "include/string_builder.h"
#include "include/string_view.h"

#include <stdio.h>
#include <stdlib.h>

void free_function(void **item) { free(*item); }
int comparefn(const void *a, const void *b) { return (long)a - (long)b; }

int main(void) {
  // dynamic array of longs
  dynamic_array *numbers;
  dynamic_array_create(&numbers, sizeof(long), free_function, NULL);
  for (long i = 1; i <= 100; i++) {
    long *num = malloc(sizeof(long));
    *num = i;
    dynamic_array_add(numbers, num);
  }
  dynamic_array_iter *numbers_it;
  long *value = NULL;
  dynamic_array_iter_create(&numbers_it, numbers);
  printf("===========dynamic_array(numbers)==============\n");
  while ((dynamic_array_iter_next(numbers_it, (void **)&value)) == 0) {
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
  dynamic_array_iter *strings_it;
  dynamic_array_iter_create(&strings_it, strings);
  char *str;
  printf("===========dynamic_array(strings)==============\n");
  while (dynamic_array_iter_next(strings_it, (void **)&str) == 0) {
    printf("'%s'\n", str);
  }
  printf("\n");

  // linked list
  linked_list *floats;
  linked_list_create(&floats, sizeof(float), free_function, NULL);
  for (float f = 0.314; f < 10; f += 0.314) {
    float *f_ptr = malloc(sizeof(float));
    *f_ptr = f;
    linked_list_add(floats, f_ptr);
  }

  linked_list_iter *float_it;
  float *fvalue;
  linked_list_iter_create(&float_it, floats);
  printf("===========linked_list(floats)==============\n");
  while (linked_list_iter_next(float_it, (void **)&fvalue) == 0) {
    printf("%f -> ", *fvalue);
  }
  printf("NULL\n\n");

  linked_list_reverse(&floats);

  linked_list_iter_reset(&float_it, floats);
  printf("===========linked_list(floats) reversed==============\n");
  while (linked_list_iter_next(float_it, (void **)&fvalue) == 0) {
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
  hash_table_iter *chars_it;
  hash_table_entry *entry;
  hash_table_iter_create(&chars_it, chars);
  printf("===========hash_table(char)==============\n");
  while ((hash_table_iter_next(chars_it, &entry)) == 0) {
    char *cvalue;
    char *ckey;
    hash_table_get_entry_key(entry, &ckey);
    hash_table_get_entry_value(entry, (void **)&cvalue);
    printf("{%s: %c} ", ckey, *cvalue);
  }
  printf("\n\n");

  // AVL tree
  long tree_data[9] = {2, 1, 7, 4, 5, 5, 3, 8, 15};
  avl_tree *tree;

  avl_tree_create(&tree, comparefn);

  for (long i = 0; i < 8; i++) {
    avl_tree_insert(tree, (void *)tree_data[i]);
  }

  avl_tree_search(tree, (void *)tree_data[8]); // 5

  printf("===========avl tree==============\n");
  avl_tree_print(tree);

  avl_tree_delete(tree, (void *)tree_data[8]); // 15
  avl_tree_delete(tree, (void *)tree_data[0]); // 2

  printf("after deleting 15 and 2\n");
  avl_tree_print(tree);

  // de-allocate
  dynamic_array_destroy(&numbers);
  dynamic_array_destroy(&strings);
  dynamic_array_iter_destroy(&numbers_it);
  dynamic_array_iter_destroy(&strings_it);
  string_builder_destroy(&sb);
  linked_list_destroy(&floats);
  linked_list_iter_destroy(&float_it);
  hash_table_destroy(&chars);
  hash_table_iter_destroy(&chars_it);
  avl_tree_destroy(&tree);
  free(buffer);
  free(view_buffer);
  free(view2_buffer);

  return 0;
}
