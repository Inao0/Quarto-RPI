#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ncurses.h"
#include "sdl_helper.h"

#define RED 0
#define N_CHOICES 5
#define EXIT "EXIT"
#define EXIT_INDEX N_CHOICES+1

char *choices_right[] = {"New local game", "New online game", "Rules", "About", EXIT};

int main() {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    char quarto_board[4][4];
    char remaining_pawns[4][4];

    SDL_Color orange = {255, 140, 0, 255};
    SDL_Color light_orange = {255, 215, 167, 255};
    SDL_Color white = {255, 255, 255, 255};


    if (0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1024, 600, SDL_WINDOW_SHOWN);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    if (NULL == window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }
    if (0 != SDL_SetRenderDrawColor(renderer, light_orange.r, light_orange.g, light_orange.b, light_orange.a)) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    if (0 != SDL_RenderClear(renderer)) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }

    SDL_Delay(500);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);

    if (0 != SDL_SetRenderDrawColor(renderer, orange.r, orange.g, orange.b, orange.a)) {
        fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s", SDL_GetError());
        goto Quit;
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            quarto_board[i][j] = -1;
        }
    }

    quarto_board[2][3] = 7;
    quarto_board[2][1] = 2;
    quarto_board[0][2] = 0;
    quarto_board[3][3] = 13;

    remaining_pawns[2][3] = 7;
    remaining_pawns[2][1] = 2;
    remaining_pawns[0][2] = 0;
    remaining_pawns[3][3] = 13;

    draw_quarto_board(renderer, quarto_board);
    draw_remaining_pawns(renderer, remaining_pawns);
    draw_current_pawn(renderer, rand() % 16);

    SDL_RenderPresent(renderer);

    statut = EXIT_SUCCESS;
    SDL_Delay(6000);

    Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;


    WINDOW *menu_win;
    int highlight, input, choice;

    initscr();
    clear(); /* Clear shell */
    noecho(); /* Prevent inputs to be printed */
    cbreak();    /* Buffer has only one input. Signals (ex: ^C) are transmitted. */
    curs_set(0); /* Hide cursor */
    if (has_colors()) { /* Shell supports colors */
        start_color(); /* Allows to use colors */
        use_default_colors(); /* Mandatory to use the right color */
        init_pair(COLOR_RED, RED, COLOR_BLACK);
    }

    // Accept user input
    keypad(menu_win, TRUE);

    mvprintw(0, 0, "Use arrow keys to navigate.\nPress ENTER to select a choice.");
    refresh();

    // Displaying choice menu
    wattron(menu_win, A_BOLD); // Bold caracters
    wattron(menu_win, COLOR_PAIR(RED));
    box(menu_win, 0, 0);
    wrefresh(menu_win);

    while (1) {
        input = wgetch(menu_win); // attends un input utilisateur
        switch (input) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = EXIT_INDEX;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == EXIT_INDEX)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // enter
                choice = highlight;
                break;
            default:
                mvprintw(getmaxy(stdscr) - 1, 0, "Unauthorized key. You pressed '%c'", input);
                wrefresh(stdscr);
                break;
        }

        if (choice == EXIT_INDEX)    /* On quitte le programme */
            break;
        else if (choice != 0) {
            box(menu_win, 0, 0);
            wrefresh(menu_win);
            choice = 0;
        }
    }
    endwin();
    return 0;

}