#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "prioq.h"

static struct heap *heap_init(int (*compare)(const void *, const void *)) {
    /* ... CODE MISSING HERE .... */

    /* Store the function pointer compare argument in the struct here.
       If the struct pointer is called 'h' the assignment is:
       h->compare = compare; */

    /* ... CODE MISSING HERE .... */
}

prioq *prioq_init(int (*compare)(const void *, const void *)) {
    return heap_init(compare);
}

long int prioq_size(const prioq *q) {
    /* ... CODE MISSING HERE .... */
}

static int heap_cleanup(struct heap *h, void free_func(void *)) {
    /* ... CODE MISSING HERE .... */
}

int prioq_cleanup(prioq *q, void free_func(void *)) {
    /* ... CODE MISSING HERE .... */
}

static int heap_insert(struct heap *h, void *p) {
    /* ... CODE MISSING HERE .... */

    /* To compare items in the heap these heaps functions can use the stored
       compare function pointer in the following way:
       if (h->compare(arg1, arg1)) { ...  */
}

int prioq_insert(prioq *q, void *p) {
    return heap_insert(q, p);
}

static void *heap_pop(struct heap *h) {
    /* ... CODE MISSING HERE .... */
}

void *prioq_pop(prioq *q) {
    return heap_pop(q);
}
