/* 
 * File:   main.h
 * Author: Daniil Orekhov
 *
 * Created on January 16, 2018, 3:37 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "configBits.h"
#include "enums.h"
#include "inputHandler.h"
#include "runmodeHandler.h"
#include "tapeHandler.h"
#include "I2C.h"
#include "rtc.h"
#include "eep.h"
#include "arduino.h"

typedef struct {
    
    int destination;
    enum diet dietType;
    int dietNum; //number of types the diet is repeated
    
} Operation;

void enterStandby(void);
int discardOperations(int* tapedDrawers, int tapedDrawersNum);
void optimizeOperationOrder(int operationNum);

int columnNeedsFood(int currentColumn, int operationsExecuted);
int rowNeedsFood(int currentRow, int operationsExecuted);

#endif	/* MAIN_H */