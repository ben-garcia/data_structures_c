#include "include/arena.h"
#include "include/avl_tree.h"
#include "include/deque.h"
#include "include/dynamic_array.h"
#include "include/hash_table.h"
#include "include/linked_list.h"
#include "include/priority_queue.h"
#include "include/queue.h"
#include "include/stack.h"
#include "include/string_builder.h"
#include "include/string_view.h"

#include <stdalign.h>
#include <stdio.h>
#include <stdlib.h>

#define FALSE 0

// Return the size of array 'n'
#define ARRAY_SIZE(n) ((int)((sizeof((n))) / (sizeof((n)[0]))))

typedef struct student {
  char *name;
  float gpa;
  int id;
} student;

int comparefn(const void *a, const void *b) { return *(long *)a - *(long *)b; }
// Comparison function that uses student.gpa to determine priority
// and use student.name for duplicates.
// This function makes the priority_queue a max heap that follows FIFO.
int student_comparefn(const void *a, const void *b) {
  student s1 = *(student *)a;
  student s2 = *(student *)b;

  if (s1.gpa == s2.gpa) {
    int result = strcmp(s1.name, s2.name);
    // uncomment to remove the last element added first.
    // if (result > 0) return 1;
    // else if (result < 0) return -1;
    if (result < 0) {
      return 1;
    } else if (result > 0) {
      return -1;
    }
    return 0;
  }

  float result = s1.gpa - s2.gpa;
  // uncomment to convert to max heap.
  // if (result < 0)
  //   return -1;
  // else if (result > 0)
  //   return 1;
  if (result > 0) {
    return -1;
  } else if (result < 0) {
    return 1;
  }
  return 0;
}

int main(void) {
  arena *permanent_arena;
  arena_create(&permanent_arena, MB(4));

  printf("===========dynamic_array(numbers)==============\n");
  dynamic_array *numbers;
  dynamic_array_create(&numbers, 16, sizeof(long), NULL, permanent_arena);

  for (long i = 1; i <= 10; i++) {
    long *num =
        arena_alloc(permanent_arena, sizeof(long), alignof(long), FALSE);
    *num = i;
    dynamic_array_add(numbers, num);
  }

  dynamic_array_iterator *numbers_it;
  long *value = NULL;
  dynamic_array_iterator_create(&numbers_it, numbers);

  while ((dynamic_array_iterator_next(numbers_it, (void **)&value)) == 0) {
    printf("%ld ", *value);
  }

  printf("\n\n");

  // string builder
  string_builder *sb;
  string_view *str_view;
  string_view_create(&str_view, " and Hello", permanent_arena);
  char *buffer;

  string_builder_create(&sb, permanent_arena);
  string_builder_append(sb, "Hello World!");
  string_builder_append_view(sb, str_view);
  string_builder_append_fmt_str(sb, "%s", " C!");
  string_builder_build(sb, &buffer);

  // string view
  string_view *view ;
  string_view_create(&view, buffer,permanent_arena);
  string_view *view2;
  string_view_create(&view2, "Testing",permanent_arena);
  char *view_buffer;
  char *view2_buffer;
  string_view_to_string(view, &view_buffer);
  string_view_to_string(view2, &view2_buffer);

  printf("===========dynamic_array(strings)==============\n");
  dynamic_array *strings;
  dynamic_array_iterator *strings_it;
  char *str;

  dynamic_array_create(&strings, 8, sizeof(char *), NULL, permanent_arena);
  dynamic_array_add(strings, buffer);
  dynamic_array_add(strings, view_buffer);
  dynamic_array_add(strings, view2_buffer);

  dynamic_array_iterator_create(&strings_it, strings);

  while (dynamic_array_iterator_next(strings_it, (void **)&str) == 0) {
    printf("'%s'\n", str);
  }
  printf("\n");

  printf("===========linked_list(floats)==============\n");
  linked_list *floats;
  linked_list_create(&floats, NULL, permanent_arena);
  for (float f = 0.314; f < 10; f += 0.314) {
    float *f_ptr =
        arena_alloc(permanent_arena, sizeof(float), alignof(float), FALSE);
    *f_ptr = f;
    linked_list_add(floats, f_ptr);
  }

  linked_list_iterator *float_it;
  float *fvalue;
  linked_list_iterator_create(&float_it, floats);
  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%.2f -> ", *fvalue);
  }

  printf("NULL\n\n");

  printf("===========linked_list(floats) reversed==============\n");
  linked_list_reverse(&floats);

  linked_list_iterator_reset(&float_it, floats);

  while (linked_list_iterator_next(float_it, (void **)&fvalue) == 0) {
    printf("%.2f -> ", *fvalue);
  }

  printf("NULL\n\n");

  printf("===========hash_table(char)==============\n");
  hash_table *chars;
  hash_table_create(&chars, 32, sizeof(char), NULL, permanent_arena);

  for (char c = 'a'; c <= 'z'; c++) {
    char *c_ptr =
        arena_alloc(permanent_arena, sizeof(char) + 1, alignof(char *), FALSE);
    snprintf(c_ptr, 2, "%c", c);
    hash_table_insert(chars, c_ptr, c_ptr);
  }

  hash_table_iterator *chars_it;
  hash_table_entry *entry;
  hash_table_iterator_create(&chars_it, chars);

  while ((hash_table_iterator_next(chars_it, &entry)) == 0) {
    char *cvalue;
    char *ckey;
    hash_table_get_entry_key(entry, &ckey);
    hash_table_get_entry_value(entry, (void **)&cvalue);
    printf("{%s: %c} ", ckey, *cvalue);
  }

  printf("\n\n");

  printf("=============AVL tree=================\n");
  long tree_data[9] = {2, 1, 7, 4, 5, 5, 3, 8, 15};
  avl_tree *tree;

  avl_tree_create(&tree, comparefn, permanent_arena);

  printf("inserting... ");
  for (int i = 0; i < 8; i++) {
    long *data_ptr =
        arena_alloc(permanent_arena, sizeof(long), alignof(long), FALSE);
    *data_ptr = tree_data[i];
    printf("%ld ", *data_ptr);
    avl_tree_insert(tree, (void *)data_ptr);
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

  stack_create(&my_stack, permanent_arena);

  for (int i = 0; i < 5; i++) {
    printf("pushing %s\n", stack_data[i]);
    stack_push(my_stack, stack_data[i]);
  }

  printf("\n");

  while (stack_is_empty(my_stack) != 0) {
    void *data;
    stack_peek(my_stack, &data);
    stack_pop(my_stack);
    printf("poping %s\n", (char *)data);
  }

  printf("\n");

  printf("=============queue====================\n");
  char *queue_data[] = {"q", "u", "e", "u", "e"};
  queue *my_queue;

  queue_create(&my_queue, permanent_arena);

  for (int i = 0; i < 5; i++) {
    printf("enqueing %s\n", queue_data[i]);
    queue_enqueue(my_queue, queue_data[i]);
  }

  printf("\n");

  while (queue_is_empty(my_queue) != 0) {
    void *data;
    queue_peek(my_queue, &data);
    printf("dequeing %c\n", *(char *)data);
    queue_dequeue(my_queue);
  }

  printf("\n");

  printf("============MIN priority queue============\n");
  student max_pqueue_data[] = {
      {"foo1", 3.1f, 1},   {"bar1", 4.0f, 2}, {"baz1", 3.9f, 3},
      {"bar2", 4.0f, 4},   {"baz2", 3.9f, 5}, {"foo2", 3.1f, 6},
      {"foobaz", 1.9f, 7}, {"bar3", 4.0f, 8}, {"foobar", 1.1f, 9}};
  priority_queue *max_pqueue;

  priority_queue_create(&max_pqueue, 16, student_comparefn, permanent_arena);

  for (int i = 0; i < ARRAY_SIZE(max_pqueue_data); i++) {
    printf("inserting {name: %s, gpa: %.2f, id: %d}\n", max_pqueue_data[i].name,
           max_pqueue_data[i].gpa, max_pqueue_data[i].id);
    priority_queue_insert(max_pqueue, &max_pqueue_data[i]);
  }

  printf("\n");

  while (priority_queue_is_empty(max_pqueue) != 0) {
    student *data;
    priority_queue_peek(max_pqueue, (void **)&data);
    printf("deleting {name: %s, gpa: %.2f, id: %d}\n", data->name, data->gpa,
           data->id);
    priority_queue_delete(max_pqueue);
  }

  printf("\n");

  printf("============MAX priority queue============\n");
  long min_pqueue_data[] = {9, 8, 2, 3, 4, 5, 6, 7, 5};
  priority_queue *min_pqueue;

  priority_queue_create(&min_pqueue, 16, NULL, permanent_arena);

  printf("inserting... ");
  for (int i = 0; i < ARRAY_SIZE(min_pqueue_data); i++) {
    printf("%ld ", min_pqueue_data[i]);
    long *long_ptr =
        arena_alloc(permanent_arena, sizeof(long), alignof(long), FALSE);
    *long_ptr = min_pqueue_data[i];
    priority_queue_insert(min_pqueue, long_ptr);
  }

  printf("\ndeleting... ");

  while (priority_queue_is_empty(min_pqueue) != 0) {
    long *data;
    priority_queue_peek(min_pqueue, (void **)&data);
    printf(" %ld", *data);
    priority_queue_delete(min_pqueue);
  }

  printf("\n");

  printf("==============deque================\n");

  char *deque_data[] = {"e", "u", "q", "e", "d", "e", "u", "q", "e", "d"};
  deque *my_deque;
  deque_create(&my_deque, permanent_arena);

  printf("adding to the back... ");
  for (int i = 0; i < 5; i++) {
    printf("%s ", deque_data[i]);
    deque_add_back(my_deque, deque_data[i]);
  }

  printf("\nadding to the front... ");
  for (int i = 5; i < 10; i++) {
    printf("%s ", deque_data[i]);
    deque_add_front(my_deque, deque_data[i]);
  }

  printf("\nremoving from the front... ");
  for (int i = 0; i < 5; i++) {
    char *data;
    deque_peek_front(my_deque, (void **)&data);
    printf("%s ", data);
    deque_remove_front(my_deque);
  }

  printf("\nremoving from the back... ");
  for (int i = 5; i < 10; i++) {
    char *data;
    deque_peek_back(my_deque, (void **)&data);
    printf("%s ", data);
    deque_remove_back(my_deque);
  }

  printf("\n");

  // de-allocate
  arena_destroy(&permanent_arena);

  return 0;
}
