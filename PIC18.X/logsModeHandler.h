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
#include "timer.h"

volatile int currentLogsMode = MODE_OPERATIONS_COMPLETE;

int logsSaved = 0;
int fromStandby = 0;

int linesDisplayed = 1;

volatile int showNewLogs = 0;

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

void wait(int mode) {
    int time = 0;
    while (currentLogsMode == mode) {
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
}

void showOperationsComplete(void) {
    __lcd_clear();
    printf("Press C to ");
    __lcd_newline();
    printf("continue ");
    wait(MODE_OPERATIONS_COMPLETE);
}

void showSaveOperations(void) {
    setArduinoToLogs('O');
    __lcd_clear();
    printf("Do you want to ");
    __lcd_newline();
    printf("log operations? ");
    wait(MODE_SAVE_OPERATIONS);
}

void showLogsPrompt(void) {
    setArduinoToLogs('P');
    __lcd_clear();
    printf("Not enough ");
    __lcd_newline();
    printf("memory. ");
    wait(MODE_LOGS_PROMPT);
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
    wait(MODE_LOGGING_COMPLETE);
}

void showViewLogs(void)  {
    setArduinoToLogs('Q');
    __lcd_clear();
    printf("Select way ");
    __lcd_newline();
    printf("to view logs.");
    wait(MODE_VIEW_LOGS);
}

void showViewLogsGLCD(void) {
    
    showNewLogs = 1;
    
    int time = 0;
    
    while(currentLogsMode == MODE_VIEW_LOGS_GLCD) {
        if (showNewLogs) {           
            __lcd_clear();
            printf("Displaying ");
            __lcd_newline();
            printf("logs. ");
            int logLength = 14 + 3 * (read_octet_eep(linesDisplayed) - 1);

            for (unsigned int i = linesDisplayed; i<=linesDisplayed+logLength; i++) {
                prepareBuffer(read_octet_eep(i));
            }
            setArduinoToDisplayLogsEEPROM();
            __lcd_clear();
            printf("Press C to ");
            __lcd_newline();
            printf("continue ");

            linesDisplayed += logLength;
            
            showNewLogs = 0;
        }
        if (readTimer() - time >= 10)  {
            time = readTimer();
            printTimeToGLCD();
        }
    }
    linesDisplayed = 0;
}

void showTransferLogs(void) {
    setArduinoToLogs('V');
    __lcd_clear();
    printf("Please connect ");
    __lcd_newline();
    printf("to COM1 port. ");
    wait(MODE_TRANSFER_LOGS);
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

void processLogsInterrupt(char keypress) {
    if (currentLogsMode == MODE_OPERATIONS_COMPLETE) {
        if (keypress == 11) currentLogsMode = MODE_SAVE_OPERATIONS;
    }
    else if (currentLogsMode == MODE_SAVE_OPERATIONS) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 14) currentLogsMode = MODE_TRANSFER_LOGS;
        else if (keypress == 11) {
            if (enoughSpace()) currentLogsMode = MODE_LOGGING;
            else currentLogsMode = MODE_LOGS_PROMPT;
        }
        else if (keypress == 12) currentLogsMode = MODE_VIEW_LOGS;
    }
    else if (currentLogsMode == MODE_LOGS_PROMPT) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 15) {
            cleanEEPROM();
            currentLogsMode = MODE_OPERATIONS_COMPLETE;
        }
        else if (keypress == 14) currentLogsMode = MODE_TRANSFER_LOGS;
    }
    else if (currentLogsMode == MODE_LOGGING_COMPLETE) {
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 14) currentLogsMode = MODE_VIEW_LOGS;
    }
    else if (currentLogsMode == MODE_TRANSFER_LOGS) {   
        if (keypress == 7) currentLogsMode = MODE_RETURN;
        else if (keypress == 11) currentLogsMode = MODE_TRANSFERRING_LOGS;
    }
    else if (currentLogsMode == MODE_VIEW_LOGS) {   
        if (keypress == 12) currentLogsMode = MODE_VIEW_LOGS_GLCD;
        else if (keypress == 14) currentLogsMode = MODE_TRANSFER_LOGS;
        else if (keypress == 7) currentLogsMode = MODE_RETURN;
    }
    else if (currentLogsMode == MODE_VIEW_LOGS_GLCD) {   
        if (keypress == 11) {
            if (linesDisplayed != getCurrentAddress()) showNewLogs = 1; 
            else currentLogsMode = MODE_VIEW_LOGS;
        }
        else if (keypress == 7) currentLogsMode = MODE_VIEW_LOGS;
    }
}

#endif	/* LOGSMODEHANDL
R_H */