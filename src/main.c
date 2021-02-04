
#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <stdio.h>
#include <stdlib.h>

 #define HELP "<C></U></D></B>Select your choice<!B>"
 #define TITLE "<C>QUARTO - RPi<!C>"
 #define LIST_SIZE 11
 #define HEIGHT (LIST_SIZE+10)
 #define WIDTH 50
 #define FOREGROUND 254
 #define BACKGROUND 15
 #define DEFAULT_PAIR 200
 
static int preProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input);

int main () {
   CDKSCREEN *cdkscreen, *cdkSubScreen;
   CDKSCROLL *scroll = 0;
   CDKLABEL *header;
   WINDOW* subWindow;

   static const char *items[] = {""," <#DI> New local game ", "", " <#DI> New online game ", "", " <#DI> Rules ", "", " <#DI> About ", "", " <#DI> Exit ", ""};
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
   setCDKScrollPostProcess(scroll, preProcessScroll, 0);
   /* Activate the scroll list. */
   while((selected = activateCDKScroll (scroll, 0) == -1));

   /* Clean up. */
   destroyCDKScreenObjects(cdkscreen);
   destroyCDKScreen (cdkscreen);
   endCDK ();
   exit (EXIT_SUCCESS);
}

static int preProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input){
   CDKSCROLL * scroll = (CDKSCROLL*) object;
   int current = getCDKScrollCurrent(scroll);
   if (current%2 == 0) {
      if (current == 0) {
         setCDKScrollCurrent(scroll, 1);
      } else if (current == LIST_SIZE-1) {
         setCDKScrollCurrent(scroll, LIST_SIZE-2);
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