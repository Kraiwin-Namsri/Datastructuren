/* Do not edit this file. */

#ifndef _MAZE_H_
#define _MAZE_H_

/* Defines for ascii characters used in the maze array. */
#define WALL '#'
#define FLOOR ' '
#define PATH 'x'
#define VISITED '.'
#define TO_VISIT ','

/* We move in the following four directions: up, right, down, left
 * The origin of the maze (0, 0) is the upper left hand corner. */
#define N_MOVES 4
extern int m_offsets[N_MOVES][2];

/* Forward declaration for using a struct maze pointer in the prototypes. */
struct maze;

/* Reads a square maze from stdin. Start and destination markers are detected
 * and recorded. Everything that is not a WALL is stored as a FLOOR.
 * Returns a pointer to the maze or NULL if an error occured. */
struct maze *maze_read(void);

/* Frees all memory associated with the maze. */
void maze_cleanup(struct maze *m);

/* Returns the maze character stored at row 'r', column 'c'. */
char maze_get(const struct maze *m, int r, int c);

/* Sets the maze character at row 'r', column 'c' to 'value'. */
void maze_set(struct maze *m, int r, int c, char value);

/* Prints the maze to stdout. If 'blocks' is true walls are printed as a block
 * character, otherwise the WALL character '#' is used. */
void maze_print(const struct maze *m, bool blocks);

/* Writes the maze in Portable Pixmap (ppm) format to 'filename'. */
int maze_output_ppm(const struct maze *m, const char *filename);

/* Sets the integer values pointed to by 'r' and 'c' to the row
 * and column of the start position. */
void maze_start(const struct maze *m, int *r, int *c);

/* Sets the integer values pointed to by 'r' and 'c' to the row
 * and column of the destination position. */
void maze_destination(const struct maze *m, int *r, int *c);

/* Returns true if (r, c) is the start location. */
bool maze_at_start(const struct maze *m, int r, int c);

/* Returns true if (r, c) is the destination location. */
bool maze_at_destination(const struct maze *m, int r, int c);

/* Returns true if (r, c) is valid position in the maze.
 * Note: The borders of the maze, rows 0 and maze_size() - 1 and columns
 * 0 and maze_size() - 1, are inaccessible. */
bool maze_valid_move(const struct maze *m, int r, int c);

/* Returns the size of the maze 'm'.
 *
 * We only support square mazes, so the size is the number of rows
 * which is also the number of columns. */
int maze_size(const struct maze *m);

/* Returns the index in the 1d array for row 'r' and column 'c'.
 *
 * Although there is no need to expose that the maze is internally stored
 * as one dimension array, using the index allows a location to be
 * stored as a single integer on the stack or queue instead of two
 * separate integers for the row and column of a location. */
int maze_index(const struct maze *m, int r, int c);

/* Returns the row number of the 1d 'index'. */
int maze_row(const struct maze *m, int index);

/* Returns the column number of the 1d 'index'. */
int maze_col(const struct maze *m, int index);

#endif
