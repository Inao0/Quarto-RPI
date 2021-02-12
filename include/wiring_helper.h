#ifndef WIRING_HELPER_H
#define WIRING_HELPER_H
#ifndef BUILD_PC
void displayButtons();
void printButton(int i, int j);
void scanButton(int i);
int buttonHeldDown(int i);
void activateButton(int rowPin, int colPin);
#endif
#endif