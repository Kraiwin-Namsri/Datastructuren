/* Name: Kraiwin Namsri
 * UvAID: 15032094
 *
 * This is an implementation of a tree:
 * I tried to implement AVL tree, but not finished: https://en.wikipedia.org/wiki/AVL_tree
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tree.h"

#define MAX_DELTA_LENGTH 1
// #define DEBUG

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

/* Returns the max of x and y (Could be replaced with a MACRO) */
static long max(long x, long y) {
  return x > y ? x : y;
}

static int print_tree_dot_r(node *root, FILE *dotf) {
  int left_id, right_id, my_id = global_node_counter++;

  if (root == NULL) {
    fprintf(dotf, "    %d [shape=point];\n", my_id);
    return my_id;
  }

  fprintf(dotf, "    %d [color=%s label=\"d: %d\nl: %lu r: %lu\"]\n", my_id, "black", root->data, root->lhs_length, root->rhs_length);

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

/* A function that runs a functions for every node */
static int node_run_forevery(node *n, int(*f)(node *n)) {
  if (!n)
    return 0;

  int f_ret = f(n);
  if (f_ret)
    return f_ret;

  int ret = 0;
  ret = ret || node_run_forevery(n->rhs, f);
  ret = ret || node_run_forevery(n->lhs, f);
  return ret;
}

/* A test function that checks the height of a node and its children. */
static int node_check_height(node *n) {
  if (!n)
    return 0;

  if (n->rhs && (n->height + 1 != n->rhs->height))
    return 1;
  if (n->lhs && (n->height + 1 != n->lhs->height))

    return 1;

  return 0;
}

/* A test function that checkt the BST property of a node and its children. */
static int node_check_BST_property(node *n) {
  if (!n)
    return 0;

  if (n->rhs && (n->data > n->rhs->data))
    return 1;

  if (n->lhs && (n->data < n->lhs->data))
    return 1;

  return 0;
}

/* A test function that check if the distance to the leaf is correct. */
static int node_check_length(node *n) {
  if (!n)
    return 0;

  if (n->rhs && n->rhs_length != (unsigned long)max((long)n->rhs->rhs_length, (long)n->rhs->lhs_length) + 1)
    return 1;

  if (!n->rhs && n->rhs_length != 0)
    return 1;

  if (n->lhs && n->lhs_length != (unsigned long)max((long)n->lhs->rhs_length, (long)n->lhs->lhs_length) + 1)
    return 1;

  if (!n->lhs && n->lhs_length != 0)
    return 1;
  
  return 0;
}


/* Rotate a n and n->lhs to the right*/
void node_rotate_right(node **n) {
  node *lhs_child = (*n)->rhs;
  assert(lhs_child);
  (*n)->lhs = lhs_child->rhs;
  lhs_child->rhs = *n;
  *n = lhs_child;
}

/* Rotate n and n->rhs to the left */
void node_rotate_left(node **n) {
  if (n || *n)
    return;

  node *rhs_child = (*n)->rhs;
  assert(rhs_child);
  (*n)->rhs = rhs_child->lhs;
  rhs_child->lhs = *n;
  *n = rhs_child;
}

#ifdef DEBUG
/* A funtion that returns the difference between the rhs en lhs length. */
static long calculate_node_balance(node *n) {
  return (long)n->rhs_length - (long)n->lhs_length;
}
#endif

#ifdef DEBUG
/* A test function that check the AVL property of node n and its children. */
static int node_check_AVL_property(node *n) {
  if (!n)
    return 0;

  if (calculate_node_balance(n) > MAX_DELTA_LENGTH)
    return 1;

  return 0;
}
#endif

int tree_check(const struct tree *tree) {
#ifdef DEBUG
  static int check_rotate = 1;
  if (check_rotate) {
    check_rotate = 0;
    struct tree *t = tree_init(0);
    if (!t)
      return -1;
    for (int i = 0; i < 6; i += 2) {
      tree_insert(t, i);
    }
    tree_insert(t, 1);
    tree_insert(t, -1);

    node_rotate_right(&(t->root));

    tree_dot(t, "tree.dot");

    tree_cleanup(t);
  }
#endif

  if (node_run_forevery(tree->root, node_check_height))
    return 1;
  if (node_run_forevery(tree->root, node_check_BST_property))
    return 2;
  if (node_run_forevery(tree->root, node_check_length))
    return 3;
#ifdef DEBUG
  if (node_run_forevery(tree->root, node_check_AVL_property)) {
    return 4;
  }
#endif
  return 0;
}

struct tree *tree_init(int turbo) {
  struct tree* tree = malloc(sizeof(struct tree));
  if (!tree) 
    return NULL;

  tree->turbo = turbo;
  tree->root = NULL;
  return tree;
}


/* Sets all the heights of a tree recursivly. */
static int recalc_tree_height(node *n, unsigned long height) {
  if (!n)
    return -1;

  n->height = height;
  return (int)max((long)recalc_tree_height(n->rhs, height + 1),
                  (long)recalc_tree_height(n->lhs, height + 1));
}

#ifdef DEBUG
/* This function is for debugging only!
 * Gets the heights of a node recursivly. */
static int get_tree_height(node *n) {
  if (!n)
    return -1;

  return (int)max((long)get_tree_height(n->rhs), (long)get_tree_height(n->lhs));
}
#endif 

/* Calculates the balance of a tree, with n being a root.
 * Returns:
 *  <0 on a left-heavy tree.
 *  0 on a balanced tree.
 *  >0 on a right-heavy tree.
 */


#ifdef DEBUG
/* Rebalances the tree. */
static void node_balance(node **n) {
  long balance = calculate_node_balance(*n);
  if (balance > MAX_DELTA_LENGTH) {
    node_rotate_left(n);
  } else if (balance < -MAX_DELTA_LENGTH) {
    node_rotate_right(n);
  }
}
#endif /* ifdef DEBUG */

/* Recursivly finds the spot where to insert m, and inserts. At the same time it updates heights and lengths.
 * n: the parent.
 * m: the child to insert.
 * length: a pointer to the length till the deepest leaf of the branch.
 * Returns -1 on error, 0 on succes, 1 if a node with the same value already exists. 
 */
static int node_insert(node **n, node *m, unsigned long *length) {
  if (!m || !n)
    return -1;

  if (m->data == (*n)->data)
    return 1;
  
  int ret;
  m->height = (*n)->height + 1;
  if (m->data > (*n)->data) {
    if (!(*n)->rhs){
      (*n)->rhs = m;
      (*n)->rhs_length = (unsigned long)max((long)m->rhs_length, (long)m->lhs_length) + 1;
      ret = 0;
    } else {
      ret = node_insert(&(*n)->rhs, m, &(*n)->rhs_length);
    }

  } else {
    if (!(*n)->lhs) {
      (*n)->lhs = m;
      (*n)->lhs_length = (unsigned long)max((long)m->rhs_length, (long)m->lhs_length) + 1;
      ret = 0;
    } else {
      ret = node_insert(&(*n)->lhs, m, &(*n)->lhs_length);
    }

  }

  if (length) {
    *length = (unsigned long)max((long)(*n)->rhs_length, (long)(*n)->lhs_length) + 1;
  }

#ifdef DEBUG
  node_balance(n);
#endif

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

  int ret = node_insert(&(tree->root), m, NULL);
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

/* Recursivly find the rightmost node and unlink it */
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
 * n: A pointer to the pointernode of the root.
 * data: the data to remove.
 * length: A pointer to the length of the previous node (Recursive previous).
 */
static int node_remove(node **n, int data, unsigned long *length) {
  if (!n || !(*n))
    return 1;

  if ((*n)->data == data) {
    /* We found the correct node to remove. */

    if (!(*n)->rhs && !(*n)->lhs) {
      /* This means that n is a leaf. */
      *length = (unsigned long)max((long)(*n)->rhs_length, (long)(*n)->lhs_length);
      free(*n);
      *n = NULL;
    } else if (!((*n)->rhs && (*n)->lhs)) {
      /* This means that there is only 1 child. */
      node *child = (*n)->rhs ? (*n)->rhs : (*n)->lhs;
      recalc_tree_height(child, (*n)->height);
      *length = (unsigned long)max((long)(*n)->rhs_length, (long)(*n)->lhs_length);
      free(*n);
      *n = child;
    } else {
      /* This means that n has 2 childs. */
      node *rightmost = node_unlink_rightmost(&(*n)->lhs);
      recalc_tree_height(rightmost, (*n)->height);
      *length = (unsigned long)max((long)(*n)->rhs_length, (long)(*n)->lhs_length);
      free(*n);
      *n = rightmost;
    }

    return 0;
  }

  /* We have not found the correct node to remove yet. */
  int ret;
  if (data > (*n)->data) {
    ret = node_remove(&(*n)->rhs, data, &(*n)->rhs_length);
  } else {
    ret = node_remove(&(*n)->lhs, data, &(*n)->lhs_length);
  }

  if (length != &((*n)->rhs_length) && length != &((*n)->lhs_length))
    *length = (unsigned long)max((long)(*n)->rhs_length, (long)(*n)->lhs_length) + 1;

  return ret;
}

int tree_remove(struct tree *tree, int data) {
  if (!tree)
    return -1;
  
  if (!tree->root)
    return 1;

  unsigned long *length = data > tree->root->data ? &(tree->root->rhs_length) : &(tree->root->lhs_length);
  return node_remove(&tree->root, data, length);
}

/* Traverse nodes in order recursivly and print the data to stdout */
static void node_in_order_traversal(node *n) {
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

/* Cleans the left and right subtree of n */
static void clean_subtree(node *n) {
  if (!n)
    return;

  if (n->lhs)
    clean_subtree(n->lhs);

  if (n->rhs)
    clean_subtree(n->rhs);

  free(n);
}

void tree_cleanup(struct tree *tree) {
  clean_subtree(tree->root);
  free(tree);
}
