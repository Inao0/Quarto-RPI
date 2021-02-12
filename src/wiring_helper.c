#include <stdio.h>
#include <unistd.h>
#ifndef BUILD_PC

#include "../lib/pi/wiringPi/wiringPi/wiringPi.h"
#include "../include/wiring_helper.h"

const int rows[] = {2,3,21,22};
const int columns[] = {23,24,25,6};
const int ButtonIDs[4][4] = {{4,3,2,1},{8,7,6,5},{12,11,10,9},{16,15,14,13}};

int getButton(){
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
                    return ButtonIDs[i][j] - 1;
                }
            }
            digitalWrite(columns[j], HIGH);
        }
    }
    return -1;
}

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

int buttonHeldDown(int i) {
    if (digitalRead(rows[i]) == 0) {
        return TRUE;
    }
    return FALSE;
}

void activateButton(int rowPin, int colPin) {
    int btnIndex = ButtonIDs[rowPin][colPin] - 1;
    printf("Button %d pressed\n", btnIndex);
    usleep(300000);
}

#endif