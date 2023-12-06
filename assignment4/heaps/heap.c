/* Name: Kraiwin Namsri
 * UvA-netID: 
 *
 * This program implements a min-heap.
 */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

#define INITIAL_HEAP_ARRAY_SIZE 64

static void print_array(struct heap *h) {
  long size = array_size(h->array);
  for (int i = 0; i < size; i++) {
    void * value = array_get(h->array, i);
    printf("array[%d] = %ld\n", i, (long)value);
  }
}

/* Returns the amount of elements inside the heap
 * Returns -1 on error.
 */
long heap_size(struct heap *h) {
  long size = array_size(h->array);
  if (size == -1l)
    return -1;
  return size - 1;
}

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
  struct heap *h = malloc(sizeof(struct heap));
  if (!h || !compare)
    return NULL;

  struct array *array = array_init(INITIAL_HEAP_ARRAY_SIZE);
  if (!array) {
    free(h);
    return NULL;
  }
  array_append(array, NULL);
  h->array = array;
  h->compare = compare;
  assert(heap_size(h) == 0);
  return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(const prioq *q) {
  if (!q)
    return -1;
  return array_size(q->array);
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
  if (!h)
    return 1;
  array_cleanup(h->array, free_func);
  free(h);
  return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
  return heap_cleanup(q, free_func);
}

/* Calculates the parent's index of the child.
 * Beware! It is the programmers duty to check if the return-value is correct!
 */
static long calculate_parent_index(long index_child) {
  /* No need to check if it is the right or left child, because there is integer devision flooring. */
  return index_child / 2;
}

/* Swap index a with b inside the array of h. 
 */
int swap(struct heap *h, long a, long b) {
  void *a_buffer = array_get(h->array, a);
  void *b_buffer = array_get(h->array, b);
  if (!a_buffer || !b_buffer)
    return 1;

  array_set(h->array, a, b_buffer);
  array_set(h->array, b, a_buffer);
  return 0;
}

/* Percolates index up untill the compare function says not to.
 *
 */
static void percolate_up(struct heap *h, long index_child) {
  long index_parent = calculate_parent_index(index_child);
  void *parent_buffer = array_get(h->array, index_parent);
  void *child_buffer = array_get(h->array, index_child);
  if (!child_buffer || !parent_buffer)
    return;

  /* If the value of the child is smaller than the parent, swap. */
  if (h->compare(child_buffer, parent_buffer) < 0) {
    swap(h, index_child, index_parent);
    percolate_up(h, index_parent);
  }
}

static int heap_insert(struct heap *h, void *p) {
  if (!h)
    return 1;

  if (array_append(h->array, p)) 
    return 2;

  long index_rightmost = array_size(h->array);
  if (index_rightmost <= 1)
    return 3;

  percolate_up(h, index_rightmost);
  return 0;
}

int prioq_insert(prioq *q, void *p) {
  return heap_insert(q, p);
}

/* Calculates the index of the child of the parent
 *
 * index_parent: a long of the index of the parent.
 * right_child: a boolean value either 0 or 1. Denoting if the left or right child should be returned.
 */
static long calculate_child_index(long index_parent, int right_child) {
  return (index_parent * 2) + right_child;
}


static void percolate_down(struct heap *h, long index) {
  long index_right = calculate_child_index(index, 1);
  long index_left = calculate_child_index(index, 0);
  void *parent_buffer = array_get(h->array, index);
  void *right_buffer = array_get(h->array, index_right);
  void *left_buffer = array_get(h->array, index_left);

  if (!right_buffer && !left_buffer) {
    return; 
  } else if (!right_buffer) {
    if (h->compare(parent_buffer, left_buffer) > 0) {
      swap(h, index, index_left);
      percolate_down(h, index_left);
    }
  } else if (!left_buffer) {
    if (h->compare(parent_buffer, right_buffer) > 0) {
      swap(h, index, index_right);
      percolate_down(h, index_right);
    }
  } else {
    if (h->compare(left_buffer, right_buffer) > 0) {
      /* This means right is smaller, and should be swapped. */
      swap(h, index, index_right);
      percolate_down(h, index_right);
    } else {
      /* This means left is smaller, and should be swapped. */
      swap(h, index, index_left);
      percolate_down(h, index_left);
    }
  }
}

static void *heap_pop(struct heap *h) {
  if (!h)
    return NULL;
  if (heap_size(h) <= 0)
    return NULL;
  void *root = array_get(h->array, 1);
  void *rightmost_buffer = array_pop(h->array);
  array_set(h->array, 1, rightmost_buffer);
  percolate_down(h, 1);
  return root;
}

void *prioq_pop(prioq *q) {
    return heap_pop(q);
}
