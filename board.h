#ifndef BOARD_H
#define BOARD_H

/*********************************************************
 * clear_screen: Clears the screen so either a new board *
 *               be drawn or the board updated.          *
 *********************************************************/

void clear_screen(void);

/*********************************************************
 * draw_board: Takes the peice array from calling        *
 *             function.  Draws a new board and          *
 *             allocates the pieces on the board acording*
 *             to the piece array.                       *
 *********************************************************/

void draw_board(void);

#endif // BOARD_H
