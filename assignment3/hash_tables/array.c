#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

struct array {
  int *data;
  size_t capacity;
  size_t n_elements;
};

struct array *array_init(unsigned long initial_capacity) {
  struct array *a = malloc(sizeof(struct array));
  if (!a)
    return NULL;
  int *data = malloc(initial_capacity * sizeof(int));
  if (!data) {
    free(a);
    return NULL;
  }
  a->data = data;
  a->capacity = initial_capacity;
  a->n_elements = 0;
  return a;
}

void array_cleanup(struct array *a) {
  free(a->data);
  free(a);
}

int array_get(const struct array *a, unsigned long index) {
  if (!a)
    return -1;
  if (index >= a->n_elements)
    return -1;
  return a->data[index];
}
/* Returns 0 on succes, 1 on not.
 *
 */
int array_resize(struct array *a) {
  int *data = realloc(a->data, (a->capacity *= 2) * sizeof(int));
  if (!data)
    return 1;
  a->data = data;
  return 0;
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
  if (a->n_elements >= a->capacity)
    if (array_resize(a))
      return 1;
  a->data[a->n_elements++] = elem;
  return 0;
}

unsigned long array_size(const struct array *a) {
  if (!a)
    return NULL;
  return a->n_elements;
}
