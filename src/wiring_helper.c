#include <stdio.h>
#include <unistd.h>
#ifndef BUILD_PC

#include "../lib/rpi/wiringPi/wiringPi/wiringPi.h"
#include "../include/wiring_helper.h"

const int rows[] = {2,3,21,22};
const int columns[] = {6,25,24,23};
const int buttonIDs[4][4] = {{4,3,2,1},{8,7,6,5},{12,11,10,9},{16,15,14,13}};

void getButton(int *x, int *y){
    int i,j;

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
                    *x = i;
                    *y = j;
                    return;
                }
            }
            digitalWrite(columns[j], HIGH);
        }
    }
    return;
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
    int btnIndex = buttonIDs[rowPin][colPin] - 1;
    printf("Button %d pressed\n", btnIndex);
    usleep(300000);
}

#endif