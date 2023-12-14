#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "tree.h"

struct set {
  struct tree *tree;
};

struct set *set_init(int turbo) {
  struct set *s = malloc(sizeof(struct set));
  if (!s)
    return NULL;


  struct tree *tree = tree_init(turbo);
  if (!tree) {
    free(s);
    return NULL;
  }

  s->tree = tree;
  return s;
}

int set_insert(struct set *s, int num) {
  if (!s)
    return -1;

  return tree_insert(s->tree, num);
}

int set_find(struct set *s, int num) {
  if (!s)
    return -1;

  return tree_find(s->tree, num);
}

int set_remove(struct set *s, int num) {
  if (!s)
    return -1;


  return tree_remove(s->tree, num);
}
void set_cleanup(struct set *s) {
  if (!s)
    return;

  tree_cleanup(s->tree);
  free(s);
}

void set_print(const struct set *s) {
  if (!s)
    return;

  tree_print(s->tree);
}

int set_verify(const struct set *s) {
  return tree_check(s->tree);
}
