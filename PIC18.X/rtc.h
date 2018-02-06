/* 
 * File:   rtc.h
 * Author: daniil
 *
 * Created on February 2, 2018, 7:13 PM
 */

#ifndef RTC_H
#define	RTC_H

#include <xc.h>
#include "protocolHandler.h"

void readTime(char* time) {
    unsigned char i; // Loop counter
    useI2C();
    
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
    unsigned char* time;
    readTime(time);
    char timeString[10];
    useSPI();
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, 14, BLUE);
    sprintf(timeString, "%02x/%02x/%02x    %02x:%02x:%02x", time[6],time[5],time[4], time[2],time[1],time[0]);
    print_px_string(5, 1, timeString);
}

#endif	/* RTC_H */

