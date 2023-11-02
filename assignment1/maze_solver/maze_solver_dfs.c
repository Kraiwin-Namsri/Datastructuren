#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND -1
#define ERROR -2
int *init_traverse(size_t capacity) {
  int *arr = malloc(capacity * sizeof(int));
  if (!arr) return NULL;
  for (size_t i = 0; i < capacity; i++) {
    arr[i] = -1;
  }
  return arr;
}

int traverse_path(struct maze *m, int *arr, int index) {
  int path_length = 0;
  int i = index;
  char c = 'D';
  int x, y;
  do {
    i = arr[i];
    y = maze_row(m, i);
    x = maze_col(m, i);
    c = maze_get(m, y, x);  
    if (c == ',') {
      maze_set(m, y, x, 'X');
    }
    path_length++;
    maze_print(m, false);
  } while (!maze_at_start(m, y, x));
  free(arr);
  return path_length;
}
/* Checks if the position is not visited yet and is visitable (not a wall).
 *
 */
int is_position_empty(struct maze *m, int r, int c) {
  return maze_get(m, r, c) == ' '; 
}
/* Find all indexes that are connect to the current index.
 * m: A pointer to a maze.
 * index: the index to find all possible paths for.
 * arr: an array with size 4 for storing all the connected indices.
 * Return-Value:
 *   The amount of found paths.
 */
int find_paths(struct maze *m, int index, int *arr) {
  int x, y;
  x = maze_col(m, index);
  int amount_paths = 0;
  y = maze_row(m, index);
  if (maze_valid_move(m, y+1, x) && is_position_empty(m, y+1, x)) {
    arr[amount_paths] = maze_index(m, y+1, x);
    amount_paths++;
  }
  if (maze_valid_move(m, y-1, x) && is_position_empty(m, y-1, x)) {
    arr[amount_paths] = maze_index(m, y-1, x);
    amount_paths++;
  }
  if (maze_valid_move(m, y, x+1) && is_position_empty(m, y, x+1)) {
    arr[amount_paths] = maze_index(m, y, x+1);
    amount_paths++;
  }
  if (maze_valid_move(m, y, x-1) && is_position_empty(m, y, x-1)) {
    arr[amount_paths] = maze_index(m, y, x-1);
    amount_paths++;
  }
  return amount_paths;
}

void set_maze_visited(struct maze *m, int index) {
  int x = maze_col(m, index);
  int y = maze_row(m, index);
  if (is_position_empty(m, y, x)) {
    maze_set(m, y, x, ',');
  }
}
/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int dfs_solve(struct maze *m) {
  struct stack *s;
  int x, y;
  size_t capacity = (size_t)maze_size(m);
  capacity = capacity * capacity;
  s = stack_init(capacity);
  if (!s) return ERROR;
  int *traverse_arr = init_traverse(capacity);
  if (!traverse_arr) {
    stack_cleanup(s);
    return ERROR;
  }
  maze_start(m, &y, &x);
  stack_push(s, maze_index(m, y, x));
  // printf("y: %d x: %d\n", y, x);
  // traverse_arr[index] = index;
  int index = 0;
  int paths[4];
  int amount_paths;
  while (index != -1) {
    index = stack_pop(s);
    set_maze_visited(m, index);
    amount_paths = find_paths(m, index, paths);
    if (amount_paths != 0) {
      for (int i = 0; i < amount_paths; i++) {
        x = maze_col(m, paths[i]);
        y = maze_row(m, paths[i]);
        traverse_arr[paths[i]] = index;
        if (maze_at_destination(m, y, x)) {
          stack_cleanup(s);
          return traverse_path(m, traverse_arr, paths[i]);
        }
        stack_push(s, paths[i]);
      }
    }
  }
  stack_cleanup(s);
  return NOT_FOUND;
}


int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
