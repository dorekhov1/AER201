// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ARDUINO_H
#define	ARDUINO_H

#include <xc.h>
#include "configBits.h"
#include "I2C.h"
#include "main.h"

unsigned char buffer[100];
int b_count = 0;

int inProgress;

void waitForCompletion(void) {
    I2C_Master_Start();
    I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
    int data = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    __delay_ms(10);
    while (data == 0){
        I2C_Master_Start();
        I2C_Master_Write(0b00010001); // 7-bit Arduino slave address + Read
        data = I2C_Master_Read(NACK);
        I2C_Master_Stop();
        __delay_ms(10);
    }
}

int arduinoIsInProgress(void) {
    return inProgress;
}

void prepareBuffer(char c) {
    buffer[b_count] = c;
    b_count++;
}

void sendByteToArduino(unsigned char byte) {
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(byte);
    I2C_Master_Stop();
    __delay_ms(10);
}

void setArduinoToStandby (void) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('S');
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToLogs (unsigned char byte) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('L');
    sendByteToArduino(byte);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToDisplayLogs(unsigned char* startTime, int operationNum, int rCount, int fCount, int lCount, unsigned int runTime) {
    inProgress = 1;
    sendByteToArduino('D');
    sendByteToArduino(operationNum);
    for(unsigned int i = 0; i < 7; i++) sendByteToArduino(startTime[i]);
    for(unsigned int i = 0; i < b_count; i++) sendByteToArduino(buffer[i]);
    sendByteToArduino(rCount);
    sendByteToArduino(fCount);
    sendByteToArduino(lCount);
    sendByteToArduino(runTime);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    b_count = 0;
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToDisplayLogsEEPROM(void) {
    inProgress = 1;
    sendByteToArduino('D');
    for(unsigned int i = 0; i < b_count; i++) sendByteToArduino(buffer[i]);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    b_count = 0;
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToInput(unsigned char byte) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino(byte);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToInputNum(unsigned int num) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino('Q');
    sendByteToArduino(num);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToRunCounter(unsigned char counter, unsigned int state) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('K'); //Kounter
    sendByteToArduino(counter);
    sendByteToArduino('0'+state);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToRunArm(int arm) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('A');
    sendByteToArduino('0'+arm);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void setArduinoToRunMoving(int pos) {
    inProgress = 1;
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('M');
    sendByteToArduino('0'+pos);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

void sendCountToArduino(int r, int f, int l) {
    inProgress = 1;
    sendByteToArduino('K');
    sendByteToArduino(r);
    sendByteToArduino(f);
    sendByteToArduino(l);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    waitForCompletion();
    inProgress = 0;
}

#endif	/* ARDUINO_H */