#ifndef WIRING_HELPER_H
#define WIRING_HELPER_H
#ifndef BUILD_PC

/* 
 * Read from the SBC-matrix. 
 * Place the coordinates of the first pressed button in x and y.
 */
void getButton(int *i, int *j);

/* 
 * Read continuously from the SBC-matrix. 
 * (Does not work with the breadboard.dts layout)
 */
void displayButtons();

/*
 * Check if the pressed button is held down
 */
int buttonHeldDown(int i);

/*
 * Displays the number of the pressed button
 */
void activateButton(int rowPin, int colPin);
#endif
#endif