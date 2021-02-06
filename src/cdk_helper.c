#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <cdk/cdk_util.h>
#include <cdk/cdkscreen.h>
#include <cdk/entry.h>
#include <cdk/label.h>
#include <cdk/marquee.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cdk_helper.h"
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
    CDKSWINDOW* swindow = newCDKSwindow(cdkScreen, CENTER, CENTER, 25, 85, title, 2, TRUE, FALSE);
    if (swindow == 0) {
       destroyCDKScreen (cdkScreen);
       endCDK ();

       printf ("Cannot create the slide window.\n");
       printf ("Is the window too small?\n");
       exit (EXIT_FAILURE);
    }
    setCDKSwindowContents(swindow, msg, length);
    activateCDKSwindow(swindow, 0);
    destroyCDKSwindow (swindow);
}

int quitMarquee (__attribute__((unused)) EObjectType cdktype,__attribute__((unused)) void *object, void *clientData, __attribute__((unused)) chtype key) {
   CDKMARQUEE *marquee = (CDKMARQUEE *)clientData;
   destroyCDKMarquee(marquee);
   //endCDK();
   //exit(0);
   return 0;
}

void displayMarquee(CDKSCREEN* cdkScreen, const char* msg){
   CDKMARQUEE* marquee = newCDKMarquee(cdkScreen, CENTER, CENTER, 25, TRUE, TRUE);
   //const char * liste[1];
   //liste[0] = "0";
   //CDKLABEL* header = newCDKLabel (cdkScreen, RIGHT, TOP, (CDK_CSTRING2)liste, 1, TRUE, FALSE);
   //if (header == 0) {
   //   destroyCDKScreen (cdkScreen);
   //   endCDK ();
   //   printf ("Cannot create the marquee.\n");
   //   printf ("Is the window too small?\n");
   //   exit (EXIT_FAILURE);
   //}
   //refreshCDKScreen(cdkScreen);
	//activateCDKLabel (header, 0);
   if (marquee == 0) {
      /* Exit CDK. */
      destroyCDKScreen (cdkScreen);
      endCDK ();
      printf ("Cannot create the marquee.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }
   //bindCDKObject (vLABEL, header, KEY_EXIT, quitMarquee, marquee);
   activateCDKMarquee(marquee, msg, 10, 1, TRUE);
   destroyCDKMarquee(marquee);
   //destroyCDKLabel(header);
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