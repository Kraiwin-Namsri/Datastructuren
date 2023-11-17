/* Name: Kraiwin Namsri
 * UvA-ID: 15032094
 * This program is made to sort a string from stdin.
 */
#include <check.h>
#include <getopt.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#define BUF_SIZE 1024
#define DELIMITER " "

char buf[BUF_SIZE];

struct config {
    /* You can ignore these options until you implement the
       extra command-line arguments. */

    /* Set to 1 if -d is specified, 0 otherwise. */
    int descending_order;

    // Set to 1 if -c is specified, 0 otherwise.
    int combine;

    // Set to 1 if -o is specified, 0 otherwise.
    int remove_odd;

    /* Set to 1 if -z is specified, 0 otherwise. */
    int zip_alternating;
};

int parse_options(struct config *cfg, int argc, char *argv[]) {
    memset(cfg, 0, sizeof(struct config));
    int c;
    while ((c = getopt(argc, argv, "dcoz")) != -1) {
        switch (c) {
        case 'd':
            cfg->descending_order = 1;
            break;
        case 'c':
            cfg->combine = 1;
            break;
        case 'o':
            cfg->remove_odd = 1;
            break;
        case 'z':
            cfg->zip_alternating = 1;
            break;
        default:
            fprintf(stderr, "invalid option: -%c\n", optopt);
            return 1;
        }
    }
    return 0;
}
/* Print a list to stdout.
 *  l: a pointer to a list.
 */
void print_output(struct list *l) {
  struct node *node = list_head(l);
  while (node) {
    printf("%d\n", list_node_get_value(node));
    node = list_next(node);
  }
}
/* Parse an integer inside a string. 
 *  token: a pointer to the start of a string.
 *  n: a pointer to a long, that can be set to the result.
 * Return-Value
 *  Returns 0 on succes, 1 on not.
 */
int parse_token(char *token, long *n) {
  char *endptr = token;
  char *endptr_prev = endptr;
  while (*endptr != '\0') {
    *n = strtol(endptr, &endptr, 10);
    // printf("str: %s char: %c, ptr: %p n: %ld\n", endptr, *endptr, endptr, n);
    if (endptr == endptr_prev) {
      endptr += sizeof(char);
    } else {
      return 0;
    }
    endptr_prev = endptr;
  }
  return 1;
}
/* Parses a character buffer like: "1 3 5 1"
 *   l: a pointer to a list.
 *   buf: a pointer to the beginning of a string.
 *  Return-Value:
 *   0 on succes
 */
int parse_input(struct list *l, char *buf) {
  char *token = strtok(buf, DELIMITER);
  while (token) {
    long n;
    int status = parse_token(token, &n);
    if(status) {
      break;
    }
    struct node *node = list_new_node((int)n);
    if (!node)
      return 1;
    if (list_add_front(l, node)) {
      list_free_node(node);
      return 1;
    }
    token = strtok(NULL, DELIMITER);
  }
  return 0;
}

/* Swaps node n with node->next inside the list.
 *  l: a pointer to a list.
 *  n: a pointer to the node the switch.
 * Return-Value:
 *  Returns 0 on succes, and 1 on fail.
 */
int swap_next(struct list *l, struct node *n) {
  struct node *node_next = list_next(n);
  if (!node_next)
    return 1;
  list_unlink_node(l, n);
  list_insert_after(l, n, node_next);
  return 0;
}
/*  Sorts a list in ascending order.
*   Return-Value:
*     Returns 0 on sucecs, and 1 on fail.
*/
void sort_list(struct list *l, int descending_order) { 
  size_t len = list_length(l);
  for (int i = (int)len; i >= 0; i--) {
    struct node *n = list_get_ith(l, (size_t)i);
    struct node *n_next = list_next(n);
    if (n_next && n) {
      while (((list_node_get_value(n) < list_node_get_value(n_next) && descending_order) ||
              (list_node_get_value(n) > list_node_get_value(n_next) && !descending_order))) {
        if (swap_next(l, n))
          break;
        n_next = list_next(n);
      }
    }
  }
}
/* Combine every pair inside list l to one node, by adding the values of both.
*   l: a pointer to a list.
*  Side-Effects:
*   Combines every pair.
*/
void combine(struct list *l) {
  size_t len = list_length(l);
  for (size_t i = 0; i < len; i++) {
    struct node *n_1 = list_get_ith(l, i);
    struct node *n_2 = list_get_ith(l, i + 1);
    list_node_set_value(n_1, list_node_get_value(n_1) + list_node_get_value(n_2));
    list_unlink_node(l, n_2);
    list_free_node(n_2);
  }
}
/* Removes all odd valued nodes from list l.
*/
void remove_odd(struct list *l) {
  size_t len = list_length(l);
  for (size_t i = 0; i < len; i++) {
    struct node *n = list_get_ith(l, i);
    if ((list_node_get_value(n) % 2)) {
      list_unlink_node(l, n);
      list_free_node(n);
      len--;
      i--;
    }
  }
}
/* Cuts list l in half, with the first half being longer or equal to the second half.
*   l: a pointer to a list.
*/
void zip(struct list *l) {
  size_t len = list_length(l);
  size_t mid = (len / 2) + (len % 2);
  for (size_t i = 0; i < mid; i++) {
    struct node *n = list_get_ith(l, mid+i);
    struct node *m = list_get_ith(l, i*2);
    list_unlink_node(l, n);
    list_insert_after(l, n, m);
  }
}
/* Processes a list in the order: sort, dcoz
*/
void process(struct config cfg, struct list *l) {
  sort_list(l, cfg.descending_order);
  if (cfg.remove_odd)
    remove_odd(l);
  if (cfg.combine)
    combine(l);
  if (cfg.zip_alternating)
    zip(l);
}
int main(int argc, char *argv[]) {
  struct config cfg;
  if (parse_options(&cfg, argc, argv) != 0) {
    return 1;
  }
  struct list *l = list_init();
  if (!l)
    return 1;
  while (fgets(buf, BUF_SIZE, stdin)) {
    if (parse_input(l, buf)) {
      list_cleanup(l);
      fprintf(stderr, "Could not parse stdin!\n");
      return 1;
    }
  }
  process(cfg, l);
  print_output(l);
  list_cleanup(l);
  return 0;
}
