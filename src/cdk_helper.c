#include <cdk.h>
#include <cdk/cdk_objs.h>
#include <cdk/cdk_util.h>
#include <cdk/cdkscreen.h>
#include <cdk/curdefs.h>
#include <cdk/entry.h>
#include <cdk/label.h>
#include <cdk/marquee.h>
#include <cdk/matrix.h>
#include <cdk/scroll.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cdk_helper.h"
#include "indexes_menu.h"

#define N_LETTERS 3
#define N_COLS 3

/* Texts to be displayed in slide windows */
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

/*
 * -----------------------------------
 * A few helpful CDK helping functions
 * -----------------------------------
 */

/*
 * Callback function called on the scrolling menu
 * It implements an infinite scrolling menu
 */
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

/*
 * Display a sliding window with a title and a msg
 * msg is of the form CDK_CSTRING2, and length has its size
 */
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

/*
 * Displays an animation with a message
 */
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

/*
 * Ask a player's name using a cdk_matrix.
 * It is placed in the name variable.
 */
int askForPlayer(CDKSCREEN* cdkscreen, const char *title, char * name){
   CDKMATRIX *charMatrix = 0;
   int rows             = 1;
   int cols             = N_LETTERS;
   int vrows            = 1;
   int vcols            = N_LETTERS;

   const char *coltitle[N_COLS];
   const char *rowtitle[N_COLS];

   int colwidth[N_COLS] = {1};
   int colvalue[N_COLS] = {vUMIXED};

   chtype input;
   boolean functionKey = FALSE;
   char *currentValue;
   char newValue;
   int status;
   int col_spacing = -1;
   int row_spacing = -1;
   /* the above initialization was not enough ¯\_(ツ)_/¯*/
   for (int i = 0; i <= N_LETTERS; i++) {
      colwidth[i] = 1 ;
      rowtitle[i] = "";
      coltitle[i] = "";
   }
   rowtitle[1] = "Pseudo:";

   /* Create the matrix object. */
   charMatrix = newCDKMatrix (cdkscreen,CENTER,CENTER,
			      rows, cols, vrows, vcols,
			      title,
			      (CDK_CSTRING2)rowtitle,
			      (CDK_CSTRING2)coltitle,
			      colwidth, colvalue,
			      col_spacing, row_spacing, '.',
			      COL, TRUE,
			      FALSE,
			      TRUE);

   /* Fill the matrix with A */
   for (int i = 0; i < N_LETTERS + 1; i++) {
      setCDKMatrixCell(charMatrix, 1, i, "A");
   }
   
   drawCDKMatrix(charMatrix,TRUE);
   // a bit of a hack to quit widget at the right-most cell
   // Normally, we would call activateCDKMatrix() but this was a blocking call
   // and we needed to intercept the keys (a callback could not end the activateCDKMatrix() call)
   while (charMatrix->exitType != vNORMAL) {
      input = (chtype)getchCDKObject (ObjOf (charMatrix), &functionKey);
      currentValue = getCDKMatrixCell(charMatrix, charMatrix->crow, charMatrix->ccol);
      switch (input) {
         case KEY_UP:
            if (currentValue[0] == 'Z')
               newValue = 'A';
            else
               newValue = (char)(((int)currentValue[0]) + 1);
            setCDKMatrixCell(charMatrix, charMatrix->crow, charMatrix->ccol, &newValue);
            break;
         case KEY_DOWN:
            if (currentValue[0] == 'A')
               newValue = 'Z';
            else
               newValue = (char)(((int)currentValue[0]) - 1);
            setCDKMatrixCell(charMatrix, charMatrix->crow, charMatrix->ccol, &newValue);
            break;
         case KEY_ENTER:
            if (charMatrix->ccol == N_LETTERS){
               input = KEY_ENTER;
            } else {
               input = KEY_RIGHT;
            }
            break;
         case KEY_ESC:
            destroyCDKMatrix(charMatrix);
            return EXIT_FAILURE;
      }
      drawCDKMatrix(charMatrix, TRUE);
      injectCDKMatrix (charMatrix, input);
   };

   /* If the user pressed KEY_ENTER */
   if (charMatrix->exitType == vNORMAL) {
      for (int i = 1; i <= N_LETTERS; i++) {
         currentValue = getCDKMatrixCell(charMatrix, 1, i);
         name[i-1] = currentValue[0];
      }
      name[N_LETTERS] = '\0';
      status = EXIT_SUCCESS;
   } else {
      status = EXIT_FAILURE;
   }

   /* Clean up. */
   destroyCDKMatrix (charMatrix);
   return status;
}

/*
 * Displays a message to congratulate the winner.
 */
void showWinner(CDKSCREEN* screen, int winner, char *name1, char *name2) {
   const char *mesg[80];
   char temp1[80];
   char temp2[80];

   sprintf (temp1, "<C>  <#VL>Player </K/11>%s<!K><!11> won brilliantly !<#VL>  ", (winner == 1) ? name1 : name2);
   sprintf (temp2, "Player %s did good too.", (winner == 1) ? name2 : name1);
   
   mesg[0] = "End of the game...";
   mesg[1] = "";
   mesg[2] = "<C><#UL><#HL(28)><#UR>";
   mesg[3] = temp1;
   mesg[4] = "<C><#LL><#HL(28)><#LR>";
   mesg[5] = "";
   mesg[6] = temp2;
   mesg[7] = "<C><#HL(10)>";
   mesg[8] = "<C>Press any key to continue.";
   popupLabel (screen, (CDK_CSTRING2)mesg, 9);
}

/*
 * Displays a message in case of ex aequo.
 */
void showExAequo(CDKSCREEN* screen) {
   const char *mesg[80];
   mesg[0] = "End of the game...";
   mesg[1] = "";
   mesg[2] = "<C>None of you came on top.";
   mesg[3] = "<C>But what a game it was !";
   mesg[4] = "";
   mesg[5] = "<C>Press any key to continue.";
   popupLabel (screen, (CDK_CSTRING2)mesg, 6);
}