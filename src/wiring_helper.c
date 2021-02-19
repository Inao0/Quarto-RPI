#ifndef BUILD_PC
#include <stdio.h>
#include <unistd.h>
#include "wiringPi.h"
#include "wiring_helper.h"

/* 
 * ---------------------------------
 * A few helpful wiring Pi functions 
 * ---------------------------------
 */

const int rows[] = {2,3,21,22};
const int columns[] = {6,25,24,23};
const int buttonIDs[4][4] = {{4,3,2,1},{8,7,6,5},{12,11,10,9},{16,15,14,13}};

/* 
 * Read from the SBC-matrix. 
 * Place the coordinates of the first pressed button in x and y.
 */
void getButton(int *x, int *y){
    int i,j;
    int button_pressed = 0;
    int buttons_not_released = 1;
    wiringPiSetup();

    /* Initialize the pins Mode */
    for (i = 0 ; i < 4 ; ++i){
        pinMode      (rows[i], INPUT) ;
        pullUpDnControl(rows[i], PUD_UP);
        pinMode(columns[i], OUTPUT);
        digitalWrite(columns[i], HIGH);
    }
    
    while (!button_pressed){
        for (j = 0 ; j < 4 ; ++j){
            digitalWrite(columns[j], LOW);
            for (i = 0; i < 4; i++) {
                if (digitalRead(rows[i]) == 0) {
                    *x = i;
                    *y = j;
                    while (buttonHeldDown(i)) continue;

                    /* place the pins in the input mode to support .dts layout */
                    for (i = 0 ; i < 4 ; ++i) {
                        pinMode(columns[i], INPUT);
                    }
                    return;
                }
            }
            digitalWrite(columns[j], HIGH);
        }
    }
}

/* 
 * Read continuously from the SBC-matrix. 
 * (Does not work with the breadboard.dts layout)
 */
void displayButtons(){
    int i,j,col_scan;

    wiringPiSetup();

    for (i = 0 ; i < 4 ; ++i){
        pinMode      (rows[i], INPUT) ;
        pullUpDnControl(rows[i], PUD_UP);
        pinMode(columns[i], OUTPUT);
        digitalWrite(columns[i], HIGH);
    }
    
    while (TRUE){
        for (j = 0 ; j < 4 ; ++j){
            digitalWrite(columns[j], LOW);
            for (i = 0; i < 4; i++) {
                if (digitalRead(rows[i]) == 0) {
                    activateButton(i,j);
                    while (buttonHeldDown(i)) {
                        continue;
                    }
                }
            }
            digitalWrite(columns[j], HIGH);
        }
    }
}

/*
 * Check if the pressed button is held down
 */
int buttonHeldDown(int i) {
    if (digitalRead(rows[i]) == 0) {
        return TRUE;
    }
    return FALSE;
}

/*
 * Displays the number of the pressed button
 */
void activateButton(int rowPin, int colPin) {
    int btnIndex = buttonIDs[rowPin][colPin] - 1;
    printf("Button %d pressed\n", btnIndex);
    usleep(300000);
}

#endif