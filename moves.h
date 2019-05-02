#ifndef MOVES_H
#define MOVES_H
#include <stdbool.h>

/*****************************************************
 * initialize: Initialize will set both the          *
 *             piece_position array and piece_struct *
 *             structs up for a new game.  Sets      *
 *             turn_flag to false for white's first  *
 *             go.                                   *
 *****************************************************/

void initialize(void);


/*****************************************************
 * play_game: Play game: will be responcable for     *
 *            calling the appropriate function(s) to *
 *            move a piece or take an opponents      *
 *            piece.                                 *
 *****************************************************/

void play_game(void);

/*****************************************************
* get coordinates: asks for the x and y co-ordinates *
*                  multiplys the y co-ordinate by 8  *
*                  and adds the x co-ordinate to the *
*                  result so a unique number is      *
*                  generated then returns the result *
*                  or 99 if the player surrenders    *
******************************************************/

int get_coordinates(bool error);

/*****************************************************
 * find piece: accepts the co-ordinates from the     *
 *             calling function searches the piece   *
 *             struct elements and returns the       *
 *             elements index if found otherwise     *
 *             returns -1.                           *
 *****************************************************/

int find_piece(int x, int y);

/*****************************************************
 * Valid piece :Accepts the co-ordinates entered by  *
 *              the user and calls find piece to     *
 *              check if there is a piece of thier   *
 *              colour there.  If so returns 0       *
 *              otherwise returns -1                 *
 *****************************************************/

int valid_piece(int x, int y);

/*****************************************************
 * valid_square: Accepts the new co-ordinates of a   *
 *               piece, checks them against the      *
 *               illegal square array and then calls *
 *               find piece to make sure the square  *
 *               isnt occupied by another piece.     *
 *               Returns -1 if invalid for above     *
 *               reasons 0 if its okay.              *
 *****************************************************/

int valid_square(int x, int y);

/*****************************************************
 * check if king: Accepts the old co-ordinates from  *
 *                the calling function.  Calls       *
 *                find_piece to get the correct      *
 *                structs array element index and    *
 *                checks if its a king or not.       *
 *                returns true or false respectivly  *
 *****************************************************/

bool check_if_king(int x, int y);

/*****************************************************
 * calculate moves: Accepts the old and new          *
 *                  co-ordinates and calls the       *
 *                  appropriate function to either   *
 *                  move a piece 1 square or take    *
 *                  the opponents piece(s).  Returns *
 *                  0 if sucessful or -1 if it fails *
 *****************************************************/

int calculate_moves(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * whos_turn_multiplyer: To be used by a function    *
 *                       a switch statment.  If it is*
 *                       whites turn it returns 1 if *
 *                       its blacks turn it returns  *
 *                       -1.                         *
 *****************************************************/

int whos_turn_multiplyer(void);

/*****************************************************
 * move_piece: Move piece accepts the old and new    *
 *             co-ordinates and will move a piece    *
 *             one square diagonaly up the board     *
 *             (down in the case of black).  Then    *
 *             updates the piece array and the       *
 *             pieces struct element.  With the new  *
 *             co-ordinates of the piece.  Returns   *
 *             0 if sucsessful -1 if it fails        *
 *****************************************************/

int move_piece(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * update piece position: updates the piece position *
 *                        array and the appropriate  *
 *                        element of the players     *
 *                        piece struct array.        *
 *                        returns 0 if ok or -1 if it*
 *                        fails.                     *
 *****************************************************/

int update_piece_position(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * take_piece: Accepts the old and new co-ordinates  *
 *             and checks the pieces struct element  *
 *             to see if is a king.  Then calculates *
 *             the route by marking the possible     *
 *             pieces taken as "t" in the take_piece *
 *             array.  If a valid path is found it   *
 *             updates the appropriate pieces struct *
 *             elements and piece array.  Finaly it  *
 *             returns 0.  If no valid path is found *
 *             it then returns -1                    *
 *****************************************************/

int take_piece(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * taking own piece: Make sure the player isnt trying*
 *                   to jump thier own piece.        *
 *                   Returns false if they aren't or *
 *                   true if they are.               *
 *****************************************************/

bool taking_own_piece(char temp_array[][8], int old_x, int old_y);

/*****************************************************
 * find taken piece: searches the struct piece array *
 *                   for the piece with the matching *
 *                   co-ordinates of the piece that  *
 *                   has been taken and returns the  *
 *                   array elements index or returns *
 *                   -1 if it cant find it.          *
 *****************************************************/

int find_taken_piece(int x, int y);

/*****************************************************
 * initalize temp array Copies the piece array into  *
 *                      the temp array that is       *
 *                      created either in take_piece *
 *                      or take_piece_king functions *
 *                      and passed to it.            *
 *****************************************************/

void initialize_temp_array(char temp_array[][8], int x);

/*****************************************************
 * transfer temp array: updates the piece array and  *
 *                      the opponants apropriate     *
 *                      struct array element with any*
 *                      piece(s) taken.              *
 *****************************************************/

void transfer_temp_array(char temp_array[][8], int x);

/*****************************************************
 * make_king: Accepts the new co-ordinates and       *
 *            updates the pieces struct element      *
 *            making the king definition true.  Then *
 *            changes the letter in the piece array  *
 *            to a capital letter (ie W or B).       *
 *****************************************************/
void make_king(int new_x, int new_y);

/*****************************************************
 * calculate_kings_moves: Accepts the old and new    *
 *                        co-ordinates and calls the *
 *                        appropriate function to    *
 *                        either move 1 square or    *
 *                        take a piece.  Returns 1 if*
 *                        successful of -1 if not.   *
 *****************************************************/

int calculate_king_moves(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * move_king: Accepts the old and new co-ordinates   *
 *            and moves the king diagonaly one square*
 *            in any direction.  Then updates the    *
 *            piece's struct element and piece array *
 *            with the new co-ordinates.  Returns 0  *
 *            all ok or -1 otherwise.                *
 *****************************************************/

int move_king(int old_x, int old_y, int new_x, int new_y);

 /****************************************************
  * take_piece_king: Accepts the old and new         *
  *                  co-ordinates and works out the  *
  *                  jumps needed to arrive at the   *
  *                  position the same way as take   *
  *                  piece does.  If a suitable path *
  *                  is found it updates the pieces  *
  *                  struct element with new         *
  *                  co-ordinates and the piece array*
  *                  both in terms of pieces taken   *
  *                  and the kings new location.     *
  *                  Returns 0 if a suitable path    *
  *                  found or -1 if it fails.        *
  ****************************************************/

int take_piece_king(int old_x, int old_y, int new_x, int new_y);

/*****************************************************
 * check diagonal square: Checks the temp piece array*
 *                        passed to it for a piece in*
 *                        the square up/down and to  *
 *                        the left/right 1 square of *
 *                        the pieces position.       *
 *                        Returns true if a piece of *
 *                        ANY colour is found or     *
 *                        false if none are found.   *
 *****************************************************/

bool check_diagonal_square(char temp_array[][8], int x, int y);

/*****************************************************
 * count taken pieces: Runs through the opponents    *
 *                     piece array and counts the    *
 *                     number of taken pieces and    *
 *                     returns the number of taken   *
 *                     pieces.                       *
 *****************************************************/

int count_taken_pieces(void);

/*****************************************************
 * toggle_turn: toggles the turn_flag between false  *
 *              for white and true for black.        *
 *              Returns either true or false         *
 *              respectivly.                         *
 *****************************************************/

bool toggle_turn(void);

/*****************************************************
 * game over: Checks the integer passed to it to     *
 *            determin if a player surrendered (9) or*
 *            if all the opponents pieces were taken *
 *            (0). Displays game over on the screen  *
 *            and the winning colour.                *
 *****************************************************/

void game_over(int x);

#endif // MOVES_H
