#ifndef WIRING_HELPER_H
#define WIRING_HELPER_H
#ifndef BUILD_PC
void displayButtons();
void getButton(int *i, int *j);
int buttonHeldDown(int i);
void activateButton(int rowPin, int colPin);
#endif
#endif