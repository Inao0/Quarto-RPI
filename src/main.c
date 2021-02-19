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

#ifndef BUILD_PC
   #include "wiring_helper.h"
#endif

#include "indexes_menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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

int SDL_Quarto ();

int main() {

 CDKSCREEN *cdkscreen, *cdkSubScreen;
   CDKSCROLL *scroll = 0;
   CDKLABEL *header;
   WINDOW* subWindow;

   const char *items[] = {""," <#DI> New local game ", "", " <#DI> New online game ", "", " <#DI> Rules ", "", " <#DI> About ", "", " <#DI> Shutdown ", ""};
   const char * local_loading_text = "Loading...";
   const char * online_loading_text = "Waiting for a second RPi - Not implemented";
   const char *list[1];
   list[0] = TITLE;
   int i = 0;
   char name1[50];
   char name2[50];
   int selected;

   /* Initialize the terminal with ncurses and cdk */
   cdkscreen = initCDKScreen (NULL);
   curs_set(0); // hide cursor
   initCDKColor (); /* Set up CDK Colors. */
   init_color(FOREGROUND, 1000, 550, 0);
   init_pair(200, COLOR_BLACK, BACKGROUND);
   assume_default_colors(FOREGROUND, BACKGROUND);
   subWindow = newwin(HEIGHT, WIDTH, (LINES-HEIGHT)/2, (COLS-WIDTH)/2);
   refresh();
   cdkSubScreen = initCDKScreen(subWindow);
   
   /* Title to be placed on top */
   header = newCDKLabel (cdkSubScreen, CENTER, TOP, (CDK_CSTRING2)list, 1, TRUE, FALSE);
   if (header != 0)
	   activateCDKLabel (header, 0);

   /* Main menu */
   scroll = newCDKScroll (cdkSubScreen,CENTER,CENTER,RIGHT,LIST_SIZE+3,50,HELP, (CDK_CSTRING2) items, LIST_SIZE, false, A_REVERSE, TRUE, TRUE);
   if (scroll == 0)
   {
      destroyCDKScreen (cdkSubScreen);
      endCDK ();

      printf ("Cannot create the scroll list.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }
   setCDKScrollCurrent(scroll, 1);
   setCDKScrollPostProcess(scroll, postProcessScroll, 0);

   /* Main menu treatment */
   while(i == 0){
      drawCDKScreen(cdkSubScreen);
      selected = activateCDKScroll (scroll, 0);
      if (scroll->exitType == vESCAPE_HIT) {
         i = 1;
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
         displayMarquee(cdkscreen, local_loading_text);
         SDL_Quarto ();
         break;
      case ONLINE:
         askForPlayer(cdkscreen, "Enter your name", name1);
         displayMarquee(cdkscreen, online_loading_text);
         break;
      case RULES:
         displaySlide(cdkscreen, RULES, 13, "<C></B/U/D>Rules (Short, I promise)<!D>");
         break;
      case ABOUT:
         displaySlide(cdkscreen, ABOUT, 29, "<C></B/U/D>About QuartoRpi<!D>");
         break;
      case EXIT:
         i = 1;
         break;
      }
   }

   /* Clean up. */
   destroyCDKScreenObjects(cdkscreen);
   destroyCDKScreen (cdkscreen);
   endCDK ();
   #ifndef BUILD_PC
      system("shutdown -h now");
   #endif
   exit (EXIT_SUCCESS);
}

/* Play a quarto game */
int SDL_Quarto (){

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
}
