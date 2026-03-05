#include "priority_queue.h"
#include "utils.h"
#include <stdalign.h>
#include <string.h>

#define ZERO_OUT_FALSE 0

// These functions return the largest integral value that is not greater than n.
// For example, FLOOR(0.5) is 0.0, and FLOOR(-0.5) is -1.0.
#define FLOOR(n)                                                               \
  ((int)((((n) < 0) && ((n) != ((float)((int)(n))))) ? (((int)(n)) - 1)        \
                                                     : ((int)(n))))
// Returns the index of i's parent
#define PARENT_INDEX(i) (int)FLOOR(((float)(i) - 1.0f) / 2.0f)
// Returns the index of i's left child
#define LEFT_CHILD_INDEX(i) (2 * (i) + 1)
// Returns the index of i's right child
#define RIGHT_CHILD_INDEX(i) (2 * (i) + 2)

struct priority_queue {
  void *items;
  /**
   * Comparison function
   *
   * @example:
   *
   * Comparison function that uses student.gpa to determine priority
   * and use student.name for duplicates.
   * This function makes the priority_queue a max heap that follows FIFO.
   * int student_comparefn(const void *a, const void *b) {
   *   student s1 = *(student *)a;
   *   student s2 = *(student *)b;
   *
   *   if (s1.gpa == s2.gpa) {
   *     int result = strcmp(s1.name, s2.name);
   *     // uncomment and replace to delete the last element first.
   *     // if (result > 0) return 1;
   *     // else if (result < 0) return -1;
   *     if (result < 0) return 1;
   *     else if (result > 0) return -1;
   *     return 0;
   *   }
   *
   *   float result = s1.gpa - s2.gpa;
   *   // uncomment and replace to use a min heap.
   *   // if (result > 0) return -1;
   *   // else if (result < 0) return 1;
   *   if (result < 0) return -1;
   *   else if (result > 0) return 1;
   *   return 0;
   * }
   */
  int (*comparefn)(const void *a, const void *b); // comparison function
  arena *arena;           // memory block used for allocations
  unsigned int data_size; // number of bytes for each item in the queue
  unsigned int capacity;  // current limit of items
  unsigned int size;      // elements of items
};

/**
 * @brief Resizes pq->items array when it reaches the pq->capacity
 *
 * @param pq priority queue
 * @return 0 on success, 1 otherwise
 */
static int priority_queue_resize(priority_queue **pq) {
  unsigned int old_capacity = (*pq)->capacity;

  if (((*pq)->items = arena_realloc((*pq)->arena, (*pq)->items,
                                    old_capacity * sizeof(void *),
                                    sizeof(void *) * ((*pq)->capacity <<= 1),
                                    alignof(void *), ZERO_OUT_FALSE)) == NULL) {
    return 1;
  }

  return 0;
}

/**
 * @brief Maintains the heap property after insertion.
 *
 * Swaps the newly added item with its parent until
 * the heap property is satisfied.
 *
 * @param pq priority queue to check
 * @param index the index of the newly added item
 */
static void heapify_up(priority_queue *pq, unsigned int index) {
  while (index > 0 && pq->comparefn((char *)pq->items + (index * pq->data_size),
                                    (char *)pq->items + ((PARENT_INDEX(index) *
                                                          pq->data_size))) >
                          0) { // parent is lesser
    int parent_index = PARENT_INDEX(index);

    // swap values
    char temp[pq->data_size];
    memcpy(temp, (char *)pq->items + (index * pq->data_size), pq->data_size);
    memcpy((char *)pq->items + (index * pq->data_size),
           (char *)pq->items + (parent_index * pq->data_size), pq->data_size);
    memcpy((char *)pq->items + (parent_index * pq->data_size), temp,
           pq->data_size);
    index = parent_index;
  }
}

/**
 * @brief Maintains the heap property after deletion.
 *
 * Swaps the newly added item with its left or right child until
 * the heap property is satisfied.
 *
 * @param pq priority queue to check
 * @param index the index of the root
 */
static void heapify_down(priority_queue *pq, unsigned int index) {
  while (index < pq->size) {
    unsigned int left_child_index = LEFT_CHILD_INDEX(index);
    unsigned int right_child_index = RIGHT_CHILD_INDEX(index);

    if ((left_child_index < pq->size) &&
        (pq->comparefn((char *)pq->items + (left_child_index * pq->data_size),
                       (char *)pq->items + (index * pq->data_size)) > 0) &&
        (pq->comparefn((char *)pq->items + (left_child_index * pq->data_size),
                       (char *)pq->items +
                           (right_child_index * pq->data_size)) > 0)) {
      // left child is greater than, so swap with left child
      char temp[pq->data_size];
      memcpy(temp, (char *)pq->items + (left_child_index * pq->data_size),
             pq->data_size);
      memcpy((char *)pq->items + (left_child_index * pq->data_size),
             (char *)pq->items + (index * pq->data_size), pq->data_size);
      memcpy((char *)pq->items + (index * pq->data_size), temp, pq->data_size);

      index = left_child_index;
      continue;
    } else if ((right_child_index < pq->size) &&
               (pq->comparefn(
                    (char *)pq->items + (right_child_index * pq->data_size),
                    (char *)pq->items + (index * pq->data_size)) > 0) &&
               (pq->comparefn(
                    (char *)pq->items + (right_child_index * pq->data_size),
                    (char *)pq->items + (left_child_index * pq->data_size)) >
                0)) {
      // right child is greater than, so swap with right child
      char temp[pq->data_size];
      memcpy(temp, (char *)pq->items + (right_child_index * pq->data_size),
             pq->data_size);
      memcpy((char *)pq->items + (right_child_index * pq->data_size),
             (char *)pq->items + (index * pq->data_size), pq->data_size);
      memcpy((char *)pq->items + (index * pq->data_size), temp, pq->data_size);

      index = right_child_index;

      continue;
    }

    index = pq->size; // indicates heap property is satisfied
  }
}

/**
 * @brief Default comparison function.
 *
 * @return the greater of 'a' and 'b', 'b' when equal
 */
static int choose_greater(const void *a, const void *b) {
  if (*(long *)a == *(long *)b) {
    return 1;
  }

  return *(long *)a - *(long *)b;
}

int priority_queue_create(priority_queue **pq, unsigned int initial_capacity,
                          unsigned int data_size,
                          int (*comparefn)(const void *a, const void *b),
                          arena *arena) {
  if (((*pq) = arena_alloc(arena, sizeof(priority_queue),
                           alignof(priority_queue), ZERO_OUT_FALSE)) == NULL) {
    return 1;
  }

  (*pq)->capacity = ROUND_POW2(initial_capacity);
  (*pq)->arena = arena;
  (*pq)->comparefn = comparefn == NULL ? choose_greater : comparefn;
  (*pq)->data_size = data_size;
  (*pq)->size = 0;

  if ((((*pq)->items) = arena_alloc(arena, (*pq)->capacity * sizeof(void *),
                                    alignof(void *), 1)) == NULL) {
    return 1;
  }

  return 0;
}

int priority_queue_insert(priority_queue *pq, void *data) {
  if (pq == NULL || data == NULL) { // must be defined
    return 1;
  }

  if (pq->size == pq->capacity) {
    if (priority_queue_resize(&pq) != 0) {
      return 1;
    }
  }

  memcpy((char *)pq->items + (pq->size * pq->data_size), data, pq->data_size);
  heapify_up(pq, pq->size);
  pq->size++;

  return 0;
}

int priority_queue_peek(priority_queue *pq, void *data) {
  if (pq == NULL || pq->size == 0 || data == NULL) {
    return 1;
  }

  // *data = pq->items[0];
  memcpy(data, pq->items, pq->data_size);
  return 0;
}

int priority_queue_delete(priority_queue *pq) {
  if (pq == NULL || pq->size == 0) {
    return 1;
  }

  // pq->items[0] = pq->items[pq->size - 1]; // last element is new root
  memcpy(pq->items, (char *)pq->items + ((pq->size - 1) * pq->data_size),
         pq->data_size);

  pq->size--;
  heapify_down(pq, 0);

  return 0;
}

int priority_queue_size(priority_queue *pq) {
  if (pq == NULL) {
    return -1;
  }

  return pq->size;
}

int priority_queue_is_empty(priority_queue *pq) {
  if (pq == NULL) {
    return 1;
  }

  return !(pq->size == 0);
}
