/* 
 * File:   runmodeHandler.h
 * Author: Daniil Orekhov
 *
 * Created on February 2, 2018, 4:45 PM
 */

#ifndef LOGSMODEHANDLER_H
#define	LOGSMODEHANDLER_H

#include <xc.h>
#include "main.h"
#include "eep.h"

int currentLogsMode = MODE_OPERATIONS_COMPLETE;

int logsSaved = 0;
int fromStandby = 0;

void setFromStandby(int b) {
    fromStandby = b;
}
void setLogsSaved(int s) {
    logsSaved = s;
}

void setLogsMode(int mode){
    currentLogsMode = mode;
}
int getLogsMode(void) {
    return currentLogsMode;
}

void showOperationsComplete(void) {
    setArduinoToLogs('O');
    __lcd_clear();
    printf("Do you want to ");
    __lcd_newline();
    printf("log operations? ");
    while(currentLogsMode == MODE_OPERATIONS_COMPLETE);
}

void showLogsPrompt(void) {
    setArduinoToLogs('P');
    __lcd_clear();
    printf("Not enough ");
    __lcd_newline();
    printf("memory. ");
    while(currentLogsMode == MODE_LOGS_PROMPT);
}

void showLogging(void) {
    setArduinoToLogs('L');
    __lcd_clear();
    printf("Logging is ");
    __lcd_newline();
    printf("in progress. ");
}

void showLoggingComplete(void) {
    setArduinoToLogs('D');
    __lcd_clear();
    printf("Logging ");
    __lcd_newline();
    printf("is complete!");
    while(currentLogsMode == MODE_LOGGING_COMPLETE);
}

void showViewLogs(void) {
    setArduinoToLogs('V');
    __lcd_clear();
    printf("Please connect ");
    __lcd_newline();
    printf("to COM1 port. ");
    while(currentLogsMode == MODE_VIEW_LOGS);
}

void showTransferringLogs(void) {
    setArduinoToLogs('T');
    __lcd_clear();
    printf("Logs are ");
    __lcd_newline();
    printf("transferring. ");
}

void completeLogs(void) {
    logsSaved = 1;
    currentLogsMode = MODE_LOGGING_COMPLETE;
}

void completeTransfer(void) {
    __delay_ms(1000);
    if (fromStandby || logsSaved) currentLogsMode = MODE_RETURN;
    else currentLogsMode = MODE_OPERATIONS_COMPLETE;
}

unsigned int enoughSpace(void) {
    return (256 - getCurrentAddress()) >  (20 + 3 *getOperationNum());
}

void processLogsInterrupt(char keypress){
    if (currentLogsMode == MODE_OPERATIONS_COMPLETE) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 14) currentLogsMode = MODE_VIEW_LOGS;
        else if (keypress == 11) {
            if (enoughSpace()) currentLogsMode = MODE_LOGGING;
            else currentLogsMode = MODE_LOGS_PROMPT;
        }
    }
    else if (currentLogsMode == MODE_LOGS_PROMPT) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 15) {
            cleanEEPROM();
            currentLogsMode = MODE_OPERATIONS_COMPLETE;
        }
        else if (keypress == 14) currentLogsMode = MODE_VIEW_LOGS;
    }
    else if (currentLogsMode == MODE_LOGGING_COMPLETE) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 14) currentLogsMode = MODE_VIEW_LOGS;
    }
    else if (currentLogsMode == MODE_VIEW_LOGS) {   
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 11) currentLogsMode = MODE_TRANSFERRING_LOGS;
    }
}

#endif	/* LOGSMODEHANDLE
R_H */