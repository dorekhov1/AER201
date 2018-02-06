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
#include "GLCD_PIC.h"
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
//int newNumberInput = 0;

char input[16]; //stores the user input
char message[40]; //the message to be displayed on the LCD

/***************************GETTERS AND SETTERS***************************/

void setInputMode(int newMode) { currentInputMode = newMode; }
int getInputMode(void) { return currentInputMode; }

int getNewDietType(void) { return newDietType; }
int getNewDietNum(void) { return newDietNum; }
int getNewDestination(void) { return newDestination; }

int getOperationNum(void) { return operationNum; }
void setOperationNum(int num) { operationNum = num; }

int getDisplayedOperationNum(void) { return displayedOperationNum; }

int isOperationReady(void) { return operationReady; }
void setOperationReady(int b) { operationReady = b; }

/***************************FUNCTIONS***************************/

void clear(void) {
    __lcd_clear();
    __glcd_clear();
    __lcd_home();
    
    memset(input,0,strlen(input));
}

void askForOperationInput(void) {
    clear();
    __lcd_display_control(1, 0, 0)
    printf("Operations ");
    __lcd_newline();
    printf("created: %d", operationNum);
    
    print_px_string(2, 27, "Press * to create a");
    print_px_string(2, 38, "new operation.");
    print_px_string(2, 49, "Press B to go back ");
    print_px_string(2, 60, "to standby.");
    
    int i = 10;
    while (currentInputMode == MODE_EMPTY_INPUT) {
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;
    }
    __lcd_display_control(1, 1, 0)
}

void askForDestination(void) {    
    clear();
    
    print_px_string(2, 27, "Input the ");
    print_px_string(2, 38, "destination drawer.");
    print_px_string(2, 49, "Value must be");
    print_px_string(2, 60, "from 1 to 16.");
    
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 103, 128, BLUE);
    print_px_string(2, 104, "Legend: B-go back,");
    print_px_string(2, 115, "C-continue, D-delete");
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_DESTINATION) {
        __lcd_clear();
        printf("Target drawer: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;
    }
}

void askForDietType(void) {
    clear();
    
    print_px_string(2, 16, "Input diet type: ");
    print_px_string(2, 33, "R-1 F-2 L-3 RF-4 RL-5");
    print_px_string(2, 44, "FL-6 RRF-7 RRL-8");
    print_px_string(2, 55, "RFF-9 RLL-10 RFL-11");
    print_px_string(2, 66, "FFL-12 FLL-13 RRFL-14");
    print_px_string(2, 77, "RFFL-15 RFLL-16");
    print_px_string(2, 88, "RLLL-17 FLLL-18");
    
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 103, 128, BLUE);
    print_px_string(2, 104, "Legend: B-go back,");
    print_px_string(2, 115, "C-continue, D-delete");
    int i = 10;
    while (currentInputMode == MODE_INPUT_DIET) {
        __lcd_clear();
        printf("Diet type: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;    
    }
    
}

void askForDietNum(void) {
    clear();
    
    char s[10];
    if (newDietType-1 == L) sprintf(s, "From 1 to %d",3);
    else sprintf(s, "From 1 to %d",2);
    
    print_px_string(2, 16, "Input the number of");
    print_px_string(2, 27, "times to repeat diet");    
    
    print_px_string(2, 49, "Value must be");
    print_px_string(2, 60, s);
    
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 103, 128, BLUE);
    print_px_string(2, 104, "Legend: B-go back,");
    print_px_string(2, 115, "C-continue, D-delete");
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_DIET_NUM) {
        __lcd_clear();
        printf("Repeat diet: ");
        __lcd_newline();
        printf("%s", input);
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;    
    }
    
}

void showPrompt(void) {
    clear();
    
    __lcd_display_control(1, 0, 0)
    printf("Operations ");
    __lcd_newline();
    printf("created: %d", operationNum);
    
    print_px_string(2, 27, "Press * to create a");
    print_px_string(2, 38, "new operation.");
    print_px_string(2, 49, "Press # to view");
    print_px_string(2, 60, "created operations.");
    
    print_px_string(2, 80, "To go back to standby");
    print_px_string(2, 91, "you must delete all");
    print_px_string(2, 102, "created operations.");
    
    int i = 10;
    while (currentInputMode == MODE_INPUT_PROMPT) {
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;    
    }
    __lcd_display_control(1, 1, 0);
}

void showInput(int num, int diet, int dietN, int dest) {
    clear();
    
    int opNum = num; //this is the weirdest bug of my entire coding career
    
    __lcd_display_control(1, 0, 0);
    printf("Operation %d", num+1);
    __lcd_newline();
    printf("%sx%d into %d", diet, dietN, dest);
    
    print_px_string(2, 27, "Press B to go back.");
    print_px_string(2, 38, "Press C to view next");
    print_px_string(2, 49, "operation.");
    print_px_string(2, 60, "Press D to delete");
    print_px_string(2, 71, "this operation.");
    
    int i = 10;
    while (opNum == displayedOperationNum && currentInputMode == MODE_SHOW_INPUT) {        
        __delay_ms(100);
        if (i==10) {
            printTimeToGLCD();
            i=0;
        }
        i++;
    }
    
    __lcd_display_control(1, 1, 0);
}

int dietNeedsNum(void) {
    return newDietType-1 == R || newDietType-1 == F || newDietType-1 == RF 
            || newDietType-1 == RL || newDietType-1 == FL || newDietType-1 == L;
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
                newDestination = atoi(input);
                currentInputMode = MODE_INPUT_DIET;
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
        if (keypress == 12) currentInputMode = MODE_INPUT_DESTINATION;
        else if (keypress == 14) currentInputMode = MODE_SHOW_INPUT;
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
    }
}

#endif	/* INPUTHANDLER_H */