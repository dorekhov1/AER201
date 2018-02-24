/* 
 * File:   tapeHandler.h
 * Author: Daniil Orekhov
 *
 * Created on February 2, 2018, 4:59 PM
 */

#ifndef TAPEHANDLER_H
#define	TAPEHANDLER_H

#include <xc.h>


unsigned short readTape(char channel){
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

#endif	/* TAPEHANDLER_H */

