#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

struct queue {
  unsigned int *data;
  size_t front;
  size_t rear;
  size_t length;
  size_t capacity;
  size_t pushes;
  size_t pops;
  size_t max_length; 
};

struct queue *queue_init(size_t capacity) {
  struct queue *q = malloc(sizeof(struct queue));
  if (!q) return NULL;
  unsigned int *data = malloc(capacity * sizeof(unsigned int));
  if (!data) {
    free(q);
    return NULL;
  }
  q->front = 0;
  q->rear = 0;
  q->length = 0;
  q->pushes = 0;
  q->pops = 0;
  q-> max_length = 0;
  q->capacity = capacity;
  q->data = data;
  return q;
}

void queue_cleanup(struct queue *q) {
  free(q->data);
  free(q);
}

void queue_stats(const struct queue *q) {
  fprintf(stderr, "stats %lu %lu %lu", q->pushes, q->pops, q->max_length);
}

int queue_push(struct queue *q, int e) {
  if (!q) return 1;
  if (q->length >= q->capacity) return 1;
  q->pushes++;
  q->data[(q->rear++) % q->capacity] = (unsigned int)e;
  q->length++;
  if (q->length > q->max_length) 
    q->max_length = q->length;
  return 0;
}

int queue_pop(struct queue *q) {
  if (!q) return -1;
  if (!q->length) return -1;
  q->pops++;
  q->length--;
  return (int)q->data[q->front++ % q->capacity];
}

int queue_peek(const struct queue *q) {
  if (!q) return -1;
  if (!q->length) return -1;
  return (int)q->data[q->front % q->capacity];
}

int queue_empty(const struct queue *q) {
  if (!q) return -1;
  return !q->length;
}

size_t queue_size(const struct queue *q) {
  return q->length;
}
