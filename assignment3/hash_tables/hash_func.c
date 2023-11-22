#include "hash_func.h"

/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long) *str;
}

