#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <cdk/cdkscreen.h>
#include <cdk/label.h>
#include <cdk/scroll.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "cdk_helper.h"

 #define HELP "<C></U></D></B>Select your choice<!B>"
 #define TITLE "<C>QUARTO - RPi<!C>"
 #define LIST_SIZE 11
 #define HEIGHT (LIST_SIZE+10)
 #define WIDTH 50
 #define FOREGROUND 86
 #define BACKGROUND 15
 #define DEFAULT_PAIR 87
 #define LOCAL 1
 #define ONLINE 3
 #define RULES 5
 #define ABOUT 7
 #define EXIT 9

int main () {
   CDKSCREEN *cdkscreen, *cdkSubScreen;
   CDKSCROLL *scroll = 0;
   CDKLABEL *header;
   WINDOW* subWindow;

   const char *items[] = {""," <#DI> New local game ", "", " <#DI> New online game ", "", " <#DI> Rules ", "", " <#DI> About ", "", " <#DI> Exit ", ""};
   char* about_text[] = {
      "<I=1>This is a student project for the UI lesson at Centrale Lille.","So it may be full of </B>bugs<!B> but don't let that impress you", "",
      "<I=1>The project was inspired by the original Quarto.","Go check it out ! It's even more fun with the board game.",
      "<I=1>Quarto is a board game for two players invented by Swiss mathematician","Blaise Muller. It is published and copyrighted by Gigamic.",
      "It won the following awards: ",
      "<#DI> De d'Or des Createurs de Jeux 1989 - Paris, France",
      "<#DI> Oscar du Jouet - Toy Oscar 1992 - Paris, France",
      "<#DI> Jouet de l'annee - Game of the Year 1992 - Brussels",
      "<#DI> Super As d'Or Festival International des Jeux - Super Golden Ace 1992 - Cannes, France",
      "<#DI> Toy Award 1992 - Benelux",
      "<#DI> Spiel des Jahres - Game of the Year 1993 nominee - Germany",
      "<#DI> Gioco Dell'anno - Game of the Year 1993 - Italy",
      "<#DI> Speelgoed Vant Jaar - Game of the year 1993 - Netherlands",
      "<#DI> Mensa Select Top 5 Best Games 1993 - US",
      "<#DI> Parent's Choice Gold Award 1993 - US",
      "<#DI> Best Bet of the Toy Testing Council 1994 - Canada",
      "<#DI> Prix d'Excellence des Consommateurs - Consumer's Toy Award 1994 - Quebec, Canada",
      "<#DI> Games Magazine 'Games 100 Selection' 1995 - US",
      "<#DI> Games Magazine 'Games 100 Selection' 1996 - US",
      "<#DI> Games Magazine 'Games 100 Selection' 1997 - US",
      "<#DI> Game of the Year 2004 - Finland",
      "<#DI> Parent's Choice Top 25 games in 25 years 2004 - US",
      "",
      "<R>Feel free to use and if you have any ideas,  ", "<R>don't hesitate to contact us   ",
      "<R>Alban, Iona  ",
      "PS: Press Esc to exit ;)"
   };
   char* rules_text[] = {
      "",
      "<I=1>The game is played on a 4x4 board.",
      "",
      "<I=1>There are 16 unique pieces to play with, each of which is either:",
      "<#DI> tall or short;",
      "<#DI> light- or dark-stained wood;",
      "<#DI> square or circular; and",
      "<#DI> hollow-top or solid-top.",
      "<I=1>Players take turns choosing a piece which the other player","must then place on the board.", 
      "<I=1>A player wins by placing a piece on the board which forms"," a horizontal, vertical, or diagonal row of four pieces",
      "all of which have a common attribute (all short, all circular, etc.)."
   };
   const char * local_loading_text = "Loading...";
   const char * online_loading_text = "Waiting for a second RPi";
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
         displaySlide(cdkscreen, rules_text, 13, "<C></B/U/D>Rules (Short, I promise)<!D>");
         break;
      case ABOUT:
         displaySlide(cdkscreen, about_text, 29, "<C></B/U/D>About QuartoRpi<!D>");
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
   exit (EXIT_SUCCESS);
}
