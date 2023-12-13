#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
    struct node *root;
};

struct node {
    int data;
    struct node *lhs;
    struct node *rhs;

    /* ... SOME CODE MISSING HERE ... */
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(/* .. */) {
    /* ... SOME CODE MISSING HERE ... */
}

static int print_tree_dot_r(node *root, FILE *dotf) {
    int left_id, right_id, my_id = global_node_counter++;

    if (root == NULL) {
        fprintf(dotf, "    %d [shape=point];\n", my_id);
        return my_id;
    }

    fprintf(dotf, "    %d [color=%s label=\"%d\"]\n", my_id, "black", root->data);

    left_id = print_tree_dot_r(root->lhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"l\"]\n", my_id, left_id);

    right_id = print_tree_dot_r(root->rhs, dotf);
    fprintf(dotf, "    %d -> %d [label=\"r\"]\n", my_id, right_id);

    return my_id;
}

void tree_dot(const struct tree *tree, const char *filename) {
    node *root = tree->root;
    global_node_counter = 0;
    FILE *dotf = fopen(filename, "w");
    if (!dotf) {
        printf("error opening file: %s\n", filename);
        exit(1);
    }
    fprintf(dotf, "digraph {\n");
    if (root) {
        print_tree_dot_r(root, dotf);
    }
    fprintf(dotf, "}\n");
    fclose(dotf);
}

int tree_check(const struct tree *tree) {
    /* ... OPTIONALLY IMPLEMENT TREE CHECKING HERE ... */
    return 0;
}

struct tree *tree_init(int turbo) {
    /* ... SOME CODE MISSING HERE ... */
}

int tree_insert(struct tree *tree, int data) {
    /* ... SOME CODE MISSING HERE ... */
}

int tree_find(struct tree *tree, int data) {
    /* ... SOME CODE MISSING HERE ... */
}

int tree_remove(struct tree *tree, int data) {
    /* ... SOME CODE MISSING HERE ... */
}

void tree_print(const struct tree *tree) {
    /* ... SOME CODE MISSING HERE ... */
}

void tree_cleanup(struct tree *tree) {
    /* ... SOME CODE MISSING HERE ... */
}
