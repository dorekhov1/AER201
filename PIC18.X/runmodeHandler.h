/* 
 * File:   runmodeHandler.h
 * Author: Daniil Orekhov
 *
 * Created on February 2, 2018, 4:45 PM
 */

#ifndef RUNMODEHANDLER_H
#define	RUNMODEHANDLER_H

#include <xc.h>
#include "tapeHandler.h"

void beginTopCounters(void) {
    //set counters to ON
    setArduinoToRunTopCounter('R', 'N');
    setArduinoToRunTopCounter('F', 'N');
    setArduinoToRunTopCounter('L', 'N');
}

void readTapes(int* tapeValues) {
    
    //NEED DELAYS EVERYWHERE, EXPERIMENTATION!!!!

    setArduinoToRunMovingCabinet('1');
    readTapeColumn(1, tapeValues);
    setArduinoToRunMovingCabinet('2');
    readTapeColumn(2, tapeValues);
    setArduinoToRunMovingCabinet('3');
    readTapeColumn(3, tapeValues);
    setArduinoToRunMovingCabinet('4');
    readTapeColumn(4, tapeValues);
}

void openAllDrawers(void) {
    
}

void determineFoodCount(char * diet, int dietNum) {
    
}

void countFood(void) {
    
}

void closeDrawer(int row) {
    
}

void moveToColumn (int column) {
    
}

#endif	/* RUNMODEHANDLE
R_H */

