#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <cdk/cdk_util.h>
#include <cdk/cdkscreen.h>
#include <cdk/entry.h>
#include <cdk/label.h>
#include <cdk/marquee.h>
#include <cdk/scroll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cdk_helper.h"
#include "indexes_menu.h"

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

int postProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input){
   CDKSCROLL * scroll = (CDKSCROLL*) object;
   int current = getCDKScrollCurrent(scroll);
   if (current%2 == 0) {
      if (current == 0) {
         if (input == KEY_UP) {
            setCDKScrollCurrent(scroll, LIST_SIZE-2);
         } else if (input == KEY_DOWN) {
            setCDKScrollCurrent(scroll, 1);
         }
      } else if (current == LIST_SIZE-1) {
         if (input == KEY_UP) {
            setCDKScrollCurrent(scroll, LIST_SIZE - 2);
         } else if (input == KEY_DOWN) {
            setCDKScrollCurrent(scroll, 1);
         }
      } else {
         switch (input) {
            case KEY_UP:
               setCDKScrollCurrent(scroll, current-1);
               break;
            case KEY_DOWN:
               setCDKScrollCurrent(scroll, current+1);
               break;
            default:
               return 1;
         }
      }
   }
   return 0;
}

void displaySlide(CDKSCREEN* cdkScreen, int msg, int length, const char * title){
   CDKSWINDOW* swindow = newCDKSwindow(cdkScreen, CENTER, CENTER, 25, 85, title, 2, TRUE, FALSE);
   if (swindow == 0) {
      destroyCDKScreen (cdkScreen);
      endCDK ();
      printf ("Cannot create the slide window.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }
   if (msg == RULES) {
     setCDKSwindowContents(swindow, rules_text, length);
   } else if (msg == ABOUT) {
     setCDKSwindowContents(swindow, about_text, length);
   }
   activateCDKSwindow(swindow, 0);
   destroyCDKSwindow (swindow);
}

void displayMarquee(CDKSCREEN* cdkScreen, const char* msg){
   CDKMARQUEE* marquee = newCDKMarquee(cdkScreen, CENTER, CENTER, 25, TRUE, TRUE);
   if (marquee == 0) {
      /* Exit CDK. */
      destroyCDKScreen (cdkScreen);
      endCDK ();
      printf ("Cannot create the marquee.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }
   activateCDKMarquee(marquee, msg, 10, 1, TRUE);
   destroyCDKMarquee(marquee);
}

void askForPlayer(CDKSCREEN* cdkscreen, const char *title, char * name){
   CDKENTRY *entry;
   const char *label    = "</U/5>Name:<!U!5>";
   char *temp;
   
   entry = newCDKEntry(cdkscreen, CENTER, CENTER, title, label, A_NORMAL, '.', vMIXED, 40, 0, 256, TRUE, FALSE);
   if (entry == 0) {
      destroyCDKScreen(cdkscreen);
      endCDK();

      printf("Cannot create the entry field. Is the window too small?\n");
      exit(EXIT_FAILURE);
   }
   temp = activateCDKEntry(entry, 0);
   if (temp != NULL) {
      strcpy(name, temp);
   }
   destroyCDKEntry(entry);
}