#include "quarto.h"


/* input : a quartoboard
 * output : 0 if there is no line that match the victory condition
 *          10 + i if the line i has four pawns with a common attribute
 *          20 + i if the column i has four pawns with a common attribute
 *          30 if the column from top left to bottom right has four pawns with a common attribute
 *          40 if the column from top right to bottom left has four pawns with a common attribute
 *          */
int victory(signed char quarto_board[4][4]) {
    for (int i = 0; i < 4; ++i) {
        // Check lines
        if (match(quarto_board[i][0], quarto_board[i][1], quarto_board[i][2], quarto_board[i][3])) {
            return (i + 10);
        }
        // Check columns
        if (match(quarto_board[0][i], quarto_board[1][i], quarto_board[2][i], quarto_board[3][i])) {
            return (i + 20);
        }
    }
    //Check diagonnals
    if (match(quarto_board[0][0],quarto_board[1][1],quarto_board[2][2],quarto_board[3][3])) {
        return (30);
    } else if (match(quarto_board[0][3],quarto_board[1][2],quarto_board[2][1],quarto_board[3][0])) {
        return (40);
    }
    return (0);
}


/* input : four char corresponding to pawns or empty spaces on the board
 * output : 0 if there is no common attribute or if at least one is an empty space
 *          1 if there is at least one common attribute to the four pawns
 */
int match(signed char a, signed char b, signed char c, signed char d) {
    // If there is a pawn on each places
    if (a & b & c & d & 0x10) {
        // If the pawns have at least one common attribute
        if (((~((a ^ b) | (a ^ c) | (a ^ d))) & 0x0F) > 0) {
            return (1);
        }
    }
    return (0);
}
/* input : two 4x4 char tables
 * output :  the first one is an empty quarto board filled with 0
 *           the second one is filled with the 16 different pawns of the Quarto game
 */
void initialise_game(signed char quarto_board[4][4], signed char remaining_pawns[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            quarto_board[i][j] = 0;
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            remaining_pawns[i][j] = 4 * i + j + 0x10;
        }
    }
}

/* input : a char corresponding to a pawn
 * output : 1 if the pawn is orange
 *          0 if the pawn is brown
 */
int is_orange(signed char pawn) {
    return (pawn & 0x01);
}

/* input : a char corresponding to a pawn
 * output : 1 if the pawn is square
 *          0 if the pawn is round
 */
int is_square(signed char pawn) {
    return (pawn & 0x02);
}


/* input : a char corresponding to a pawn
 * output : 1 if the pawn is big
 *          0 if the pawn is small
 */
int is_big(signed char pawn) {
    return (pawn & 0x04);
}


/* input : a char corresponding to a pawn
 * output : 1 if the pawn is hollow
 *          0 if the pawn is full
 */
int is_hollow(signed char pawn) {
    return (pawn & 0x08);
}