#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "queue.h"

#define NOT_FOUND -1
#define ERROR -2
void maze_calculate_index(struct maze *m, int index, int *row, int *col) {
  *row = maze_row(m, index);
  *col = maze_col(m, index);
}
int traverse_path(struct maze *m, int *arr, int index) {
  int path_length = 0;
  int i = index;
  int row, col;
  do {
    i = arr[i];
    maze_calculate_index(m, i, &row, &col);
    char c = maze_get(m, row, col);
    if (c == VISITED) {
      maze_set(m, row, col, PATH);
    }
    path_length++;
  } while (!maze_at_start(m, row, col));
  return path_length;
}
/* Checks if the position is not visited yet and is visitable (not a wall).
 *
 */
int is_position_empty(struct maze *m, int r, int c) {
  return maze_get(m, r, c) == FLOOR;
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

void set_maze_to_visit(struct maze *m, int row, int col) {
  if (is_position_empty(m, row, col)) {
    maze_set(m, row, col, TO_VISIT);
  }
}

void set_maze_visited(struct maze *m, int index) {
  int row, col;
  maze_calculate_index(m, index, &row, &col);
  if (maze_get(m, row, col) == TO_VISIT) {
    maze_set(m, row, col, VISITED);
  }
}
/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occured.
 */
int bfs_solve(struct maze *m) {
   // Initialize stack
  struct queue *q;
  size_t capacity = (size_t)maze_size(m);
  capacity = capacity * capacity;
  q = queue_init(capacity);
  if (!q) return ERROR;
  int traverse_arr[capacity];

  int row, col;
  maze_start(m, &row, &col);
  queue_push(q, maze_index(m, row, col));
  do {
    int index = queue_pop(q);
    set_maze_visited(m, index);

    int paths[4];
    int amount_paths;
    amount_paths = find_paths(m, index, paths);
    if (amount_paths != 0) {
      for (int i = 0; i < amount_paths; i++) {
        maze_calculate_index(m, paths[i], &row, &col);
        traverse_arr[paths[i]] = index;
        if (maze_at_destination(m, row, col)) {
          queue_cleanup(q);
          return traverse_path(m, traverse_arr, paths[i]);
        }
        set_maze_to_visit(m, row, col);
        queue_push(q, paths[i]);
      }
    }
  } while (!queue_empty(q));
  queue_cleanup(q);
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
    int path_length = bfs_solve(m);
    if (path_length == ERROR) {
        printf("bfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("bfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
