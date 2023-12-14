/* Name: Kraiwin Namsri
 * UvAID: 15032094
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

/* C files can be modified anywhere.
 * So you can change or remove these structs to suit your needs. */
struct tree {
  struct node *root;
  int turbo;
};

struct node {
  int data;
  struct node *lhs;
  struct node *rhs;
  unsigned long height;
  unsigned long rhs_length;
  unsigned long lhs_length;
};
typedef struct node node;

/* Unique id's for numbering nodes in dot format. */
static int global_node_counter = 0;

/* Helper function: Allocate a new tree node and initialise it with
 * the given parameters. Return a pointer to the new node or NULL on
 * failure. */
static node *make_node(int data){
  node *n = malloc(sizeof(struct node));
  if (!n)
    return NULL;

  n->data = data;
  n->lhs = NULL;
  n->rhs = NULL;
  n->height = 0;
  n->rhs_length = 0;
  n->lhs_length = 0;
  return n;
}

long max(long x, long y) {
  return x > y ? x : y;
}

static int print_tree_dot_r(node *root, FILE *dotf) {
  int left_id, right_id, my_id = global_node_counter++;

  if (root == NULL) {
    fprintf(dotf, "    %d [shape=point];\n", my_id);
    return my_id;
  }

  fprintf(dotf, "    %d [color=%s label=\"d: %d\nr: %lu\n l: %lu\"]\n", my_id, "black", root->data, root->rhs_length, root->lhs_length);

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

int node_run_forevery(node *n, int(*f)(node *n)) {
  if (!n)
    return 0;

  if (f(n))
    return 1;

  int ret = 0;
  ret = ret || node_run_forevery(n->rhs, f);
  ret = ret || node_run_forevery(n->lhs, f);
  return ret;
}

int node_check_height(node *n) {
  if (!n)
    return 0;

  if (n->rhs && (n->height + 1 != n->rhs->height)) {
    return 1;
  }

  if (n->lhs && (n->height + 1 != n->lhs->height)) {
    return 1;
  }

  return 0;
}

int node_check_BST_property(node *n) {
  if (!n)
    return 0;

  if (n->rhs && (n->data > n->rhs->data))
    return 1;

  if (n->lhs && (n->data < n->lhs->data))
    return 1;

  return 0;
}

int node_check_length(node *n) {
  if (!n)
    return 0;

  if (n->rhs && n->rhs_length != (unsigned long)max((long)n->rhs->rhs_length, (long)n->rhs->lhs_length) + 1){
    return 1;
  }

  if (!n->rhs && n->rhs_length != 0) {
    return 1;
  }

  if (n->lhs && n->lhs_length != (unsigned long)max((long)n->lhs->rhs_length, (long)n->lhs->lhs_length) + 1){
    return 1;
  }

  if (!n->lhs && n->lhs_length != 0) {
    return 1;
  }
  
  return 0;
}

int tree_check(const struct tree *tree) {
  int ret = 0;
  ret = ret || node_run_forevery(tree->root, node_check_height); 
  ret = ret || node_run_forevery(tree->root, node_check_BST_property);
  ret = ret || node_run_forevery(tree->root, node_check_length);
  return ret;
}

struct tree *tree_init(int turbo) {
  struct tree* tree = malloc(sizeof(struct tree));
  if (!tree) 
    return NULL;

  tree->turbo = turbo;
  tree->root = NULL;
  return tree;
}

/* Sets all the heights of a tree recursivly. 
 */
static int recalc_tree_height(node *n, unsigned long height) {
  if (!n)
    return -1;

  n->height = height;
  return (int)max((long)recalc_tree_height(n->rhs, height + 1),
                  (long)recalc_tree_height(n->lhs, height + 1));
}

/* This function is for debugging only!
 * Gets the heights of a node recursivly. */
static int get_tree_height(node *n) {
  if (!n)
    return -1;

  return (int)max((long)get_tree_height(n->rhs), (long)get_tree_height(n->lhs));
}

/* Calculates the balance of a tree, with n being a root.
 * Returns:
 *  -1 on a left-heavy tree.
 *  0 on a balanced tree.
 *  1 on a right-heavy tree.
 * */
static int calculate_tree_balance(node *n) {

}

/* Rebalances the tree. */
static int tree_balance(node *n) {

}


/* Recursivly finds the spot where to insert m, and inserts.
 * n: the parent.
 * m: the child to insert.
 * height: a pointer to the height of the branch.
 * Returns -1 on error, 0 on succes, 1 if a node with the same value already exists. 
 */
static int node_insert(node *n, node *m, unsigned long *length) {
  if (!m || !n)
    return -1;

  if (m->data == n->data)
    return 1;
  
  int ret;
  m->height = n->height + 1;
  if (m->data > n->data) {
    if (!n->rhs){
      n->rhs = m;
      n->rhs_length = (unsigned long)max((long)m->rhs_length, (long)m->lhs_length) + 1;
      ret = 0;
    } else {
      ret = node_insert(n->rhs, m, &(n->rhs_length));
    }

  } else {
    if (!n->lhs) {
      n->lhs = m;
      n->lhs_length = (unsigned long)max((long)m->rhs_length, (long)m->lhs_length) + 1;
      ret = 0;
    } else {
      ret = node_insert(n->lhs, m, &(n->lhs_length));
    }

  }

  if (length) {
    *length = (unsigned long)max((long)n->rhs_length, (long)n->lhs_length) + 1;
  }

  return ret;
}

int tree_insert(struct tree *tree, int data) {
  if (!tree)
    return -1;

  node *m = make_node(data);
  if (!m)
    return -1;
  
  if (!tree->root) {
    tree->root = m;
    return 0;
  }

  // unsigned long *length = data > tree->root->data ? &(tree->root->rhs_length) : &(tree->root->rhs_length);
  int ret = node_insert(tree->root, m, NULL);
  if (ret)
    free(m);

  return ret;
}

/* Recursivly moves through the branches of the tree to find node m
 * Returns 1 if the data is found, 0 if it is not found.
 * n: the parent
 * data: the data to find.
 */
static int node_find(node *n, int data) {
  if (!n)
    return 0;

  if (n->data == data)
    return 1;

  if (data > n->data) {
    if (!n->rhs)
      return 0;

    return node_find(n->rhs, data);
  } else {
    if (!n->lhs)
      return 0;
    
    return node_find(n->lhs, data);
  }
}

int tree_find(struct tree *tree, int data) {
  if (!tree)
    return -1;

  return node_find(tree->root, data);
}

static node *node_unlink_rightmost(node **n) {
  if (!n || !(*n))
    return NULL;

  node **rightmost = n;
  while ((*rightmost)->rhs) {
    rightmost = &(*rightmost)->rhs;
  }

  *rightmost = NULL;
  return *rightmost;
}

/* Recursivly removes the node n
 * Sets n to the value.
 */
static int node_remove(node **n, int data) {
  if (!n || !*n)
    return 1;

  if ((*n)->data == data) {
    /* We found the correct node to remove. */

    if (!(*n)->rhs && !(*n)->lhs) {
      /* This means that n is a leaf. */
      free(*n);
      *n = NULL;
    } else if (!((*n)->rhs && (*n)->lhs)) {
      /* This means that there is only 1 child. */
      node *child = (*n)->rhs ? (*n)->rhs : (*n)->lhs;
      recalc_tree_height(child, (*n)->height);
      free(*n);
      *n = child;
    } else {
      /* This means that n has 2 childs. */
      node *rightmost = node_unlink_rightmost(&(*n)->lhs);
      recalc_tree_height(rightmost, (*n)->height);
      free(*n);
      *n = rightmost;
    }
    return 0;
  }

  /* We have not found the correct node to remove yet. */
  if (data > (*n)->data) {
    return node_remove(&(*n)->rhs, data);
  } else {
    return node_remove(&(*n)->lhs, data);
  }
}

int tree_remove(struct tree *tree, int data) {
  if (!tree)
    return -1;
  
  return node_remove(&tree->root, data);
}

void node_in_order_traversal(node *n) {
  if (!n)
    return;

  node_in_order_traversal(n->lhs);
  printf("%d\n", n->data);
  node_in_order_traversal(n->rhs);
}

void tree_print(const struct tree *tree) {
  if (!tree)
    return;

  node_in_order_traversal(tree->root);
}

/* Cleans the left and right subtree of n 
 */
void clean_subtree(node *n) {
  if (!n)
    return;

  if (n->lhs) {
    clean_subtree(n->lhs);
  }

  if (n->rhs) {
    clean_subtree(n->rhs);
  }

  free(n);
}

void tree_cleanup(struct tree *tree) {
  clean_subtree(tree->root);
  free(tree);
}
