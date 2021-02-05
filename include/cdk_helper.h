#ifndef CDK_HELPER_H
#define CDK_HELPER_H

int postProcessScroll(__attribute__((unused)) EObjectType cdkType, void *object, __attribute__((unused)) void *clientData,chtype input);
void displaySlide(CDKSCREEN* cdkScreen, char** msg, int length, const char* title);
#endif