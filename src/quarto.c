#include "quarto.h"


int victory(signed char quarto_board[4][4]) {
    for (int i = 0; i < 4; ++i) {
        if (match(quarto_board[i][0], quarto_board[i][1], quarto_board[i][2], quarto_board[i][3])) {
            return (i + 10);
        }
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

int match(signed char a, signed char b, signed char c, signed char d) {
    if (a & b & c & d & 0x10) {
        //printf("%X\n",((~((a ^ b) | (a ^ c) | (a ^ d))) & 0x0F));
        if (((~((a ^ b) | (a ^ c) | (a ^ d))) & 0x0F) > 0) {
            return (1);
        }
    }
    return (0);
}

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

int is_orange(signed char pawn) {
    return (pawn & 0x01);
}

int is_square(signed char pawn) {
    return (pawn & 0x02);
}

int is_big(signed char pawn) {
    return (pawn & 0x04);
}

int is_hollow(signed char pawn) {
    return (pawn & 0x08);
}