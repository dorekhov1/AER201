// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ARDUINO_H
#define	ARDUINO_H

#include <xc.h>
#include "configBits.h"
#include "I2C.h"

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
}

void setArduinoToLogs (unsigned char byte) {
    sendByteToArduino('M');
    sendByteToArduino('L');
    sendByteToArduino(byte);
    sendByteToArduino('C');
}

void setArduinoToInput(unsigned char byte) {
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino(byte);
    sendByteToArduino('C');
}

void setArduinoToInputNum(unsigned int num) {
    sendByteToArduino('M');
    sendByteToArduino('I');
    sendByteToArduino('Q');
    sendByteToArduino(num);
    sendByteToArduino('C');
}

void setArduinoToRunCounter(unsigned char counter, unsigned char state) {
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('K'); //Kounter
    sendByteToArduino(counter);
    sendByteToArduino(state);
    sendByteToArduino('C');
}

void setArduinoToRunArm(char arm) {
    sendByteToArduino('M');
    sendByteToArduino('R');
    sendByteToArduino('A');
    sendByteToArduino(arm);
    sendByteToArduino('C');
}

#endif	/* ARDUINO_H */