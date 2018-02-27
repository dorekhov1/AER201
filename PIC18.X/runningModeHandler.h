/* 
 * File:   runmodeHandler.h
 * Author: Daniil Orekhov
 *
 * Created on February 2, 2018, 4:45 PM
 */

#ifndef RUNNINGMODEHANDLER_H
#define	RUNNINGMODEHANDLER_H

#include <xc.h>

int rCount = 0;
int fCount = 0;
int lCount = 0;

void startTopCounters(void) {
    setArduinoToRunCounter('T', 'N');
}

void stopTopCounters(void) {
    setArduinoToRunCounter('T', 'F');
}

void startBottomCounter(char counter) {
    setArduinoToRunCounter(counter, 'N');
}

void stopBottomCounter(char counter) {
    setArduinoToRunCounter(counter, 'F');
}

void moveToColumn(int column) {
    //stepper code
}

unsigned short readTape(unsigned char channel){
    ADCON0 = (channel & 0x0F) << 2; // Select ADC channel (i.e. pin)
    ADON = 1; // Enable module
    ADCON0bits.GO = 1; // Initiate sampling
    while(ADCON0bits.GO_NOT_DONE){  continue;   } // Poll for acquisition completion
    return (ADRESH << 8) | ADRESL; // Return result as a 16-bit value
}

void readTapeColumn(int column, int* tapeValues) {
    tapeValues[column] = readTape(0); // topmost sensor
    tapeValues[column + 4] = readTape(0); // second topmost sensor
    tapeValues[column + 8] = readTape(0); // second bottommost sensor
    tapeValues[column + 12] = readTape(0); // bottommost sensor
}

int processTapes(int* tapeValues, int* tapedDrawers) {
    int tolerance =  10; // need experimentation for these two
    int baseline = 1000;
    int tapedDrawersNum = 0;
    
    for (int i = 0; i < 16; i++)
        if (tapeValues[i] < baseline) baseline = tapeValues[i];
    
    for (int i = 0; i < 16; i++) {
        if ((tapeValues[i] - baseline) > tolerance) tapedDrawers[++tapedDrawersNum] = i+1;
    }
    
    return tapedDrawersNum;    
}

void readTapes(int* tapeValues) {
    
    //NEED DELAYS EVERYWHERE, EXPERIMENTATION!!!
    
    /*moveToColumn(1);
    readTapeColumn(1, tapeValues);
    moveToColumn(2);
    readTapeColumn(2, tapeValues);
    moveToColumn(3);
    readTapeColumn(3, tapeValues);
    moveToColumn(4);
    readTapeColumn(4, tapeValues);*/
    
    for (int i=0; i<16; i++) {
        tapeValues[i] = 40;
    }
}

void openAllDrawers(void) {
    setArduinoToRunArm('0');
}

void closeDrawer(int row) {
    setArduinoToRunArm(row+'0');
}

void determineFoodCount(char * diet, int dietNum) {
    
    rCount = 0;
    fCount = 0;
    lCount = 0; 
    
    for (unsigned int i = 0; i<sizeof(diet); i++) {
        if (diet[i] == 'R') rCount+=dietNum;
        else if (diet[i] == 'F') fCount+=dietNum;
        else if (diet[i] == 'L') lCount+=dietNum;
    }
}

void countFood(void) {
    unsigned int rPin = 0, rf = 0, rRun = 0;
    unsigned int fPin = 0, ff = 0, fRun = 0;
    unsigned int lPin = 0, lf = 0, lRun = 0;
    
    if (rCount != 0) { startBottomCounter('R'); rRun = 1; }
    else if (fCount != 0) { startBottomCounter('F'); fRun = 1; }
    else if (lCount != 0) { startBottomCounter('L'); lRun = 1; }
    
    while (rCount!=0 && fCount!=0 && lCount!=0){
        if (rPin == 1 && rf == 0) rf = 1;
        else if (rPin == 0 && rf == 1) { rCount--; rf=0; }
        
        if (fPin == 1 && ff == 0) ff = 1;
        else if (fPin == 0 && ff == 1) { fCount--; ff=0; }
        
        if (lPin == 1 && lf == 0) lf = 1;
        else if (lPin == 0 && lf == 1) { lCount--; lf=0; }
        
        if (rCount == 0 && rRun == 1) { stopBottomCounter('R'); rRun = 0; }
        if (fCount == 0 && fRun == 1) { stopBottomCounter('F'); fRun = 0; }
        if (lCount == 0 && lRun == 1) { stopBottomCounter('L'); lRun = 0; }
    }
}

#endif	/* RUNNINGMODEHANDLE
R_H */