#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <unistd.h>
#ifndef LIST_SIZE
    #define LIST_SIZE 11
#endif

int postProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input){
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

void displaySlide(CDKSCREEN* cdkScreen, char** msg, int length, const char * title){
    const char * button[] = {"Quit"};
    CDKSWINDOW* swindow = newCDKSwindow(cdkScreen, CENTER, CENTER, 25, 85, title, 2, TRUE, FALSE);
    //CDKDIALOG* dialog = newCDKDialog(cdkScreen, CENTER, CENTER, (CDK_CSTRING2)msg, length, (CDK_CSTRING2)button, 1, COLOR_PAIR(2) | A_REVERSE, FALSE, TRUE, TRUE);
    /* Is the selection list null? */
    if (swindow == 0) {
       /* Exit CDK. */
       destroyCDKScreen (cdkScreen);
       endCDK ();

       printf ("Cannot create the selection list.\n");
       printf ("Is the window too small?\n");
       exit (EXIT_FAILURE);
    }
    setCDKSwindowContents(swindow, msg, length);
    activateCDKSwindow(swindow, 0);
    destroyCDKSwindow (swindow);
}