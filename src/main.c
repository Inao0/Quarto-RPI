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

void initialise_sdl_ttf_quarto_message(SDL_Renderer *renderer, SDL_Texture **quartoMessageTexture,
                                       SDL_Rect *quartoMessage_rect, TTF_Font *font);

void initialise_sdl_ttf_remaining_message(SDL_Renderer *renderer, SDL_Texture **remainingMessageTexture,
                                          SDL_Rect *remainingMessage_rect, TTF_Font *font);

void
initialise_sdl_ttf_next_message(SDL_Renderer *renderer, SDL_Texture **nextMessageTexture, SDL_Rect *nextMessage_rect,
                                TTF_Font *font);

int victory(char quarto_board[4][4]);

int main() {

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    char quarto_board[4][4];
    char remaining_pawns[4][4];

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
    // Event variables :
    SDL_Event *event;

    // SDL_Color variables
    SDL_Color orange = {255, 140, 0, 255};
    SDL_Color light_orange = {255, 215, 167, 255};

    // Message variables
    SDL_Rect quartoMessage_rect;
    SDL_Rect remainingMessage_rect;
    SDL_Rect nextMessage_rect;
    SDL_Texture *quartoMessageTexture;
    SDL_Texture *remainingMessageTexture;
    SDL_Texture *nextMessageTexture;

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

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    TTF_Font *Sans = TTF_OpenFont("./assets/Moonglade.ttf", 60); //this opens a font style and sets a size
    if (!Sans) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    initialise_sdl_ttf_quarto_message(renderer, &quartoMessageTexture, &quartoMessage_rect, Sans);
    initialise_sdl_ttf_remaining_message(renderer, &remainingMessageTexture, &remainingMessage_rect, Sans);
    initialise_sdl_ttf_next_message(renderer, &nextMessageTexture, &nextMessage_rect, Sans);

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


    draw_quarto_board(renderer, quarto_board);
    draw_remaining_pawns(renderer, remaining_pawns);
    draw_current_pawn(renderer, rand() % 16);

    SDL_RenderCopy(renderer, quartoMessageTexture, NULL, &quartoMessage_rect);
    SDL_RenderCopy(renderer, remainingMessageTexture, NULL, &remainingMessage_rect);
    SDL_RenderCopy(renderer, nextMessageTexture, NULL, &nextMessage_rect);
    SDL_RenderPresent(renderer);

    int quit = 0;

    //while personne n'a gagné :
    while (victory && !quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            printf("event ! %d - hex : %X\n", event.type, event.type);
            if (event.type == SDL_TEXTINPUT) {
                printf("User just pressed down a key!\n");
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                printf("User just pressed mouse up\n");
                printf("Button x : %d, y: %d\n", event.button.x, event.button.y);
            }
            if (event.type == SDL_QUIT) {
                printf("QUITTING\n");
                quit = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.x >= 100)  // check if it is in the desired area
                {
                    printf("x>=100\n");
                }
                printf("Button x : %d, y: %d\n", event.button.x, event.button.y);

            }
        }

        SDL_WaitEvent(&event);
        // Un joueur sélectionne une pièce
        // Changement de jouer
        // Le joueur joue la pièce
    }


    statut = EXIT_SUCCESS;
    SDL_Delay(5000);

    Quit:

    SDL_DestroyTexture(quartoMessageTexture);
    SDL_DestroyTexture(remainingMessageTexture);
    SDL_DestroyTexture(nextMessageTexture);

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

void initialise_sdl_ttf_quarto_message(SDL_Renderer *renderer, SDL_Texture **quartoMessageTexture,
                                       SDL_Rect *quartoMessage_rect, TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *quartoMessage = TTF_RenderText_Solid(font, "Quarto",
                                                      white); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
    *quartoMessageTexture = SDL_CreateTextureFromSurface(renderer,
                                                         quartoMessage); //now you can convert it into a texture


    quartoMessage_rect->x = 105;  //controls the rect's x coordinate
    quartoMessage_rect->y = 10; // controls the rect's y coordinte
    quartoMessage_rect->w = 275; // controls the width of the rect
    quartoMessage_rect->h = 70; // controls the height of the rect

}

void initialise_sdl_ttf_remaining_message(SDL_Renderer *renderer, SDL_Texture **remainingMessageTexture,
                                          SDL_Rect *remainingMessage_rect, TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *remainingMessage = TTF_RenderText_Solid(font, "Remaining Pawns :", white);
    *remainingMessageTexture = SDL_CreateTextureFromSurface(renderer, remainingMessage);

    remainingMessage_rect->x = 670;  //controls the rect's x coordinate
    remainingMessage_rect->y = 225; // controls the rect's y coordinte
    remainingMessage_rect->w = 200; // controls the width of the rect
    remainingMessage_rect->h = 25; // controls the height of the rect
}

void
initialise_sdl_ttf_next_message(SDL_Renderer *renderer, SDL_Texture **nextMessageTexture, SDL_Rect *nextMessage_rect,
                                TTF_Font *font) {
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *nextMessage = TTF_RenderText_Solid(font, "Next Pawn :", white);
    *nextMessageTexture = SDL_CreateTextureFromSurface(renderer, nextMessage);

    nextMessage_rect->x = 670;  //controls the rect's x coordinate
    nextMessage_rect->y = 100; // controls the rect's y coordinte
    nextMessage_rect->w = 140; // controls the width of the rect
    nextMessage_rect->h = 25; // controls the height of the rect


}

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