#include <cdk.h>
#include <SDL2/SDL_render.h>
#include <cdk/cdkscreen.h>
#include <cdk/label.h>
#include <cdk/scroll.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "cdk_helper.h"
#include "indexes_menu.h"
#include <unistd.h>

#ifndef BUILD_PC
#include "wiring_helper.h"
#endif

#include "indexes_menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <wiring_helper.h>

#include "sdl_helper.h"
#include "quarto.h"

#define HELP "<C></U></D></B>Select your choice<!B>"
#define TITLE "<C>QUARTO - RPi<!C>"
#define HEIGHT (LIST_SIZE+10)
#define WIDTH 50
#define FOREGROUND 86
#define BACKGROUND 15
#define DEFAULT_PAIR 87

#define RED 0
#define NUMBER_OF_SDL_MESSAGES 3

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

int SDL_Quarto(int *winner);

int main() {

    CDKSCREEN *cdkscreen, *cdkSubScreen;
    CDKSCROLL *scroll = 0;
    CDKLABEL *header;
    WINDOW *subWindow;

    const char *items[] = {"", " <#DI> New local game ", "", " <#DI> New online game ", "", " <#DI> Rules ", "",
                           " <#DI> About ", "", " <#DI> Shutdown ", ""};
    const char *local_loading_text = "Loading...";
    const char *online_loading_text = "Waiting for a second RPi - Not implemented";
    const char *list[1];
    list[0] = TITLE;
    int willExit = 0;
    char name1[50];
    char name2[50];
    int selected;
    int winner;

    /* Initialize the terminal with ncurses and cdk */
    cdkscreen = initCDKScreen(NULL);
    curs_set(0); // hide cursor
    initCDKColor(); /* Set up CDK Colors. */
    init_color(FOREGROUND, 1000, 550, 0);
    init_pair(200, COLOR_BLACK, BACKGROUND);
    assume_default_colors(FOREGROUND, BACKGROUND);
    subWindow = newwin(HEIGHT, WIDTH, (LINES - HEIGHT) / 2, (COLS - WIDTH) / 2);
    refresh();
    cdkSubScreen = initCDKScreen(subWindow);

    /* Title to be placed on top */
    header = newCDKLabel(cdkSubScreen, CENTER, TOP, (CDK_CSTRING2) list, 1, TRUE, FALSE);
    if (header != 0)
        activateCDKLabel(header, 0);

    /* Main menu */
    scroll = newCDKScroll(cdkSubScreen, CENTER, CENTER, RIGHT, LIST_SIZE + 3, 50, HELP, (CDK_CSTRING2) items, LIST_SIZE,
                          false, A_REVERSE, TRUE, TRUE);
    if (scroll == 0) {
        destroyCDKScreen(cdkSubScreen);
        endCDK();

        printf("Cannot create the scroll list.\n");
        printf("Is the window too small?\n");
        exit(EXIT_FAILURE);
    }
    setCDKScrollCurrent(scroll, 1);
    setCDKScrollPostProcess(scroll, postProcessScroll, 0);

    /* Main menu treatment */
    while (willExit == 0) {
        drawCDKScreen(cdkSubScreen);
        selected = activateCDKScroll(scroll, 0);
        if (scroll->exitType == vESCAPE_HIT) {
            willExit = 1;
            continue;
        }
        eraseCDKScreen(cdkSubScreen);
        switch (selected) {
            case LOCAL:
            askForPlayers:
                if (askForPlayer(cdkscreen, "<C>Enter Player 1 name\n<C><#LT><#HL(30)><#RT>", name1) == EXIT_FAILURE)
                    continue;
                if (askForPlayer(cdkscreen, "<C>Enter Player 2 name\n<C><#LT><#HL(30)><#RT>", name2) == EXIT_FAILURE)
                    goto askForPlayers;
                displayMarquee(cdkSubScreen, local_loading_text, name1);
                SDL_Quarto(&winner);
                if (winner != 0) {
                  showWinner(cdkscreen, winner, name1, name2);
                } else {
                   showExAequo(cdkscreen);
                }
                break;
            case ONLINE:
                askForPlayer(cdkscreen, "Enter your name", name1);
                displayMarquee(cdkSubScreen, online_loading_text, name1);
                break;
            case RULES:
                displaySlide(cdkscreen, RULES, 13, "<C></B/U/D>Rules (Short, I promise)<!D>");
                break;
            case ABOUT:
                displaySlide(cdkscreen, ABOUT, 29, "<C></B/U/D>About QuartoRpi<!D>");
                break;
            case EXIT:
                willExit = 1;
                break;
        }
    }

    /* Clean up. */
    destroyCDKScreenObjects(cdkscreen);
    destroyCDKScreen(cdkscreen);
    endCDK();
#ifndef BUILD_PC
    system("shutdown -h now");
#endif
    exit(EXIT_SUCCESS);
}

/* Play a quarto game */
int SDL_Quarto(int *winner) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;
    signed char quarto_board[4][4];
    signed char remaining_pawns[4][4];
    signed char next_pawn = 0;

    initialise_game(quarto_board, remaining_pawns);

    // SDL_Color variables
    SDL_Color light_orange = {255, 215, 167, 255};

    // Message variables
    SDL_Rect messages_rect[NUMBER_OF_SDL_MESSAGES];
    SDL_Texture *messages_textures[NUMBER_OF_SDL_MESSAGES];

    SDL_CHECK(SDL_Init(SDL_INIT_VIDEO), "Erreur SDL_Init");
    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              1920, 1080, SDL_WINDOW_FULLSCREEN);
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
//    for (int i = 0; i < NUMBER_OF_SDL_MESSAGES; ++i) {
//        printf("Rectangle %d %d %d\n", messages_rect[i].x, messages_rect[i].y, messages_rect[i].h);
//    }


    draw_game(renderer, quarto_board, remaining_pawns, next_pawn, messages_rect, messages_textures,
              NUMBER_OF_SDL_MESSAGES);

    int compteur = 0;
    int i, j;
    int victory_condition = 0;


    //while personne n'a gagné :
    while (victory_condition == 0 && compteur < 16) {
        while (next_pawn == 0) {
            //sleep(1);
            #ifndef BUILD_PC
            getButton(&j, &i);
            #endif
            next_pawn = remaining_pawns[i][j];
            remaining_pawns[i][j] = 0;
        }
        draw_game(renderer, quarto_board, remaining_pawns, next_pawn, messages_rect, messages_textures,
                  NUMBER_OF_SDL_MESSAGES);

        while (next_pawn > 0) {
            //sleep(1);
            #ifndef BUILD_PC
            getButton(&j, &i);
            #endif
            if (quarto_board[i][j] == 0) {
                quarto_board[i][j] = next_pawn;
                next_pawn = 0;
            }
        }
        draw_game(renderer, quarto_board, remaining_pawns, next_pawn, messages_rect, messages_textures,
                  NUMBER_OF_SDL_MESSAGES);

        victory_condition = victory(quarto_board);
        compteur++;
    }
    if (victory_condition) {
        draw_victory(renderer, victory_condition);
        *winner = 1 + (compteur % 2);
    } else {
        *winner = 0;
    }
    statut = EXIT_SUCCESS;
    SDL_Delay(5000);


    for (int i = 0; i < NUMBER_OF_SDL_MESSAGES; ++i) {
        SDL_DestroyTexture(messages_textures[i]);
    }
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}
