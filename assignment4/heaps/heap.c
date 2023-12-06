#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
  if (!compare)
    return NULL;

  struct heap *h = malloc(sizeof(struct heap));
  if (!h)
    return NULL;

  struct array *array = array_init(50);
  if (!array) {
    free(h);
    return NULL;
  }

  array_append(array, NULL);
  assert(array_size(array) == 1);

  h->array = array;
  h->compare = compare;
  return h;
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(const prioq *q) {
  if (!q)
    return -1;

  return array_size(q->array) - 1;
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
  if (!h)
    return -1;

  array_cleanup(h->array, free_func);
  free(h);
  return 0;
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
  return heap_cleanup(q, free_func);
}

/* Swap index a with b inside the array of h. 
 * Returns 0 on succes, 1 on failure.
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

/* A recursive function for percolating up the value inside index. 
 * Returns 0 on succes, something else on failure.
 */
static int percolate_up(struct heap *h, long child_index) {
  if (!h || child_index < 1)
    return 1;

  void *child = array_get(h->array, child_index);
  if (!child)
    return 3;

  long parent_index = child_index / 2;
  void *parent = array_get(h->array, parent_index);

  /* There is no parent anymore, indicating that percolating up is done. */
  if (!parent)
    return 0;

  /* If the value of the child is smaller than the parent, swap. */
  if (h->compare(child, parent) < 0) {
    if (swap(h, child_index, parent_index))
      return 4;
    /* Continue percolating up with the same node.
       (Which is now at the index of the previous parent) */
    return percolate_up(h, parent_index);
  }

  /* The child is bigger than parent, so we are done! */
  return 0;
}

static int heap_insert(struct heap *h, void *p) {
  if (!h)
    return 1;
  if (array_append(h->array, p))
    return 2;
  return percolate_up(h, array_size(h->array) - 1);
}

int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);
}

/* Swaps a with b if a > b.
 * Returns 1 if swapped, 0 if not.
 */
static int swap_if_greater(struct heap *h, long a_index, long b_index) {
  void *a = array_get(h->array, a_index);
  void *b = array_get(h->array, b_index);
  int should_swap = h->compare(a, b) > 0;
  if (should_swap)
    swap(h, a_index, b_index);
  return should_swap;
}

/* A recursive function that percolates down the value in parent_index 
 * Returns 0 on succes, something else on failure.
 */
static int percolate_down(struct heap *h, long parent_index) {
  if (!h || parent_index < 1)
    return 1;
  
  long l_child_index = parent_index * 2;
  long r_child_index = l_child_index + 1;
  void *l_child = array_get(h->array, l_child_index);
  void *r_child = array_get(h->array, r_child_index);

  /* If both childs are NULL then this means that percolating down is done. */
  if (!l_child && !r_child) {
    return 0;

  } else if (!l_child) {
    /* Only the right child exists */
    if (!swap_if_greater(h, parent_index, r_child_index))
      return 0;

    /* Continue percolating down with the same node */
    return percolate_down(h, r_child_index);

  } else if (!r_child) {
    /* Only the left child exists */
    if (!swap_if_greater(h, parent_index, l_child_index))
      return 0;

    /* Continue percolating down with the same node */
    return percolate_down(h, l_child_index);

  } else {
    /* This means that both the right and left child exists. */

    /* If the parent should be swapped it must be with it's largest child. */
    if (h->compare(l_child, r_child) > 0) {
      if (!swap_if_greater(h, parent_index, l_child_index))
        return 0;
      /* This means left is larger */

      /* Continue percolating down with the same node */
      return percolate_down(h, l_child_index);
    } else {
      /* This means right is larger */
      if (!swap_if_greater(h, parent_index, r_child_index))
        return 0;

      /* Continue percolating down with the same node */
      return percolate_down(h, r_child_index);
    }
  }
}

static void *heap_pop(struct heap *h) {
  if (!h)
    return NULL;

  long leaf_index = array_size(h->array) - 1;
  if (leaf_index < 0)
    return NULL;

  /* Removing the root of the array creates a hole */
  void *root = array_get(h->array, 1);
  void *leaf = array_get(h->array, leaf_index);

  /* Fix the hole by moving the rightmost leaf to the rootposition */
  array_set(h->array, 1, leaf);

  /* Percolate the root down to restore the heap property */
  int ret = percolate_down(h, 1);
  if (ret)
    return NULL;

  return root;
}

void *prioq_pop(prioq *q) {
    return heap_pop(q);
}
