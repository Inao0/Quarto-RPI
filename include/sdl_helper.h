//
// Created by inao on 29/01/2021.
//

#ifndef QUARTO_RPI_SDL_HELPER_H
#define QUARTO_RPI_SDL_HELPER_H

void draw_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);

void fill_circle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius);

void draw_quarto_board(SDL_Renderer *renderer, signed char quarto_board[4][4]);

void draw_remaining_pawns(SDL_Renderer *renderer, signed char remaining_pawns[4][4]);

void draw_current_pawn(SDL_Renderer *renderer, signed char pawn);

void draw_pawn(SDL_Renderer *renderer, signed char pawn, int centre_x, int centre_y, SDL_Color background_color);

void initialise_messages(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *messages_textures[3],
                         SDL_Rect messages_rect[3]);

void initialise_sdl_ttf_quarto_message(SDL_Renderer *renderer, SDL_Texture **quartoMessageTexture,
                                       SDL_Rect *quartoMessage_rect, TTF_Font *font);

void initialise_sdl_ttf_remaining_message(SDL_Renderer *renderer, SDL_Texture **remainingMessageTexture,
                                          SDL_Rect *remainingMessage_rect, TTF_Font *font);

void
initialise_sdl_ttf_next_message(SDL_Renderer *renderer, SDL_Texture **nextMessageTexture, SDL_Rect *nextMessage_rect,
                                TTF_Font *font);


void draw_game(SDL_Renderer *renderer, signed char quarto_board[4][4], signed char remaining_pawns[4][4],
               signed char selected_pawn, struct SDL_Rect *messages_rect, SDL_Texture **messages_textures,
               int number_of_messages);

void draw_victory(SDL_Renderer *renderer, signed char victory);

#endif //QUARTO_RPI_SDL_HELPER_H
