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

void print_array_values(struct array *a) {
  if (a) {
    unsigned long size = array_size(a);
    for (int i = 0; i < size; i++) {
      printf("%d, ", array_get(a, i));
    }
  }
  printf("\n");
}

void print_node(struct node *n) {
  if (n) {
    printf("node: %p, next: %p, key: %s, values: ", (void*)(0xFFFF & (unsigned long)n),
           (void*)(0xFFFF & (unsigned long)n->next), n->key);
    print_array_values(n->value);
  } else {
    printf("node: %p, next: -, key: -, values: -", (void*)n);
  }
}

void print_bucket(struct node *n) {
  struct node *node = n;
  int tabs_amount = 0;
  while (node) {
    for (int i = 0; i < tabs_amount; i++) {
      printf("\t");
    }
    print_node(node);
    node = node->next;
    tabs_amount++;
  }
}

void print_table(struct table *t) {
  for (int i = 0; i < (int)t->capacity; i++) {
    print_bucket(t->array[i]);
  }
}

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

struct node *node_tail(struct node *n) {
  if (!n)
    return NULL;
  struct node *node;
  for (node = n; node->next; node = node->next);
  return node;
}

int node_add_value(struct node *n, int value) {
  if (!n)
    return 1;
  if (array_append(n->value, value))
    return 1;
  assert(array_get(n->value, array_size(n->value) - 1) == value);
  return 0;
}

struct node *node_find_key(struct node *n, const char *key) {
  if (!n || !key)
    return NULL;
  struct node *node = n;
  for (node = n; node && strcmp(key, node->key); node = node->next);
  return node;
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
  struct node *n_head = t->array[array_index];
  if (!n_head) {
    /* This means that the bucket is still empty. */
    struct node *n = node_init(key);
    if (!n)
      return 1;
    node_add_value(n, value);
    t->array[array_index] = n;
  } else {
    /* This means that either the node has the same key or does not. */
    struct node *n = node_find_key(n_head, key);
    if (n) {
      /* The node with the same key exists. */
      node_add_value(n, value);
    } else {
      /* The node with the same key does not exist. */
      n = node_init(key);
      if (!n)
        return 1;
      node_add_value(n, value);
      struct node *n_tail = node_tail(n_head);
      n_tail->next = n;
    }
  }
  return 0;
}

struct array *table_lookup(const struct table *t, const char *key) {
  if (!t || !key)
    return NULL;
  unsigned long array_index = calculate_array_index(t->hash_func, key, t->capacity);
  struct node *node_head = t->array[array_index];
  struct node *node = node_find_key(node_head, key);
  if (!node)
    return NULL;
  return node->value;
}

double table_load_factor(const struct table *t) {
    /* ... SOME CODE MISSING HERE ... */
}

int table_delete(struct table *t, const char *key) {

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
