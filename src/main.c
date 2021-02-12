#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "ncurses.h"
#include "sdl_helper.h"
#include "quarto.h"

#define RED 0
#define N_CHOICES 5
#define EXIT "EXIT"
#define EXIT_INDEX N_CHOICES+1
#define NUMBER_OF_SDL_MESSAGES 3
char *choices_right[] = {"New local game", "New online game", "Rules", "About", EXIT};


#define SDL_CHECK(status, msg)                        \
  if (0 != (status))   {                            \
    fprintf(stderr, "%s : %s\n", msg, SDL_GetError());  \
    exit (EXIT_FAILURE);                            \
  }

#define SDL_TTF_CHECK(status, msg)                        \
  if (0 != (status))   {                            \
    fprintf(stderr, "%s : %s\n", msg, TTF_GetError());  \
    exit (EXIT_FAILURE);                            \
  }


int main() {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    char quarto_board[4][4];
    char remaining_pawns[4][4];
    char next_pawn = -1;

    initialise_game(quarto_board, remaining_pawns);

    // SDL_Color variables
    SDL_Color orange = {255, 140, 0, 255};
    SDL_Color light_orange = {255, 215, 167, 255};

    // Message variables
    SDL_Rect messages_rect[NUMBER_OF_SDL_MESSAGES];
    SDL_Texture *messages_textures[NUMBER_OF_SDL_MESSAGES];

    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO), "Erreur SDL_Init");
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1024, 600, SDL_WINDOW_SHOWN);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_CHECK(window == NULL, "Erreur SDL_CreateWindow");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_CHECK(NULL == renderer, "Erreur SDL_CreateRenderer");

    SDL_TTF_CHECK(TTF_Init() == -1, "TTF_Init");
    TTF_Font *Sans = TTF_OpenFont("./assets/Moonglade.ttf", 60); //this opens a font style and sets a size
    SDL_TTF_CHECK(TTF_Init() == -1, "TTF_OpenFont");

    initialise_messages(renderer, Sans, messages_textures, messages_rect);

    SDL_CHECK(SDL_SetRenderDrawColor(renderer, light_orange.r, light_orange.g, light_orange.b, light_orange.a),
              "Erreur SDL_SetRenderDrawColor");
    SDL_CHECK(SDL_RenderClear(renderer), "Erreur SDL_RenderClear");
    SDL_RenderPresent(renderer);
    for (int i = 0; i < NUMBER_OF_SDL_MESSAGES; ++i) {
        printf("Rectancle %d %d %d\n",  messages_rect[i].x,messages_rect[i].y,messages_rect[i].h);
    }

    draw_game(renderer, quarto_board, remaining_pawns, next_pawn, messages_rect, messages_textures,
              NUMBER_OF_SDL_MESSAGES);

    int quit = 0;

    //while personne n'a gagné :
  //  while (victory(quarto_board) && !quit) {

//    }


    statut = EXIT_SUCCESS;
    SDL_Delay(3000);

    for (int i = 0; i < NUMBER_OF_SDL_MESSAGES; ++i) {
        SDL_DestroyTexture(messages_textures[i]);
    }
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



