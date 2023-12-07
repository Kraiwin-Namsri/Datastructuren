#include "hash_func.h"

/* Do not edit this function, as it used in testing too
 * Add you own hash functions with different headers instead. */
unsigned long hash_too_simple(const unsigned char *str) {
    return (unsigned long) *str;
}

/* Note: I did not use this function */
unsigned long my_hash(const unsigned char *str) {
  /* Source: https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f */
  unsigned long hash = 5381;
  unsigned long c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}
