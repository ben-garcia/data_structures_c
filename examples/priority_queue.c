#include "priority_queue.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(n) ((int)((sizeof((n))) / (sizeof((n)[0]))))

typedef struct student {
  char *name;
  float gpa;
  int id;
} student;

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
  arena *arena;

  arena_create(&arena, KB(4));

  printf("============MIN priority queue============\n");
  student min_pqueue_data[] = {
      {"foo1", 3.1f, 1},   {"bar1", 4.0f, 2}, {"baz1", 3.9f, 3},
      {"bar2", 4.0f, 4},   {"baz2", 3.9f, 5}, {"foo2", 3.1f, 6},
      {"foobaz", 1.9f, 7}, {"bar3", 4.0f, 8}, {"foobar", 1.1f, 9}};
  priority_queue *min_pqueue;

  priority_queue_create(&min_pqueue, 16, student_comparefn, arena);

  printf("min priority queue size: %d\n\n", priority_queue_size(min_pqueue));

  for (int i = 0; i < ARRAY_SIZE(min_pqueue_data); i++) {
    printf("inserting {name: %s, gpa: %.2f, id: %d}\n",
    min_pqueue_data[i].name,
           min_pqueue_data[i].gpa, min_pqueue_data[i].id);
    priority_queue_insert(min_pqueue, &min_pqueue_data[i]);
  }

  printf("\nmin priority queue size: %d\n\n", priority_queue_size(min_pqueue));

  while (priority_queue_is_empty(min_pqueue) != 0) {
    student *data;
    priority_queue_peek(min_pqueue, (void **)&data);
    printf("deleting {name: %s, gpa: %.2f, id: %d}\n", data->name, data->gpa,
           data->id);
    priority_queue_delete(min_pqueue);
  }

  printf("\nmin priority queue size: %d\n\n", priority_queue_size(min_pqueue));

  printf("============MAX priority queue============\n");
  long max_pqueue_data[] = {9, 8, 2, 3, 4, 5, 6, 7, 5};
  priority_queue *max_pqueue;

  priority_queue_create(&max_pqueue, 16, NULL, arena);

  printf("inserting... ");
  for (int i = 0; i < ARRAY_SIZE(min_pqueue_data); i++) {
    printf("%ld ", max_pqueue_data[i]);
    long *long_ptr =
        arena_alloc(arena, sizeof(long), alignof(long), 0);
    *long_ptr = max_pqueue_data[i];
    priority_queue_insert(max_pqueue, long_ptr);
  }

  printf("\n\nmax priority queue size: %d\n", priority_queue_size(max_pqueue));

  printf("\ndeleting... ");

  while (priority_queue_is_empty(max_pqueue) != 0) {
    long *data;
    priority_queue_peek(max_pqueue, (void **)&data);
    printf(" %ld", *data);
    priority_queue_delete(max_pqueue);
  }

  printf("\n\nmax priority queue size: %d\n", priority_queue_size(max_pqueue));
  
  // de-allocate
  arena_destroy(&arena);

  return 0;
}
