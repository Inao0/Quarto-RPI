#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <cdk/cdkscreen.h>
#include <cdk/label.h>
#include <cdk/scroll.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "cdk_helper.h"
#include "wiring_helper.h"
#include "indexes_menu.h"

#define HELP "<C></U></D></B>Select your choice<!B>"
#define TITLE "<C>QUARTO - RPi<!C>"
#define HEIGHT (LIST_SIZE+10)
#define WIDTH 50
#define FOREGROUND 86
#define BACKGROUND 15
#define DEFAULT_PAIR 87

int main () {
   CDKSCREEN *cdkscreen, *cdkSubScreen;
   CDKSCROLL *scroll = 0;
   CDKLABEL *header;
   WINDOW* subWindow;

   const char *items[] = {""," <#DI> New local game ", "", " <#DI> New online game ", "", " <#DI> Rules ", "", " <#DI> About ", "", " <#DI> Exit ", ""};
   const char * local_loading_text = "Loading...";
   const char * online_loading_text = "Waiting for a second RPi - Not implemented";
   const char *list[1];
   list[0] = TITLE;
   
   int selected;

   cdkscreen = initCDKScreen (NULL);
   curs_set(0); // hide cursor
   /* Set up CDK Colors. */
   initCDKColor ();
   init_color(FOREGROUND, 1000, 550, 0);
   init_pair(200, COLOR_BLACK, BACKGROUND);
   assume_default_colors(FOREGROUND, BACKGROUND);
   subWindow = newwin(HEIGHT, WIDTH, (LINES-HEIGHT)/2, (COLS-WIDTH)/2);
   refresh();

   cdkSubScreen = initCDKScreen(subWindow);
   
   header = newCDKLabel (cdkSubScreen, CENTER, TOP, (CDK_CSTRING2)list, 1, TRUE, FALSE);
   if (header != 0)
	   activateCDKLabel (header, 0);

   /* Create the scroll list. */
   scroll = newCDKScroll (cdkSubScreen,CENTER,CENTER,RIGHT,LIST_SIZE+3,50,HELP, (CDK_CSTRING2) items,LIST_SIZE,
      false, A_REVERSE, TRUE, TRUE);

   if (scroll == 0)
   {
      /* Exit CDK. */
      destroyCDKScreen (cdkSubScreen);
      endCDK ();

      printf ("Cannot create the scroll list.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }
   setCDKScrollCurrent(scroll, 1);
   setCDKScrollPostProcess(scroll, postProcessScroll, 0);
   int i = 0;
   char name1[50];
   char name2[50];
   while(i == 0){
      drawCDKScreen(cdkSubScreen);
      selected = activateCDKScroll (scroll, 0);
      eraseCDKScreen(cdkSubScreen);
      switch (selected) {
      case LOCAL:
         askForPlayer(cdkscreen, "Enter Player 1 name", name1);
         askForPlayer(cdkscreen, "Enter Player 2 name", name2);
         displayMarquee(cdkscreen, local_loading_text);
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
      int x,y;
      getButton(&x,&y);
      printf("X: %d, Y: %d\n", x, y);
   #endif
   exit (EXIT_SUCCESS);
}
