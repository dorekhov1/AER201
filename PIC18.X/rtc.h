/* 
 * File:   rtc.h
 * Author: daniil
 *
 * Created on February 2, 2018, 7:13 PM
 */

#ifndef RTC_H
#define	RTC_H

#include <xc.h>
#include <stdio.h>
#include "arduino.h"

void readTime(char* time) {
    unsigned char i; // Loop counter
    
    /* Reset RTC memory pointer. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    
    /* Read current time. */
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(i = 0; i < 6; i++){
        time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
}

void printTimeToGLCD() {
    unsigned char time[7];
    unsigned char i;
    
    readTime(time);
    
    sendByteToArduino('T');
    for(i = 0; i < 7; i++){
        sendByteToArduino(time[i]);
    }
    sendByteToArduino('C');
}

#endif	/* RTC_H */

