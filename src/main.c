
#include "../lib/pc/cdk-5.0/include/cdk.h"
#include <stdio.h>

 #define HELP "</B>Select your choice<!B>"
 #define TITLE "QUARTO - RPi"
 #define LIST_SIZE 5
 #define FOREGROUND 254
 #define BACKGROUND COLOR_WHITE
 
int main ()
{
   CDKSCREEN *cdkscreen    = 0;
   CDKSCROLL *scroll = 0;
   int selected;
   static const char *items[] = {"New local game", "New online game", "Rules", "About", "Exit"};

   cdkscreen = initCDKScreen (NULL);

   /* Set up CDK Colors. */
   initCDKColor ();
   init_color(FOREGROUND, 1000, 550, 0);
   assume_default_colors(FOREGROUND, 15);
   refresh();

   const char *list[2];
   CDKLABEL *footer;
   list[0] = TITLE;
   list[1] = 0;
   footer = newCDKLabel (cdkscreen, CENTER, BOTTOM, (CDK_CSTRING2)list, 1, TRUE, FALSE);
   if (footer != 0)
	   activateCDKLabel (footer, 0);

   /* Create the scroll list. */
   scroll = newCDKScroll (cdkscreen,CENTER,CENTER,RIGHT,20,50,HELP, (CDK_CSTRING2) items,LIST_SIZE,
      false, A_REVERSE, TRUE, TRUE);

   if (scroll == 0)
   {
      /* Exit CDK. */
      destroyCDKScreen (cdkscreen);
      endCDK ();

      printf ("Cannot create the scroll list.\n");
      printf ("Is the window too small?\n");
      exit (EXIT_FAILURE);
   }

   /* Activate the scroll list. */
   while((selected = activateCDKScroll (scroll, 0) == -1));

   /* Clean up. */
   destroyCDKScreenObjects(cdkscreen);
   destroyCDKScreen (cdkscreen);
   endCDK ();
   exit (EXIT_SUCCESS);
}
