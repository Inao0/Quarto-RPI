#ifndef CDK_HELPER_H
#define CDK_HELPER_H

int postProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input);
void displaySlide(CDKSCREEN* cdkScreen, int msg, int length, const char* title);
void displayMarquee(CDKSCREEN* cdkScreen, const char* msg);
int quitMarquee (EObjectType cdktype, void *object, void *clientData, chtype key);
void askForPlayer(CDKSCREEN* cdkscreen, const char *title, char * name);
#endif