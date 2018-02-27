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
#include "inputModeHandler.h"
#include "runningModeHandler.h"
#include "logsModeHandler.h"
#include "I2C.h"
#include "rtc.h"
#include "eep.h"
#include "arduino.h"
#include "UART_PIC.h"

typedef struct {
    int destination;
    enum diet dietType;
    int dietNum; //number of types the diet is repeated
    
    char timeCreated;
    char timeExecuted;
} Operation;

void enterStandbyMode(void);
void enterLogsMode(void);

int discardOperations(int* tapedDrawers, int tapedDrawersNum);
void optimizeOperationOrder(int operationNum);

int columnNeedsFood(int currentColumn, int operationsExecuted);
int rowNeedsFood(int currentRow, int operationsExecuted);
void logCreatedOperations(int operationNum);

#endif	/* MAIN_H */