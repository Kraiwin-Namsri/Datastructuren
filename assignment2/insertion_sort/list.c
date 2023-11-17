#include "list.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * TODO: A lot of code missing here. You will need to add implementations for
 * all the functions described in list.h here.
 *
 * Start by adding the definitions for the list and node structs. You may
 * implement any of the Linked List versions discussed in the lecture, which
 * have some different trade-offs for the functions you will need to write.
 *
 * Note: The function prototypes in list.h assume the most basic Singly Linked
 * List. If you implement some other version, you may not need all of the function
 * arguments for all of the described functions. This will produce a warning,
 * which you can suppress by adding a simple if-statement to check the value
 * of the unused parameter.
 *
 * Also, do not forget to add any required includes at the top of your file.
 */
struct node {
  int value;
  struct node *next;
};

struct list {
  struct node *first;
};


struct list *list_init(void) {
  struct list *l = malloc(sizeof(struct list));
  if (!l)
    return NULL;
  l->first = NULL;
  return l;
}

struct node *list_new_node(int num) {
  struct node *n = malloc(sizeof(struct node));
  if (!n)
    return NULL;
  n->value = num;
  n->next = NULL;
  return n;
}

struct node *list_head(const struct list *l) {
  if (!l)
    return NULL;
  return l->first;
}

struct node *list_next(const struct node *n) {
  if (!n)
    return NULL;
  return n->next;
}

int list_add_front(struct list *l, struct node *n) {
  if (!l || !n)
    return 1;
  n->next = l->first;
  l->first = n;
  return 0;
}

struct node *list_tail(const struct list *l) {
  if (!l || !l->first)
    return NULL;
  struct node *n;
  for (n = l->first; n->next; n = n->next);
  return n;
}

struct node *list_prev(const struct list *l, const struct node *n) {
  if (!l || !l->first || l->first == n)
    return NULL;
  struct node *node;
  for (node = l->first; node && node->next != n; node = node->next);
  return node;
}

int list_add_back(struct list *l, struct node *n) {
  if (!l || !n)
    return 1;
  if (!l->first) {
    l->first = n;
    return 0;
  }
  struct node *node;
  for (node = l->first; node && node->next; node = node->next);
  node->next = n;
  return 0;
}

int list_node_get_value(const struct node *n) {
  if (!n)
    return 0;
  return n->value;
}

int list_node_set_value(struct node *n, int value) {
  if (!n)
    return 1;
  n->value = value;
  return 0;
}

int list_unlink_node(struct list *l, struct node *n) {
  if (!l || !n || !l->first)
    return 1;
  struct node *node = l->first;
  struct node *node_prev = NULL;
  while (node && node != n) {
    node_prev = node;
    node = node->next;
  }
  if (!node) {
    return 1;
  } else if (node == l->first) {
    l->first = node->next;
  }
  if (node_prev)
    node_prev->next = n->next;
  n->next = NULL;
  return 0;
}

void list_free_node(struct node *n) {
  free(n);
}

int list_cleanup(struct list *l) {
  if (!l) {
    return 1;
  }
  if (l->first) {
    struct node *n = l->first;
    while (n) {
      struct node *buffer = n;
      n = n->next;
      list_free_node(buffer);
    }
  }
  free(l);
  return 0;
}

int list_node_present(const struct list *l, const struct node *n) {
  if (!l || !n)
    return -1;
  for (struct node *node = l->first; node; node = node->next) {
    if (node == n)
      return 1;
  }
  return 0;
}

int list_insert_after(struct list *l, struct node *n, struct node *m) {
  if (!l || !n || !m || !l->first || !list_node_present(l, m) || list_node_present(l, n))
    return 1;
  n->next = m->next;
  m->next = n;
  return 0;
}
int list_insert_before(struct list *l, struct node *n, struct node *m) {
  if (!l || !n || !m || !list_node_present(l, m) || list_node_present(l, n))
    return 1;
  if (m == l->first) {
    n->next = l->first;
    l->first = n;
  } else {
    struct node *prev = list_prev(l, m);
    prev->next = n;
    n->next = m;
  }
  return 0;
}

size_t list_length(const struct list *l) {
  if (!l)
    return 0;
  size_t i = 0;
  for (struct node *n = l->first; n; n = n->next) {
    i++;
  }
  return i;
}

struct node *list_get_ith(const struct list *l, size_t i) {
  if (!l)
    return 0;
  struct node *n = l->first;
  for (size_t j = 0; j < i; j++) {
    if (!n)
      return NULL;
    n = n->next;
  }
  return n;
}

struct list *list_cut_after(struct list *l, struct node *n) {
  if (!l || !n || !list_node_present(l, n))
    return NULL;
  struct list *l2 = list_init();
  if (!l2)
    return NULL;
  l2->first = n->next;
  n->next = NULL;
  return l2;
}
