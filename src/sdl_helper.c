#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <math.h>
#include "sdl_helper.h"
#include "quarto.h"


/* input : a SDL renderer, the center of the circle to draw and its radius
 *
 * Draw a circle of the given radius centered in (centreX,centreY) in the SDL_Renderer
 * (Midpoint circle algorithm)
 */
void draw_circle(SDL_Renderer *renderer, int centreX, int centreY, int radius) {
    const int diameter = (radius * 2);

    int x = (radius - 1);
    int y = 0;
    int tx = 1;
    int ty = 1;
    int error = (tx - diameter);

    while (x >= y) {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}


/* input : a SDL renderer, the center of the circle to draw and fill and its radius
 *
 * Draw a disc of the given radius centered in (centreX,centreY) in the SDL_Renderer
 * Inspired by : https://gist.github.com/derofim/912cfc9161269336f722
 */
void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int dy, dx;
    for (dy = 1; dy < radius; dy += 1) {
        dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
        SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
    }
    // The last iteration do not compute the new length to avoid a pixel sticking out on the middle horizontal line
    SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
    SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);

}



/* input : a SDL renderer and a 4x4 char table corresponding to a quarto board
 *
 * Draw a the white discs corresponding to the quarto board spaces
 * Draw the pawns from the char table on top of those white spaces
 */
void draw_quarto_board(SDL_Renderer *renderer, signed char quarto_board[4][4]) {
    int center_x, center_y;
    SDL_Color white = {255, 255, 255, 255};

    for (int i = 0; i < 4; ++i) {
        center_x = 300 + (i * 220);
        for (int j = 0; j < 4; ++j) {
            center_y = 280 + (j * 220);
            if (0 != SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
            fill_circle(renderer, center_x, center_y, 100);
            draw_pawn(renderer, quarto_board[i][j], center_x, center_y, white);

        }
    }
}


/* input : a SDL renderer and a 4x4 char table corresponding to the remaining pawns
 *
 * Draw a the remaining pawns on the bottom right part of the screen.
 */
void draw_remaining_pawns(SDL_Renderer *renderer, signed char remaining_pawns[4][4]) {
    int center_x, center_y;
    SDL_Color light_orange = {255, 215, 167, 255};

    for (int i = 0; i < 4; ++i) {
        center_x = 1280 + (i * 165);
        for (int j = 0; j < 4; ++j) {
            center_y = 445 + (j * 165);
            if (0 != SDL_SetRenderDrawColor(renderer, light_orange.r, light_orange.g, light_orange.b, light_orange.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
            draw_pawn(renderer, remaining_pawns[i][j], center_x, center_y, light_orange);
        }
    }
}

/* input : a SDL renderer and a pawn
 *
 * Draw the pawn as the next pawn to be placed on the board on the top rigth part of the screen
 */
void draw_current_pawn(SDL_Renderer *renderer, signed char pawn) {
    int center_x = 1575;
    int center_y = 190;
    SDL_Color light_orange = {255, 215, 167, 255};

    draw_pawn(renderer, pawn, center_x, center_y, light_orange);
}

/* input : a SDL renderer, a pawn, a position (centre_x, centre_y) and the color of the background at that position
 *
 * Draw the pawn at the given position (centre_x, centre_y).
 */
void draw_pawn(SDL_Renderer *renderer, signed char pawn, int centre_x, int centre_y, SDL_Color background_color) {
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b,
                           background_color.a);
    SDL_Rect square_pawn;
    square_pawn.x = centre_x - 70;
    square_pawn.y = centre_y - 70;
    square_pawn.w = 70 * 2;
    square_pawn.h = 70 * 2;
    SDL_RenderFillRect(renderer, &square_pawn);
    if (pawn > 0) {
        int size = (is_big(pawn)) ? 70 : 50;
        int hole_size = (is_big(pawn)) ? 50 : 30;

        SDL_Color orange = {255, 140, 0, 255};
        SDL_Color brown = {170, 93, 0, 255};


        if (is_orange(pawn)) {
            if (0 != SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
        } else {
            if (0 != SDL_SetRenderDrawColor(renderer, brown.r, brown.g, brown.b, brown.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
        }

        if (is_square(pawn)) {
            SDL_Rect square_pawn;
            square_pawn.x = centre_x - size;
            square_pawn.y = centre_y - size;
            square_pawn.w = size * 2;
            square_pawn.h = size * 2;
            SDL_RenderFillRect(renderer, &square_pawn);
            if (is_hollow(pawn)) {
                if (0 != SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b,
                                                background_color.a)) {
                    fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
                }
                square_pawn.x = centre_x - hole_size;
                square_pawn.y = centre_y - hole_size;
                square_pawn.w = hole_size * 2;
                square_pawn.h = hole_size * 2;
                SDL_RenderFillRect(renderer, &square_pawn);
            }
        } else {
            fill_circle(renderer, centre_x, centre_y, size);
            if (is_hollow(pawn)) {
                if (0 != SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b,
                                                background_color.a)) {
                    fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
                }
                fill_circle(renderer, centre_x, centre_y, hole_size - 1);
            }
        }
    }
}

/* input : a SDL renderer, a ttf font pointer, two tables store the SDL_Texture and SDL_Rectangle
 *
 * Initialise all text textures for the three text displayed on the screen and store the corresponding
 * textures and rectangles in the two tables.
 */
void initialise_messages(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *messages_textures[3],
                         SDL_Rect messages_rect[3]) {
    initialise_sdl_ttf_quarto_message(renderer, &messages_textures[0], &messages_rect[0], font);
    initialise_sdl_ttf_remaining_message(renderer, &messages_textures[1], &messages_rect[1], font);
    initialise_sdl_ttf_next_message(renderer, &messages_textures[2], &messages_rect[2], font);

}

/* input : a SDL renderer, a ttf font pointer, a SDL_Texture and a SDL_Rectangle
 *
 * Initialise the "Quarto" message on the top left area of the screen
 */
void initialise_sdl_ttf_quarto_message(SDL_Renderer *renderer, SDL_Texture **quartoMessageTexture,
                                       SDL_Rect *quartoMessage_rect, TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *quartoMessage = TTF_RenderText_Solid(font, "Quarto",
                                                      white); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    *quartoMessageTexture = SDL_CreateTextureFromSurface(renderer,
                                                         quartoMessage); //now you can convert it into a texture


    quartoMessage_rect->x = 200;  //controls the rect's x coordinate
    quartoMessage_rect->y = 5; // controls the rect's y coordinte
    quartoMessage_rect->w = 525; // controls the width of the rect
    quartoMessage_rect->h = 135; // controls the height of the rect

}

/* input : a SDL renderer, a ttf font pointer, a SDL_Texture and a SDL_Rectangle
 *
 * Initialise the "Remaining Pawns" message on the bottom right area of the screen
 */
void initialise_sdl_ttf_remaining_message(SDL_Renderer *renderer, SDL_Texture **remainingMessageTexture,
                                          SDL_Rect *remainingMessage_rect, TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *remainingMessage = TTF_RenderText_Solid(font, "Remaining Pawns :", white);
    *remainingMessageTexture = SDL_CreateTextureFromSurface(renderer, remainingMessage);

    remainingMessage_rect->x = 1200;  //controls the rect's x coordinate
    remainingMessage_rect->y = 290; // controls the rect's y coordinte
    remainingMessage_rect->w = 400; // controls the width of the rect
    remainingMessage_rect->h = 50; // controls the height of the rect
}

/* input : a SDL renderer, a ttf font pointer, a SDL_Texture and a SDL_Rectangle
 *
 * Initialise the "Next Pawn" message on the top right area of the screen
 */
void
initialise_sdl_ttf_next_message(SDL_Renderer *renderer, SDL_Texture **nextMessageTexture, SDL_Rect *nextMessage_rect,
                                TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *nextMessage = TTF_RenderText_Solid(font, "Next Pawn :", white);
    *nextMessageTexture = SDL_CreateTextureFromSurface(renderer, nextMessage);

    nextMessage_rect->x = 1200;  //controls the rect's x coordinate
    nextMessage_rect->y = 50; // controls the rect's y coordinte
    nextMessage_rect->w = 250; // controls the width of the rect
    nextMessage_rect->h = 50; // controls the height of the rect
}


/* input :  a SDL renderer,
 *          a 4x4 table corresponding to the quarto board,
 *          a 4x4 table corresponding to the remaining pawns,
 *          a char corresponding to the next pawn to be placed on the board
 *          a table of SDL_Rect corresponding to text positions
 *          a table of SDL_Texture which are the corresponding text textures
 *          the number of text store in the two previous tables
 *
 * Draw the whole board, remaining pawn, next pawn to be placed and text messages.
 * Display/Render it.
 */
void draw_game(SDL_Renderer *renderer, signed char quarto_board[4][4], signed char remaining_pawns[4][4],
               signed char selected_pawn, struct SDL_Rect *messages_rect, struct SDL_Texture **messages_textures,
               int number_of_messages) {
    draw_quarto_board(renderer, quarto_board);
    draw_remaining_pawns(renderer, remaining_pawns);
    draw_current_pawn(renderer, selected_pawn);

    for (int i = 0; i < number_of_messages; ++i) {
        SDL_RenderCopy(renderer, messages_textures[i], NULL, &(messages_rect[i]));
    }
    SDL_RenderPresent(renderer);
}



/* input : a SDL renderer and a victory code (cf. src/quarto.c for and the output of victory())
 *
 * Draw a green line on top of the pawns with a matching attribute (according to the victory char)
 * and render it.
 */
void draw_victory(SDL_Renderer *renderer, signed char victory) {
    SDL_Color light_green = {173, 255, 188, 150};

    if (0 != SDL_SetRenderDrawColor(renderer, light_green.r, light_green.g, light_green.b,
                                    light_green.a)) {
        fprintf(stdout, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
    }

    if (victory < 20) {
        // Draw an horizontal line
        int y_start = 280;
        int y_end = 280 + 3 * 220;
        int x_middle = 300 + ((victory-10) * 220);
        SDL_RenderDrawLine(renderer, x_middle, y_start, x_middle, y_end);
        // The for loop allow to get a thick line has SDL_RenderDrawLine only draw 1px wide lines.
        for (int k = 0; k < 15; ++k) {
            SDL_RenderDrawLine(renderer, x_middle - k, y_start, x_middle - k, y_end);
            SDL_RenderDrawLine(renderer, x_middle + k, y_start, x_middle + k, y_end);
        }
    } else if (victory < 30) {
        // Draw a vertical line
        int x_start = 300;
        int x_end = 300 + 3 * 220;
        int y_middle = 280 + ((victory - 20) * 220);
        SDL_RenderDrawLine(renderer, x_start, y_middle, x_end, y_middle);
        for (int k = 0; k < 15; ++k) {
            SDL_RenderDrawLine(renderer, x_start, y_middle - k, x_end, y_middle - k);
            SDL_RenderDrawLine(renderer, x_start, y_middle + k, x_end, y_middle + k);
        }
    } else if (victory < 40) {
        // Draw a diagonal from top left to bottom right
        int y_start = 280;
        int y_end = 280 + 3 * 220;
        int x_start = 300;
        int x_end = 300 + 3 * 220;
        SDL_RenderDrawLine(renderer, x_start, y_start, x_end, y_end);
        for (int k = 0; k < 15; ++k) {
            SDL_RenderDrawLine(renderer, x_start + k, y_start, x_end, y_end - k);
            SDL_RenderDrawLine(renderer, x_start, y_start + k, x_end - k, y_end);
        }
    } else {
        // Draw a diagonal from top right to bottom left
        int y_start = 280 + 3 * 220;
        int y_end = 280;
        int x_start = 300;
        int x_end = 300 + 3 * 220;
        SDL_RenderDrawLine(renderer, x_start, y_start, x_end, y_end);
        for (int k = 0; k < 15; ++k) {
            SDL_RenderDrawLine(renderer, x_start, y_start - k, x_end - k, y_end);
            SDL_RenderDrawLine(renderer, x_start + k, y_start, x_end, y_end + k);
        }
    }
    SDL_RenderPresent(renderer);
}
