/* 
 * File:   inputHandler.h
 * Author: Daniil Orekhov
 *
 * Created on January 17, 2018, 4:01 PM
 */

#ifndef INPUTHANDLER_H
#define	INPUTHANDLER_H

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "configBits.h"
#include "lcd.h"
#include "enums.h"
#include "rtc.h"

/*********************************VARIABLES*********************************/

const char keys[] = "123A456B789C*0#D"; //char array corresponding to the keypad

int newDietType; //type of diet for the operation being created
int newDietNum; //number to repeat the diet for the operation being created
int newDestination; //destination drawer for the operation being created

int currentInputMode = MODE_NO_INPUT; //current sub-mode of the MODE_INPUT machine mode

int operationNum = 0; //number of operations created
int displayedOperationNum = 0; //index of the operation currently displayed in the MODE_SHOW_INPUT
int operationReady = 0; //boolean to tell the main that the operation is ready to be created
int operationDelete = 0; //boolean to tell the main that the operation needs to be deleted

char input[16]; //stores the user input
int drawersUsed[8]; //stores the drawers used to prevent user repeating drawers

/***************************GETTERS AND SETTERS***************************/

void setInputMode(int newMode) { currentInputMode = newMode; }
int getInputMode(void) { return currentInputMode; }

int getNewDietType(void) { return newDietType; }
int getNewDietNum(void) { return newDietNum; }
int getNewDestination(void) { return newDestination; }

int getOperationNum(void) { return operationNum; }
void setOperationNum(int num) { operationNum = num; }

int getDisplayedOperationNum(void) { return displayedOperationNum; }

int needToDeleteOperation(void) { return operationDelete; }
void setDeleteOperation(int n) { operationDelete = n; }

int isOperationReady(void) { return operationReady; }
void setOperationReady(int b) { operationReady = b; }

/***************************FUNCTIONS***************************/

void clear(void) {
    __lcd_clear();
    __lcd_home();
    memset(input,0,strlen(input));
}

void askForOperationInput(void) {
    clear();
    setArduinoToInput('E');
    __lcd_display_control(1, 0, 0)
    printf("Operations ");
    __lcd_newline();
    printf("created: %d", operationNum);
    int i = 10;
    while (currentInputMode == MODE_EMPTY_INPUT) {
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        }
    }
    __lcd_display_control(1, 1, 0)
}

void askForDestination(void) {    
    clear();
    setArduinoToInput('D');
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_DESTINATION) {
        __lcd_clear();
        printf("Target drawer: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        }
    }
}

void askForDietType(void) {
    clear();
    setArduinoToInput('F');

    int i = 10;
    while (currentInputMode == MODE_INPUT_DIET) {
        __lcd_clear();
        printf("Diet type: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        } 
    }
}

void askForDietNum(void) {
    clear();
    if (newDietType-1 == L) setArduinoToInputNum(3);
    else setArduinoToInputNum(2);
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_DIET_NUM) {
        __lcd_clear();
        printf("Repeat diet: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        }  
    }
}

void showPrompt(void) {
    clear();
    setArduinoToInput('P');

    __lcd_display_control(1, 0, 0)
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_PROMPT) {
        __lcd_clear();
        printf("Operations ");
        __lcd_newline();
        printf("created: %d", operationNum);
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        }   
    }
    __lcd_display_control(1, 1, 0);
}

void showInput(int num, int diet, int dietN, int dest) {
    clear();
    setArduinoToInput('S');
    
    int opNum = num; //this is the weirdest bug of my entire coding career
    
    __lcd_display_control(1, 0, 0);
    printf("Operation %d", num+1);
    __lcd_newline();
    printf("%sx%d into %d", diet, dietN, dest);
    
    int i = 10;
    while (opNum == displayedOperationNum && currentInputMode == MODE_SHOW_INPUT) {        
        __delay_ms(100);
        if (i++==10) {
            printTimeToGLCD();
            i=0;
        }     
    }
    __lcd_display_control(1, 1, 0);
}

int dietNeedsNum(void) {
    return newDietType-1 == R || newDietType-1 == F || newDietType-1 == RF 
            || newDietType-1 == RL || newDietType-1 == FL || newDietType-1 == L;
}

int isDrawerUsed(int drawerNum) {
    for (int i=0; i<operationNum; i++) {
        //__lcd_clear();
        //printf("debug2: %d" , drawersUsed[i]);
        //__delay_ms(1000);
        if (drawerNum == drawersUsed[i]) return 1;
    }
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
}

void displayMaxOperationsError(void) {
    __lcd_clear();
    __lcd_home();
    printf("Operation limit ");
    __lcd_newline();
    printf("has been reached. ");
    __delay_ms(1000);
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
}

void processNumberInput(char keypress) {
    if (isNumberInput(keypress) && strlen(input) < 2) 
        input[strlen(input)] = keys[keypress];
    else if (keypress == 15) input[strlen(input)-1] = '\0';
    else if (keypress == 7) {
        if (getOperationNum() == 0) currentInputMode = MODE_EMPTY_INPUT;
        else currentInputMode = MODE_INPUT_PROMPT;
    }
    else if (keypress == 11) {
        if (isProperInput()) {
            if (currentInputMode == MODE_INPUT_DESTINATION) {
                int drawerNum = atoi(input);
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
            for (int i = displayedOperationNum; i<operationNum; i++)
                drawersUsed[i] = drawersUsed[i+1];
            displayedOperationNum = 0;
        }
    }
}
#endif	/* INPUTHANDLER_H */