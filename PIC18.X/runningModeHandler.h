/* 
 * File:   runmodeHandler.h
 * Author: Daniil Orekhov
 *
 * Created on February 2, 2018, 4:45 PM
 */

#ifndef RUNNINGMODEHANDLER_H
#define	RUNNINGMODEHANDLER_H

#include <xc.h>
#include "lcd.h"
#include "timer.h"

int rCount = 0;
int fCount = 0;
int lCount = 0;

int rTotal = 0;
int fTotal = 0;
int lTotal = 0;

int time = 0;

void startTopCounters(void) {
    setArduinoToRunCounter('T', 1);
}

void stopTopCounters(void) {
    setArduinoToRunCounter('T', 0);
}

void startBottomCounter(char counter) {
    setArduinoToRunCounter(counter, 1);
}

void stopBottomCounter(char counter) {
    setArduinoToRunCounter(counter, 0);
}

void moveToColumn(int column) {
    setArduinoToRunMoving(column);
}

void openAllDrawers(void) {
    setArduinoToRunArm(4);
}

void closeDrawer(int row) {
    setArduinoToRunArm(row);
}

void updateCountAndTime(int r, int f, int l) {
    if (readTimer() - time > 10) {
        time = readTimer();
        printTimeToGLCD();
    }
    if (r != rTotal || f != fTotal || l != lTotal) {
        sendCountToArduino(r, f, l);
        rTotal = r;
        fTotal = f;
        lTotal = l;
    }
}

unsigned short readTape(unsigned char channel){
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){  continue;   } // Poll for acquisition completion
    return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}

int drawerIsTaped(int currentRow) {
    __delay_ms(1000);
    unsigned int reading = readTape(currentRow);
    __lcd_clear();
    printf("Channel:%d", currentRow);
    __lcd_newline();
    printf("Tape:%.3d", reading);
    __delay_ms(1000);
    return reading < 800;
}

void determineFoodCount(char * diet, int dietNum) {
    rCount = 0;
    fCount = 0;
    lCount = 0; 
    
    for (unsigned int i = 0; i<4; i++) {
        if (diet[i] == 'R') rCount+=dietNum;
        else if (diet[i] == 'F') fCount+=dietNum;
        else if (diet[i] == 'L') lCount+=dietNum;
    }
}

void countFood(void) {

    unsigned int rf = 0;
    unsigned int rRun = 0;
    unsigned int ff = 0; 
    unsigned int fRun = 0;
    unsigned int lf = 0; 
    unsigned int lRun = 0;
    
    unsigned int rTime = 0;
    unsigned int fTime = 0;
    unsigned int lTime = 0;
    
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    
    if (lCount != 0) { startBottomCounter('L'); lRun = 1; }
    while (lRun == 1){
        if (readTimer() - lTime > 8 && PORTCbits.RC2 == 0 && lf == 0) lf = 1;
        else if (readTimer() - lTime > 8 && PORTCbits.RC1 == 1 && lf == 1) { lCount--; lf=0; lTime = readTimer(); }

        if (lCount <= 0 && lRun == 1) { stopBottomCounter('L'); lRun = 0; }
    }
    
    if (rCount != 0) { startBottomCounter('R'); rRun = 1; }
    while (rRun == 1) {
        if (readTimer() - rTime > 8 && PORTCbits.RC0 == 0 && rf == 0) rf = 1;
        else if (readTimer() - rTime > 8 && PORTCbits.RC0 == 1 && rf == 1) { rCount--; rf=0; rTime = readTimer();}
        
        if (rCount <= 0 && rRun == 1) { stopBottomCounter('R'); rRun = 0; }
    }
    
    if (fCount != 0) { startBottomCounter('F'); fRun = 1; }
    while (fRun == 1) {
        if (readTimer() - fTime > 8 && PORTCbits.RC1 == 0 && ff == 0) ff = 1;
        else if (readTimer() - fTime > 8 && PORTCbits.RC1 == 1 && ff == 1) { fCount--; ff=0; fTime = readTimer();}
        
        if (fCount <= 0 && fRun == 1) { stopBottomCounter('F'); fRun = 0; }
    }
}

#endif	/* RUNNINGMODEHANDLE
R_H */