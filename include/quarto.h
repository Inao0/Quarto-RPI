#ifndef QUARTO_RPI_QUARTO_H
#define QUARTO_RPI_QUARTO_H

int victory(char quarto_board[4][4]);

int is_orange(int pawn);

int is_square(int pawn);

int is_big(int pawn);

int is_hollow(int pawn);

void initialise_game(char quarto_board[4][4], char remaining_pawns[4][4]);

#endif //QUARTO_RPI_QUARTO_H
