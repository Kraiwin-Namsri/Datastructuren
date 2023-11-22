#include <stdlib.h>

#include "array.h"

struct array {
    /* ... SOME CODE MISSING HERE ... */
};

struct array *array_init(unsigned long initial_capacity) {
    /* ... SOME CODE MISSING HERE ... */
}

void array_cleanup(struct array *a) {
    /* ... SOME CODE MISSING HERE ... */
}

int array_get(const struct array *a, unsigned long index) {
    /* ... SOME CODE MISSING HERE ... */
}

/* Note: Although this operation might require the array to be resized and
 * copied, in order to make room for the added element, it is possible to do
 * this in such a way that the amortized complexity is still O(1).
 * Make sure your code is implemented in such a way to guarantee this. */
int array_append(struct array *a, int elem) {
    /* ... SOME CODE MISSING HERE ... */
}

unsigned long array_size(const struct array *a) {
    /* ... SOME CODE MISSING HERE ... */
}
