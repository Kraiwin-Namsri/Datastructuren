#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

struct stack {
  size_t length;
  size_t pops;
  size_t pushes;
  size_t max_length;
  size_t capacity;
  size_t resize_amount;
  unsigned int *data;
};

struct stack *stack_init(size_t capacity) {
  struct stack *s = malloc(sizeof(struct stack));
  if (!s) return NULL;
  unsigned int *data = malloc(capacity * sizeof(unsigned int));
  if (!data) {
    free(s);
    return NULL;
  }
  s->length = 0;
  s->max_length = 0;
  s->data = data;
  s->resize_amount = capacity + 1;
  s->capacity = capacity;
  s->pops = 0;
  s->pushes = 0;
  return s;
}
int stack_resize(struct stack *s) {
  unsigned int *data = realloc(s->data, sizeof(unsigned int) * (s->capacity + s->resize_amount));
  if (!data){
    return 0;
  }
  s->data = data;
  s->capacity += s->resize_amount;
  return 1;
}

void stack_cleanup(struct stack *s) {
  if (!s) return;
  free(s->data);
  free(s);
}

void stack_stats(const struct stack *s) {
  if (!s) return;
  fprintf(stderr, "stats %lu %lu %lu\n", s->pushes, s->pops, s->max_length);
}

int stack_push(struct stack *s, int c) {
  if (!s) return 1;
  if (s->length >= s->capacity) {
    if (!stack_resize(s))
      return 1;
  }
  s->data[s->length++] = (unsigned int)c;
  if (s->length > s->max_length) {
    s->max_length = s->length;
  }
  s->pushes++;
  return 0;
}

int stack_pop(struct stack *s) {
  if (!s) return -1;
  if (!s->length) return -1;
  s->pops++;
  return (int)s->data[--(s->length)];
}

int stack_peek(const struct stack *s) {
  if (!s) return -1;
  if (s->length == 0) {
    return -1;
  }
  return (int)s->data[s->length - 1];
}

int stack_empty(const struct stack *s) {
  if (!s) return -1;
  return !s->length;
}

size_t stack_size(const struct stack *s) {
  if (!s) return 0;
  return s->length;
}
