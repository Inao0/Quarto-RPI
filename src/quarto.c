#include "quarto.h"


int victory(char quarto_board[4][4]) {
    int victory = 0;
    // Check lines and columns
    for (int i = 0; i < 4; ++i) {
        if ((quarto_board[i][0] & quarto_board[i][1] & quarto_board[i][2] & quarto_board[i][3])
            || (quarto_board[0][i] & quarto_board[1][i] & quarto_board[2][i] & quarto_board[3][i])) {
            victory = 1;
        }
    }
    //Check diagonnals
    if ((quarto_board[0][0] & quarto_board[1][1] & quarto_board[2][2] & quarto_board[3][3])
        || (quarto_board[0][3] & quarto_board[1][2] & quarto_board[2][1] & quarto_board[3][0])) {
        victory = 1;
    }
    return (victory);
}

void initialise_game(char quarto_board[4][4], char remaining_pawns[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            quarto_board[i][j] = -1;
        }
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            remaining_pawns[i][j] = 4 * i + j;
        }
    }
}

int is_orange(int pawn) {
    return (pawn & 0x01);
}

int is_square(int pawn) {
    return (pawn & 0x02);
}

int is_big(int pawn) {
    return (pawn & 0x04);
}

int is_hollow(int pawn) {
    return (pawn & 0x08);
}