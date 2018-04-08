/* 
 * File:   inputHandler.h
 * Author: Daniil Orekhov
 *
 * Created on January 17, 2018, 4:01 PM
 */

#ifndef INPUTMODEHANDLER_H
#define	INPUTMODEHANDLER_H

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "enums.h"
#include "rtc.h"
#include "timer.h"

/*********************************VARIABLES*********************************/

const char keys[] = "123A456B789C*0#D"; //char array corresponding to the keypad

unsigned int newDietType; //type of diet for the operation being created
unsigned int newDietNum; //number to repeat the diet for the operation being created
unsigned int newDestination; //destination drawer for the operation being created

unsigned int currentInputMode = MODE_NO_INPUT; //current sub-mode of the MODE_INPUT machine mode

unsigned int operationNum = 0; //number of operations created
unsigned int displayedOperationNum = 0; //index of the operation currently displayed in the MODE_SHOW_INPUT
unsigned int operationReady = 0; //boolean to tell the main that the operation is ready to be created
unsigned int operationDelete = 0; //boolean to tell the main that the operation needs to be deleted

unsigned char input[16]; //stores the user input
unsigned char message1[16];
unsigned char message2[16];

unsigned int drawersUsed[8]; //stores the drawers used to prevent user repeating drawers

unsigned int allowInput = 0;

unsigned int orderOptimize = 0;

/***************************GETTERS AND SETTERS***************************/

void setInputMode(unsigned int newMode) { currentInputMode = newMode; }
unsigned int getInputMode(void) { return currentInputMode; }

unsigned int getNewDietType(void) { return newDietType; }
unsigned int getNewDietNum(void) { return newDietNum; }
unsigned int getNewDestination(void) { return newDestination; }

unsigned int getOperationNum(void) { return operationNum; }
void setOperationNum(unsigned int num) { operationNum = num; }

unsigned int getDisplayedOperationNum(void) { return displayedOperationNum; }
void setDisplayedOperationNum(unsigned int num) { displayedOperationNum = num   ; }

unsigned int needToDeleteOperation(void) { return operationDelete; }
void setDeleteOperation(unsigned int n) { operationDelete = n; }

unsigned int isOperationReady(void) { return operationReady; }
void setOperationReady(unsigned int b) { operationReady = b; }


unsigned int needsOptimize(void) { return orderOptimize; }
void setOptimize(unsigned int o) { orderOptimize = 0; }

/***************************FUNCTIONS***************************/

void clear(void) {
    allowInput = 0;
    __lcd_clear();
    __lcd_home();
    memset(input,0,strlen(input));
}

void printToLcd(int hasInput) {
    __lcd_clear();
    printf(message1);
    __lcd_newline();
    if (hasInput) printf(message2, input);
    else printf(message2);
    allowInput = 1;
}

void displayAskForMessage(unsigned int mode, unsigned char * mes1) {
    strncpy(message1, mes1, sizeof(message1) - 1);
    strncpy(message2, "%s", sizeof(message2) - 1);
    printToLcd(1);
    unsigned int time = 0;
    while (currentInputMode == mode) {
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
}

void askForOperationInput(void) {
    clear();
    setArduinoToInput('E');
    __lcd_display_control(1, 0, 0)
    strncpy(message1, "Operations ", sizeof(message1) - 1);
    strncpy(message2, "created: %s ", sizeof(message2) - 1);
    sprintf(input, "%d", operationNum);    
    printToLcd(1);
    
    int time = 0;
    while (currentInputMode == MODE_EMPTY_INPUT) {
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
    __lcd_display_control(1, 1, 0)
}

void askForDestination(void) {    
    clear();
    setArduinoToInput('D');
    displayAskForMessage(MODE_INPUT_DESTINATION, "Target drawer: ");
}

void askForDietType(void) {
    clear();
    setArduinoToInput('F');
    displayAskForMessage(MODE_INPUT_DIET, "Diet type: ");
}

void askForDietNum(void) {
    clear();
    if (newDietType-1 == L) setArduinoToInputNum(3);
    else setArduinoToInputNum(2);
    displayAskForMessage(MODE_INPUT_DIET_NUM, "Repeat diet: ");
}

void showPrompt(void) {
    clear();
    setArduinoToInput('P');

    __lcd_display_control(1, 0, 0)
    strncpy(message1, "Operations ", sizeof(message1) - 1);
    strncpy(message2, "created: %s ", sizeof(message2) - 1);
    sprintf(input, "%d", operationNum);
    printToLcd(1);
    
    unsigned int time = 0;
    while (currentInputMode == MODE_INPUT_PROMPT) {
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
    __lcd_display_control(1, 1, 0);
}

void showInput(int num, int diet, int dietN, int dest) {
    clear();
    setArduinoToInput('S');
    
    unsigned int opNum = num; //this is the weirdest bug of my entire coding career
    
    __lcd_display_control(1, 0, 0);
    printf("Operation %d", num+1);
    __lcd_newline();
    printf("%sx%d into %d", diet, dietN, dest);
    allowInput = 1;
    
    int time = 0;
    while (opNum == displayedOperationNum && currentInputMode == MODE_SHOW_INPUT) {
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
    __lcd_display_control(1, 1, 0);
}

int dietNeedsNum(void) {
    return newDietType-1 == R || newDietType-1 == F || newDietType-1 == RF 
            || newDietType-1 == RL || newDietType-1 == FL || newDietType-1 == L;
}

int isDrawerUsed(int drawerNum) {
    for (unsigned int i=0; i<operationNum; i++) if (drawerNum == drawersUsed[i]) return 1;
    return 0;
}

void displayDrawerUsedError (void) {
    __lcd_display_control(1, 0, 0);
    __lcd_clear();
    __lcd_home();
    printf("The drawer is ");
    __lcd_newline();
    printf("used already. ");
    __delay_ms(1000);
    __lcd_display_control(1, 1, 0);
    memset(input,0,strlen(input));
    printToLcd(1);
}

void displayMaxOperationsError(void) {
    __lcd_clear();
    __lcd_home();
    printf("Operation limit ");
    __lcd_newline();
    printf("has been reached. ");
    __delay_ms(1000);
    printToLcd(1);
}

int isProperInput(void) {
    if (currentInputMode == MODE_INPUT_DIET) 
        return (atoi(input) >= 1 && atoi(input) <= 18);
    else if (currentInputMode == MODE_INPUT_DIET_NUM) {
        if (newDietType-1 == R || newDietType-1 == F || newDietType-1 == RF 
                || newDietType-1 == RL || newDietType-1 == FL)
            return (atoi(input) >= 1 && atoi(input) <= 2);
        else if (newDietType-1 == L) return (atoi(input) >= 1 && atoi(input) <= 3);
        else return atoi(input) == 1;
    }
    else if (currentInputMode == MODE_INPUT_DESTINATION) 
        return (atoi(input) <= 16 && atoi(input) >= 1);
    return 0;
}

int isNumberInput(int keypress) {
    if (keypress == 0 || keypress == 1 || keypress == 2 ||
            keypress == 4 || keypress == 5 || keypress == 6 ||
            keypress == 8 || keypress == 9 || keypress == 10 ||
            keypress == 13) return 1;
    return 0;
}

void displayInputError(void) {
    __lcd_display_control(1, 0, 0);
    __lcd_clear();
    __lcd_home();
    printf("Value must be ");
    __lcd_newline();
    if (currentInputMode == MODE_INPUT_DIET) printf("from 1 to 18.");
    else if (currentInputMode == MODE_INPUT_DIET_NUM) {
        if (newDietType-1 == R || newDietType-1 == F || newDietType-1 == RF ||
                newDietType-1 == RL || newDietType-1 == FL) printf("from 1 to 2 ");
        else if (newDietType-1 == L) printf("from 1 to 3 ");
    }
    else if (currentInputMode == MODE_INPUT_DESTINATION) printf("from 1 to 16.");
    __delay_ms(1000);
    __lcd_display_control(1, 1, 0);
    memset(input,0,strlen(input));
    printToLcd(1);
}

void processNumberInput(char keypress) {
    if (isNumberInput(keypress) && strlen(input) < 2) {
        input[strlen(input)] = keys[keypress];
        printToLcd(1);
    }
    else if (keypress == 15) {
        input[strlen(input)-1] = '\0';
        printToLcd(1);
    }
    else if (keypress == 7) {
        if (getOperationNum() == 0) currentInputMode = MODE_EMPTY_INPUT;
        else currentInputMode = MODE_INPUT_PROMPT;
    }
    else if (keypress == 11) {
        if (isProperInput()) {
            if (currentInputMode == MODE_INPUT_DESTINATION) {
                unsigned int drawerNum = atoi(input);
                if (!isDrawerUsed(drawerNum)) {
                    drawersUsed[operationNum] = drawerNum;
                    newDestination = drawerNum;
                    currentInputMode = MODE_INPUT_DIET;
                }
                else displayDrawerUsedError();
            }
            else if (currentInputMode == MODE_INPUT_DIET) {
                newDietType = atoi(input);
                if (dietNeedsNum()) currentInputMode = MODE_INPUT_DIET_NUM;
                else {
                    newDietNum = 1;
                    operationReady = 1;
                    currentInputMode = MODE_INPUT_PROMPT;
                }
            }
            else if (currentInputMode == MODE_INPUT_DIET_NUM) {
                newDietNum = atoi(input);
                operationReady = 1;
                currentInputMode = MODE_INPUT_PROMPT;
            }
        } else displayInputError();
    }
}

void processInputInterrupt(char keypress) {
    if (allowInput) {
        if (currentInputMode == MODE_EMPTY_INPUT) {
            if (keypress == 7) currentInputMode = MODE_NO_INPUT;
            else if (keypress == 12) currentInputMode = MODE_INPUT_DESTINATION;
        }
        else if (currentInputMode == MODE_INPUT_DESTINATION 
                || currentInputMode == MODE_INPUT_DIET 
                || currentInputMode == MODE_INPUT_DIET_NUM) {
            processNumberInput(keypress);
        }
        else if (currentInputMode == MODE_INPUT_PROMPT) {
            if (keypress == 12) {
                if (operationNum < 8) currentInputMode = MODE_INPUT_DESTINATION;
                else displayMaxOperationsError();
            }
            else if (keypress == 14) currentInputMode = MODE_SHOW_INPUT;
            else if (keypress == 3) currentInputMode = MODE_INPUT_COMPLETE;
            else if (keypress == 0) orderOptimize = 1;
        }
        else if (currentInputMode == MODE_SHOW_INPUT) {
            if (keypress == 11) 
                if (operationNum - 1 == displayedOperationNum) {
                    displayedOperationNum = 0;
                    currentInputMode = MODE_INPUT_PROMPT;
                }
                else displayedOperationNum++;
            else if (keypress == 7) {
                displayedOperationNum = 0;
                currentInputMode = MODE_INPUT_PROMPT;
            }
            else if (keypress == 15) {
                operationDelete = 1;
                if (operationNum != 1) currentInputMode = MODE_INPUT_PROMPT;
                else currentInputMode = MODE_EMPTY_INPUT;            
                for (unsigned int i = displayedOperationNum; i<operationNum; i++)
                    drawersUsed[i] = drawersUsed[i+1];
//                displayedOperationNum = 0;
            }
        }
    }
}
#endif	/* INPUTMODEHANDLER_H */