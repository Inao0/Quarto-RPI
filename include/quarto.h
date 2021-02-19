#ifndef QUARTO_RPI_QUARTO_H
#define QUARTO_RPI_QUARTO_H

int match(signed char a, signed char b, signed char c, signed char d);

int victory(signed char quarto_board[4][4]);

int is_orange(signed char pawn);

int is_square(signed char pawn);

int is_big(signed char pawn);

int is_hollow(signed char pawn);

void initialise_game(signed char quarto_board[4][4], signed char remaining_pawns[4][4]);

#endif //QUARTO_RPI_QUARTO_H
