#ifndef CDK_HELPER_H
#define CDK_HELPER_H

/*
 * Callback function called on the scrolling menu
 * It implements an infinite scrolling menu
 */
int postProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input);

/*
 * Display a sliding window with a title and a msg
 * msg is of the form CDK_CSTRING2, and length has its size
 */
void displaySlide(CDKSCREEN* cdkScreen, int msg, int length, const char* title);

/*
 * Displays an animation with a message
 */
void displayMarquee(CDKSCREEN* cdkScreen, const char* msg, char * name);

/*
 * Ask a player's name using a cdk_matrix.
 * It is placed in the name variable.
 */
int askForPlayer(CDKSCREEN* cdkscreen, const char *title, char * name);

/*
 * Displays a message to congratulate the winner.
 */
void showWinner(CDKSCREEN* screen, int winner, char *name1, char *name2);


/*
 * Displays a message in case of ex aequo.
 */
void showExAequo(CDKSCREEN* screen);
#endif