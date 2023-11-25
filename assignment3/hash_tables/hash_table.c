#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "hash_table.h"

struct table {
    /* The (simple) array used to index the table */
    struct node **array;
    /* The function used for computing the hash values in this table */
    unsigned long (*hash_func)(const unsigned char *);

    /* Maximum load factor after which the table array should be resized */
    double max_load_factor;
    /* Capacity of the array used to index the table */
    unsigned long capacity;
    /* Current number of elements stored in the table */
    unsigned long load;
};

/* Note: This struct should be a *strong* hint to a specific type of hash table
 * You may implement other options, if you can build them in such a way they
 * pass all tests. However, the other options are generally harder to code. */
struct node {
    /* The string of characters that is the key for this node */
    char *key;
    /* A resizing array, containing the all the integer values for this key */
    struct array *value;

    /* Next pointer */
    struct node *next;
};
/* ... SOME CODE MISSING HERE ... */

/* Initialize a node with a key and a resizable array. */
struct node *node_init(const char *key) {
  struct node *n = malloc(sizeof(struct node));
  if (!n)
    return NULL;
  struct array *value = array_init(100);
  if (!value) {
    free(n);
    return NULL;
  }
  char *key_copy = malloc((strlen(key) + 1) * sizeof(char));
  if (!key_copy) {
    free(n);
    free(value);
    return NULL;
  }
  strcpy(key_copy, key);
  n->key = key_copy;
  n->value = value;
  n->next = NULL;
  return n;
}

/* Cleanup a node together with it's resizable array and key.
 * Does not clean up it's next node!
 */
void node_cleanup(struct node *n) {
  free(n->key);
  array_cleanup(n->value);
  free(n);
}

void node_list_cleanup(struct node *n) {
  if (!n)
    return;
  struct node *node = n;
  while (node) {
    struct node *buffer = node;
    node = node->next;
    node_cleanup(buffer);
  }
}

int node_add_value(struct node *n, int value) {
  if (!n)
    return 1;
  if (array_append(n->value, value))
    return 1;
  assert(array_get(n->value, array_size(n->value) - 1) == value);
  return 0;
}

unsigned long calculate_array_index(unsigned long (*hash_func)(const unsigned char *),
                                    const char *key,
                                    unsigned long mod) {
  return hash_func((const unsigned char*)key) % mod;
}

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {

    /* Note: The 'array' member of struct table is a pointer to a block of
     * memory that contains pointers to struct nodes. Make sure that the struct
     * node pointers in this block are all set to NULL. A good way to do this is
     * with the calloc() function. Check the manual page 'man calloc' for
     * its usage. */

  struct table *t = malloc(sizeof(struct table));
  if (!t)
    return NULL;
  struct node **array = calloc(capacity, sizeof(struct node*));
  if (!array) {
    free(t);
    return NULL;
  }
  t->array = array;
  t->hash_func = hash_func;
  t->max_load_factor = max_load_factor;
  t->capacity = capacity;
  t->load = 0;
  return t;
}

int table_insert(struct table *t, const char *key, int value) {
  if (!t || !key)
    return 1;
  unsigned long array_index = calculate_array_index(t->hash_func, key, t->capacity);
  struct node *node_head = t->array[array_index];
  if (!node_head) {
    struct node *n = node_init(key);
    if (!n)
      return 1;
    node_add_value(n, value);
    t->array[array_index] = n;
  }
  return 0;
}

struct array *table_lookup(const struct table *t, const char *key) {
  if (!t || !key)
    return NULL;
  unsigned long array_index = calculate_array_index(t->hash_func, key, t->capacity);
  struct node *node_head = t->array[array_index];
  return node_head->value;
}

double table_load_factor(const struct table *t) {
    /* ... SOME CODE MISSING HERE ... */
}

int table_delete(struct table *t, const char *key) {
    /* ... SOME CODE MISSING HERE ... */
}

/* Cleans up all node lists inside the array and frees the array */
void table_array_cleanup(struct node **array, unsigned long capacity) {
  for (int i = 0; i < (int)capacity; i++) {
    node_list_cleanup(array[i]);
  }
  free(array);
}

void table_cleanup(struct table *t) {
  table_array_cleanup(t->array, t->capacity);
  free(t);
}
