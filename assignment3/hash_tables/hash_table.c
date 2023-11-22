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

struct table *table_init(unsigned long capacity,
                         double max_load_factor,
                         unsigned long (*hash_func)(const unsigned char *)) {

    /* Note: The 'array' member of struct table is a pointer to a block of
     * memory that contains pointers to struct nodes. Make sure that the struct
     * node pointers in this block are all set to NULL. A good way to do this is
     * with the calloc() function. Check the manual page 'man calloc' for
     * its usage. */

    /* ... SOME CODE MISSING HERE ... */
}

int table_insert(struct table *t, const char *key, int value) {
    /* ... SOME CODE MISSING HERE ... */
}

struct array *table_lookup(const struct table *t, const char *key) {
    /* ... SOME CODE MISSING HERE ... */
}

double table_load_factor(const struct table *t) {
    /* ... SOME CODE MISSING HERE ... */
}

int table_delete(struct table *t, const char *key) {
    /* ... SOME CODE MISSING HERE ... */
}

void table_cleanup(struct table *t) {
    /* ... SOME CODE MISSING HERE ... */
}
