#include "SDL2/SDL.h"
#include <math.h>
#include "sdl_helper.h"

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


void fill_circle(SDL_Renderer *renderer, int cx, int cy, int radius) {
    int dy, dx;
    for (dy = 1; dy < radius; dy += 1) {
        //Inspiré de : https://gist.github.com/derofim/912cfc9161269336f722
        // This loop is unrolled a bit, only iterating through half of the
        // height of the circle.  The result is used to draw a scan line and
        // its mirror image below it.

        // The following formula has been simplified from our original.  We
        // are using half of the width of the circle because we are provided
        // with a center and we need left/right coordinates.

        dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
        SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);
    }
    // On a sorti la dernière itération pour éviter un pixel qui dépasse sur la ligne horizontale.
    SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
    SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);

}


// Joy-Pi 7'' inch screen resolution : 1024x600
void draw_quarto_board(SDL_Renderer *renderer, char quarto_board[4][4]) {
    int center_x, center_y;
    SDL_Color white = {255, 255, 255, 255};

    for (int i = 0; i < 4; ++i) {
        center_x = 150 + (i * 110);
        for (int j = 0; j < 4; ++j) {
            center_y = 170 + (j * 110);
            if (0 != SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
            fill_circle(renderer, center_x, center_y, 50);
            if (quarto_board[i][j] >= 0) {
                draw_pawn(renderer, quarto_board[i][j], center_x, center_y, white);
            }

        }
    }
}


// Joy-Pi 7'' inch screen resolution : 1024x600
void draw_remaining_pawns(SDL_Renderer *renderer, char remaining_pawns[4][4]) {
    int center_x, center_y;
    SDL_Color light_orange = {255, 215, 167, 255};

    for (int i = 0; i < 4; ++i) {
        center_x = 700 + (i * 70);
        for (int j = 0; j < 4; ++j) {
            center_y = 290 + (j * 70);
            if (0 != SDL_SetRenderDrawColor(renderer, light_orange.r, light_orange.g, light_orange.b, light_orange.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
            if (remaining_pawns[i][j] >= 0) {
                draw_pawn(renderer, remaining_pawns[i][j], center_x, center_y, light_orange);
            }

        }
    }
}

void draw_current_pawn(SDL_Renderer *renderer, int pawn) {
    int center_x = 798;
    int center_y = 170;
    SDL_Color light_orange = {255, 215, 167, 255};

    draw_pawn(renderer, pawn, center_x, center_y, light_orange);
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

void draw_pawn(SDL_Renderer *renderer, int pawn, int centre_x, int centre_y, SDL_Color background_color) {
    int size = (is_big(pawn)) ? 30 : 17;

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
            int hole_size = (is_big(pawn)) ? 20 : 5;
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
            int hole_size = (is_big(pawn)) ? 20 : 5;
            if (0 != SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b,
                                            background_color.a)) {
                fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
            }
            fill_circle(renderer, centre_x, centre_y, hole_size - 1);
        }
    }
}