/* Name: Kraiwin Namsri
 * UvAnetID: 15032094@uva.nl
 * Study: BSc Informatica
 *
 * This is an implementation of a resizable array. 
 * For more detailed function descriptions please read the corresponding header file.
 */

#include <stdlib.h>

#include "array.h"

struct array {
  int *data;
  unsigned long capacity;
  unsigned long num_elm;
};

/* Doubles the capacity of the array.
 * Return-Value:
 *   Returns 0 on succes, 1 on failure.
 */
int array_resize(struct array *a) {
  int *data = realloc(a->data, (a->capacity * 2) * sizeof(int));
  if (!data)
    return 1;

  a->capacity = a->capacity * 2;
  a->data = data;
  return 0;
}

struct array *array_init(unsigned long initial_capacity) {
  struct array *a = malloc(sizeof(struct array));
  if (!a)
    return NULL;

  int *data = malloc(sizeof(int) * initial_capacity);
  if (!data) {
    free(a);
    return NULL;
  }

  a->data = data;
  a->capacity = initial_capacity;
  a->num_elm = 0;
  return a;
}

void array_cleanup(struct array *a) {
  if (!a)
    return;

  free(a->data);
  free(a);
}

int array_get(const struct array *a, unsigned long index) {
  if (!a)
    return -1;

  if (index >= a->num_elm)
    return -1;

  return a->data[index];
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
  if (!a)
    return 1;

  if (a->num_elm >= a->capacity)
    array_resize(a);

  a->data[a->num_elm] = elem;
  a->num_elm++;
  return 0; 
}

unsigned long array_size(const struct array *a) {
  if (!a)
    return 0;

  return a->num_elm;
}
