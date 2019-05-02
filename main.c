/*********************************************************
 * Welcome to checkers designed and proggramed by me.    *
 * With thanks to all that helped me on the c proggraming*
 * forum.  Hope you enjoy.                               *
 *********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "moves.h"
#include "board.h"

int main()
{
    char answer;

    do
    {
        initialize();
        clear_screen();
        draw_board();
        play_game();
    } while ( answer == 'y' || 'Y');

    printf("Thank you for playing");


    return 0;
}
