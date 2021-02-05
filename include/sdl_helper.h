//
// Created by inao on 29/01/2021.
//

#ifndef QUARTO_RPI_SDL_HELPER_H
#define QUARTO_RPI_SDL_HELPER_H

void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);

void fill_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);

void draw_quarto_board(SDL_Renderer *renderer, char quarto_board[4][4]);

void draw_remaining_pawns(SDL_Renderer *renderer, char remaining_pawns[4][4]);

void draw_current_pawn(SDL_Renderer *renderer, int pawn);

void draw_pawn(SDL_Renderer *renderer, int pawn, int centre_x, int centre_y, SDL_Color background_color);

#endif //QUARTO_RPI_SDL_HELPER_H
