// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ARDUINO_H
#define	ARDUINO_H

#include <xc.h>
#include "configBits.h"
#include "I2C.h"
#include "main.h"

char buffer[100];
int b_count = 0;

void prepareBuffer(char c) {
    buffer[b_count] = c;
    b_count++;
}

void sendByteToArduino(unsigned char byte) {
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(byte);
    I2C_Master_Stop();
}

void setArduinoToStandby (void) {
    sendByteToArduino('M');
    sendByteToArduino('S');
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToLogs (unsigned char byte) {
    sendByteToArduino('M');
    sendByteToArduino('L');
    sendByteToArduino(byte);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToDisplayLogs(unsigned char* startTime, int operationNum, int rCount, int fCount, int lCount, unsigned char* endTime) {
    sendByteToArduino('M');
    sendByteToArduino('D');
    sendByteToArduino(operationNum);
    for(unsigned int i = 0; i < 7; i++){
        sendByteToArduino(startTime[i]);
    }
    for(unsigned int i = 0; i < b_count; i++){
        sendByteToArduino(buffer[i]);
    }
    sendByteToArduino(rCount);
    sendByteToArduino(fCount);
    sendByteToArduino(lCount);
    for(unsigned int i = 0; i < 7; i++){
        sendByteToArduino(endTime[i]);
    }
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
    b_count = 0;
}

void setArduinoToInput(unsigned char byte) {
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino(byte);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToInputNum(unsigned int num) {
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino('Q');
    sendByteToArduino(num);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToRunCounter(unsigned char counter, int state) {
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('K'); //Kounter
    sendByteToArduino(counter);
    sendByteToArduino(state);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToRunArm(char arm) {
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('A');
    sendByteToArduino(arm);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

void setArduinoToRunMoving(int pos) {
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('M');
    sendByteToArduino(pos);
    sendByteToArduino('C');
    sendByteToArduino('C');
    sendByteToArduino('C');
}

#endif	/* ARDUINO_H */