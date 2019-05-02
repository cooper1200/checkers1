#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "moves.h"
#include "board.h"

typedef struct
{
    int position_x;
    int position_y;
    bool king;
    bool taken;
} piece_struct;

// global declarations
piece_struct white[12];
piece_struct black[12]; //    element/co-ordinate [0][0] is here idiot 1's are valid pieces!!
//                            |
char piece_position[8][8];//  V
int illegal_squares[8][8] = {{1,0,1,0,1,0,1,0},
                             {0,1,0,1,0,1,0,1},
                             {1,0,1,0,1,0,1,0},
                             {0,1,0,1,0,1,0,1},
                             {1,0,1,0,1,0,1,0},
                             {0,1,0,1,0,1,0,1},
                             {1,0,1,0,1,0,1,0},
                             {0,1,0,1,0,1,0,1}};

bool whos_turn;  //false = whites turn true = blacks turn.

#define DEBUG 0

void initialize(void)
{
    int i, j, white_row = 0, white_column = 0, black_row = 7, black_column = 1;

    // clear piece_possition array with each element set to a space.
    for (j = 0; j < 8; j++) //columns
    {
        for (i = 0; i < 8; i++) //rows
        {
            piece_position[i][j] = ' ';
        }
    }

    //set up piece_structs
    for (i = 0; i < 12; i++)
    {
        white[i].position_x = white_column;
        //white[i].possition_x = 1;
        white[i].position_y = white_row;
        white[i].king = false;
        white[i].taken = false;
        piece_position[white[i].position_x][white[i].position_y] = 'w';
        black[i].position_x = black_column;
        black[i].position_y = black_row;
        black[i].king = false;
        black[i].taken = false;
        piece_position[black[i].position_x][black[i].position_y] = 'b';
        white_column += 2;
        black_column += 2;
        if (white_column == 8) // 1st rows done
        {
            white_column = 1;
            white_row = 1;
            black_column = 0;
            black_row = 6;
        }
        if (white_column == 9) // 2nd rows done
        {
            white_column = 0;
            white_row = 2;
            black_column = 1;
            black_row = 5;
        }
    }
    if (whos_turn == true) //if whos turn is set to black's set it to white's turn
    {
        whos_turn = toggle_turn();
    }
    #if DEBUG
    for (i = 0; i < 12; i++)
    {
        printf("whites co-ordinates: %d:%d blacks co-ordinates: %d:%d\n", white[i].position_x, white[i].position_y, black[i].position_x, black[i].position_y);
        printf("white king: %d black king: %d\n",white[i].king, black[i].king);
        printf("white taken: %d black taken: %d\n",white[i].taken, black[i].taken);
    }
    for (j = 7; j >= 0; j--)
    {
        for (i = 0; i < 8; i++)
        {
            printf("%c",piece_position[i][j]);
        }
        printf("\n");
    }
    #endif // debug
}

bool toggle_turn(void)
{
    static bool flag;

    if (flag == true)
    {
        flag = false;
    }
    else
    {
        flag = true;
    }

    return flag;
}
void play_game(void)
{
    int old_x, new_x, old_y, new_y, a, coordinates;
    bool error_flag = false;

    for (;;)
    {
        a = 1;
        while (a == 1)
        {
            coordinates = get_coordinates(error_flag);
            if (coordinates == 99) // 99 is the surrender code
            {
                break;
            }
            else
            {
                old_x = coordinates % 8;
                old_y = coordinates / 8;
            }
            if ((old_x >= 0 && old_x < 8) && (old_y >= 0 && old_y < 8))
            {
                error_flag = false; //set to false as if there was an error the 1st set of new co-ordinates must be ok so back on track for second set
                if (valid_piece(old_x, old_y) == 0)
                {
                    printf("valid piece selected at co-ordinates %d,%d\n",old_x, old_y);
                    coordinates = get_coordinates(error_flag);
                    new_x = coordinates % 8;
                    new_y = coordinates / 8;
                    if ((new_x >= 0 && new_x < 8) && (new_y >= 0 && new_y < 8)) //make sure new_x and new_y are between 0 and 7
                    {
                        if (valid_square(new_x, new_y) == 0)
                        {
                            if (check_if_king(old_x, old_y) == true)
                            {
                                if (calculate_king_moves(old_x, old_y, new_x, new_y) == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    printf("Illegal move start again\n");
                                    error_flag = true;//make sure get_coordinates gets the coordinates of the piece to move
                                }
                            }
                            else
                            {
                                if (calculate_moves(old_x, old_y, new_x, new_y) == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    printf("Illegal move start again\n");
                                    error_flag = true;
                                }
                            }
                        }
                        else
                        {
                            printf("Invalid square! Please start again\n");
                            error_flag = true;
                        }
                    }
                    else
                    {
                        printf("New co-ordinates are off the board! Please start again\n");
                        error_flag = true;
                    }
                }
                else
                {
                    printf("Invalid piece selected! Please try again\n");
                    error_flag = true;
                }
            }
            else
            {
                printf("Co-ordinates are off the board!! please try again\n");
                error_flag = true;
            }
        }
        if (coordinates == 99)
        {
            game_over(9);
            break;
        }
        if ((whos_turn == false && new_y == 7) || (whos_turn == true && new_y == 0)) // a piece has reached the other side of the board so make king
        {
            make_king(new_x, new_y);
        }
        if (count_taken_pieces() != 12) // make sure the opponent has pieces left
        {
            whos_turn = toggle_turn();
            clear_screen();
            draw_board();
            error_flag = false;  // turn flag to false otherwise the new co-ordinates will never be set
        }
        else
        {
            game_over(0);
            break;
        }
    }
}

int get_coordinates(bool error)
{
    int x_coordinate, y_coordinate;
    static bool flag; // flags first time run for first lot of co-ordinates or second time run for second lot of co-ordinates
    char str1[] = "whites turn:";

    if (error == true) // invalid co-ordinates entered start again
    {
        flag = false;
    }

    whos_turn ? strcpy(str1, "Blacks turn:") : strcpy(str1, "Whites turn:"); //toggle the value of str1

    if (flag == false)  //1st time run
    {
        printf("%s Please enter the x co-ordinate (0 - 7) or 9 to surrender: ", str1);
        scanf(" %d", &x_coordinate);
        if (x_coordinate == 9)
        {
            return 99; // returns a unique number to test for
        }
        else
        {
            printf("%s Please enter the y co-ordinate (0 - 7):", str1);
            scanf(" %d", &y_coordinate);
            flag = true;
            return 8 * y_coordinate + x_coordinate; //returns 2 values in 1 integer
        }
    }
    else
    {
        printf("%s Please enter the new x co-ordinate (0 - 7): ", str1);
        scanf(" %d", &x_coordinate);
        printf("%s Please enter the new y co-ordinate (0 - 7):", str1);
        scanf(" %d", &y_coordinate);
        flag = false;
        return 8 * y_coordinate + x_coordinate;
    }
}

int find_piece(int x, int y)
{
    int i, found = -1;

    if (whos_turn == false)
    {
        for (i = 0; i < 12; i++)
        {
            if ((white[i].position_x == x) && (white[i].position_y == y))
            {
                found = i; //code this better ie return i
                break;
            }
        }
    }
    else
    {
        for (i = 0; i < 12; i++)
        {
            if ((black[i].position_x == x) && (black[i].position_y == y))
            {
                found = i;
                break;
            }
        }
    }

    return found;
}

int find_taken_piece(int x, int y)
{
    int i, piece_index;

    for (i = 0; i < 12; i++)
    {
        if (whos_turn == false) //whites go so look for black pieces
        {
            if (black[i].position_x == x && black[i].position_y == y)
            {
                piece_index = i;
                break;
            }
        }
        else
        {
            if (white[i].position_x == x && white[i].position_y == y)
            {
                piece_index = i;
                break;
            }
        }
    }

    if (i < 12) //piece found
    {
        return piece_index;
    }
    else
    {
        return -1;
    }
}

int valid_piece(int x, int y)
{
    int piece_index;

    piece_index = find_piece(x, y);

    if (piece_index >= 0) //piece found
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int valid_square(int x, int y) //leave it alone its right!!!
{
    if ((illegal_squares[x][y] == 1) && (piece_position[x][y] == ' '))
    {
        if ((x >= 0 && x < 8) && (y >= 0 && y < 8))// check in case of taking 2 or more pieces and player goes off the board mid jump
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

bool check_if_king(int x, int y)
{
    int piece_index;
    bool result;

    piece_index = find_piece(x, y);
    if (piece_index >= 0)
    {
        if (whos_turn == false)
        {
            result = white[piece_index].king;
        }
        else
        {
            result = black[piece_index].king;
        }
    }
    else
    {
        result = false;
    }
    return result;
}

int calculate_moves(int old_x, int old_y, int new_x, int new_y)
{
    int multiplyer, value_to_return = -1;

    multiplyer = whos_turn_multiplyer();

    if ((old_x - new_x == 0) && (new_y - old_y == 4 * multiplyer))// take 2 pieces one above the other
    {
        if(take_piece(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
    }
    else if(((old_x - new_x == 1) || (old_x - new_x == -1)) && (new_y - old_y == 1 * multiplyer))//move piece 1 square in the right direction
    {
        if(move_piece(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
    }
    else if(((old_x - new_x == 2) || (old_x - new_x == -2)) && ((new_y - old_y == 2 * multiplyer) || (new_y - old_y == 6 * multiplyer)))//taking 1 or 3 pieces
    {
        if (take_piece(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
    }
    else if(((old_x - new_x == 4) || (old_x - new_x == -4)) && (new_y - old_y == 4 * multiplyer))//take 2 pieces in a straight line
    {
        if (take_piece(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
    }
    else if(((old_x - new_x == 6) || (old_x - new_x == -6)) && (new_y - old_y == 6 * multiplyer))//take 3 pieces in a straight line
    {
        if (take_piece(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
    }
    else
    {
        value_to_return = -1;
    }

    return value_to_return;
}

int whos_turn_multiplyer()
{
    int x;

    if (whos_turn == false)
    {
        x = 1;
    }
    else
    {
        x = -1;
    }
    return x;
}

int move_piece(int old_x, int old_y, int new_x, int new_y)
{
    if (update_piece_position(old_x, old_y, new_x, new_y) == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int update_piece_position(int old_x, int old_y, int new_x, int new_y)
{
    int piece_index;

    piece_position[old_x][old_y] = ' ';//a space
    piece_index = find_piece(old_x, old_y);
    if (piece_index >=0)
    {
        if (whos_turn == false)
        {
            white[piece_index].position_x = new_x;
            white[piece_index].position_y = new_y;
            if (white[piece_index].king == true)
            {
                piece_position[new_x][new_y] = 'W';
            }
            else
            {
                piece_position[new_x][new_y] = 'w';
            }
        }
        else
        {
            black[piece_index].position_x = new_x;
            black[piece_index].position_y = new_y;
            if (black[piece_index].king == true)
            {
                piece_position[new_x][new_y] = 'B';
            }
            else
            {
                piece_position[new_x][new_y] = 'b';
            }
        }
        return 0;
    }
    else
    {
        return -1;
    }

}

int take_piece(int old_x, int old_y, int new_x, int new_y)
{
    int multiplyer_x, multiplyer_y, coordinate_x = old_x, coordinate_y = old_y, break_loop = 1, value_to_return = -1;
    char temp_take_piece[8][8];

    initialize_temp_array(temp_take_piece, 8);
    multiplyer_y = whos_turn_multiplyer();

    while (break_loop == 1)
    {
        for (multiplyer_x = -1; multiplyer_x <= 1; multiplyer_x += 2)
        {
            if (check_diagonal_square(temp_take_piece, coordinate_x + (1 * multiplyer_x), coordinate_y + (1 * multiplyer_y)) == true)// piece to take found
            {
                if (valid_square(coordinate_x + (2 * multiplyer_x), coordinate_y + (2 * multiplyer_y)) == 0)  //check square is free incase taking more than 1 piece
                {
                    if (taking_own_piece(temp_take_piece, coordinate_x + (1 * multiplyer_x), coordinate_y + (1 * multiplyer_y)) == false)
                    {
                        temp_take_piece[coordinate_x + (1 * multiplyer_x)][coordinate_y + (1 * multiplyer_y)] = 't';
                        coordinate_x += (2 * multiplyer_x);
                        coordinate_y += (2 * multiplyer_y); // co-ordinate_y = co-ordinate_y + (+/- 2) depending on multiplyer being 1 or -1
                        value_to_return = 0;
                        break;
                    }
                    else
                    {
                        printf("you cant take your own piece\n");
                        value_to_return = -1;
                        break_loop = 0;
                        break;
                    }
                }
                else
                {
                    temp_take_piece[coordinate_x + (1 * multiplyer_x)][coordinate_y + (1 * multiplyer_y)] = 'v'; //delete what was there so its not picked up next time round
                }
            }
        }
        if (multiplyer_x > 1) // piece to take not found
        {
            value_to_return = -1;
            break_loop = 0;
        }
        if ((coordinate_x == new_x) && (coordinate_y == new_y)) // end of move
        {
            value_to_return = 0;
            break_loop = 0;
        }
    }
    if (value_to_return == 0)
    {
        if(update_piece_position(old_x, old_y, new_x, new_y) == 0)
        {
            transfer_temp_array(temp_take_piece, 8);
        }
        else
        {
            value_to_return = -1;
        }
    }
    return value_to_return;
}

void initialize_temp_array(char temp_array[][8], int x)
{
    int i, j;

    for (j = 0; j < x; j++)
    {
        for (i = 0; i < 8; i++)
        {
            temp_array[i][j] = piece_position[i][j];
        }
    }
}

bool taking_own_piece(char temp_array[][8], int x, int y)
{
    bool value_to_return;

    if (whos_turn == false) // whites go
    {
        if (temp_array[x][y] == 'w' || temp_array[x][y] == 'W')
        {
            value_to_return = true;
        }
        else
        {
            value_to_return = false;
        }
    }
    else
    {
        if (temp_array[x][y] == 'b' || temp_array[x][y] == 'B')
        {
            value_to_return = true;
        }
        else
        {
            value_to_return = false;
        }
    }

    return value_to_return;
}

void transfer_temp_array(char temp_array[][8], int x)
{
    int i, j, piece_index;

    for (j = 0; j < x; j++)
    {
        for (i = 0; i < 8; i++)
        {
            if (temp_array[i][j] == 't')
            {
                piece_position[i][j] = ' ';// a space
                piece_index = find_taken_piece(i, j);
                if (whos_turn == false)
                {
                    black[piece_index].taken = true;
                }
                else
                {
                    white[piece_index].taken = true;
                }

            }
        }
    }
}

void make_king(int new_x, int new_y)
{
    int piece_index;

    piece_index = find_piece(new_x, new_y);
    if (whos_turn == false) // whites turn
    {
        white[piece_index].king = true;
        piece_position[new_x][new_y] = 'W';
    }
    else
    {
        black[piece_index].king = true;
        piece_position[new_x][new_y] = 'B';
    }
}

int calculate_king_moves(int old_x, int old_y, int new_x, int new_y)
{
    int value_to_return = -1;

    if ((new_x - old_x == -1) || (new_x - old_x == 1))
    {
        if (move_king(old_x, old_y, new_x, new_y) == 0)
        {
            value_to_return = 0;
        }
        else
        {
            value_to_return = -1;
        }
    }
    else
    {
        if (take_piece_king(old_x, old_y, new_x, new_y) == 0) //code this
        {
            value_to_return = 0;
        }
        else
        {
            value_to_return = -1;
        }
    }

    return value_to_return;
}

int move_king(int old_x, int old_y, int new_x, int new_y)
{
    int value_to_return, multiplyer_x, multiplyer_y, break_loop = 1;

    for (multiplyer_y = -1; multiplyer_y <= 1; multiplyer_y += 2) // down and up
    {
        for (multiplyer_x = -1; multiplyer_x <= 1; multiplyer_x += 2) // left and right
        {
            if ((new_x - old_x == (1 * multiplyer_x)) && (new_y - old_y == (1 * multiplyer_y)))// check the y co-ordinates
            {
                if (update_piece_position(old_x, old_y, new_x, new_y) == 0)
                {
                    value_to_return = 0;
                    break_loop = 0;
                }
                else
                {
                    value_to_return = -1;
                    break_loop = 0;
                }
                break; //break out of multiplyer_x
            }
        }
        if (break_loop == 0)
        {
            break; // break out of multiplyer y
        }
    }
    if ((multiplyer_x > 1) && (multiplyer_y > 1)) //move not found!
    {
        value_to_return = -1;
    }

    return value_to_return;
}
int take_piece_king(int old_x, int old_y, int new_x, int new_y)
{
    int coordinate_x = old_x, coordinate_y = old_y, break_loop = 1, value_to_return = -1;
    int multiply_x, multiply_y, piece_to_take_found;
    char temp_take_piece[8][8];

    initialize_temp_array(temp_take_piece, 8);
    while (break_loop == 1)
    {
        piece_to_take_found = 0;
        for ( multiply_y = -1; multiply_y <= 1; multiply_y +=2)
        {
            for (multiply_x = -1; multiply_x <= 1; multiply_x +=2)
            {
                if (check_diagonal_square(temp_take_piece, coordinate_x + (1 * multiply_x), coordinate_y + (1 * multiply_y)) == true)
                {
                    if (valid_square(coordinate_x + (2 * multiply_x), coordinate_y + (2 * multiply_y)) == 0) //check in case more than one jump
                    {
                        if (taking_own_piece(temp_take_piece, coordinate_x + (1 * multiply_x), coordinate_y + (1 * multiply_y)) == false)
                        {
                            temp_take_piece[coordinate_x + (1 * multiply_x)][coordinate_y + (1 * multiply_y)] = 't';
                            coordinate_x += (2 * multiply_x);
                            coordinate_y += (2 * multiply_y);
                            piece_to_take_found = 1;
                            value_to_return = 0;
                        }
                        else
                        {
                            printf("you cant take your own piece\n");
                            value_to_return = -1;
                            break_loop = 0;
                        }
                        break; // break out of multiply_x loop
                    }
                    else
                    {
                        temp_take_piece[coordinate_x + (1 * multiply_x)][coordinate_y + (1 * multiply_y)] = 'v'; //delete what was there so its not picked up next time
                    }
                }
            }
            if (piece_to_take_found == 1)
            {
                break; // break out of multiply_ y loop
            }
        }
        if (multiply_x > 1 && multiply_y > 1)
        {
            value_to_return = -1;
            break_loop = 0;
        }
        if ((coordinate_x == new_x) && (coordinate_y == new_y)) // end of move
        {
            value_to_return = 0;
            break_loop = 0;
        }
    }
    if (value_to_return == 0)
    {
        if(update_piece_position(old_x, old_y, new_x, new_y) == 0)
        {
            transfer_temp_array(temp_take_piece, 8);
        }
        else
        {
            value_to_return = -1;
        }
    }

    return value_to_return;
}

bool check_diagonal_square(char temp_array[][8], int x, int y)
{
    if (temp_array[x][y] =='b' || temp_array[x][y] =='B' || temp_array[x][y] =='w' || temp_array[x][y] =='W')
    {
        return true;
    }
    else
    {
        return false;
    }
}

int count_taken_pieces(void)
{
    int i, taken_piece_count = 0;

    for (i = 0; i < 12; i++)
    {
        if (whos_turn == false) // whites go so count black pieces
        {
            if (black[i].taken == true)
            {
                taken_piece_count++;
            }
        }
    }

    return taken_piece_count;
}

void game_over(int x)
{
    char player_win[] = "Black  wins";
    clear_screen();
    if (x == 9)
    {
        whos_turn ? strcpy(player_win, "White  Wins") : strcpy(player_win, "Black  Wins");
        printf("\n\n\n\n\n\n\t\t\t Game Over\n\n\t\t\t%s\n\n\n", player_win);
    }
    else
    {
        whos_turn ? strcpy(player_win, "Black  Wins") : strcpy(player_win, "White  Wins");
        printf("\n\n\n\n\n\n\t\t\t Game Over\n\n\t\t\t%s\n\n\n", player_win);
    }
}





