
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include "SPI_PIC.h"
#include "I2C.h"

void useI2C(void){
    SSPCON1bits.SSPEN = 0;
    
    /* Force data and clock pin data directions. */
    TRISCbits.TRISC3 = 1; // SCL (clock) pin
    TRISCbits.TRISC4 = 1; // SDA (data) pin
    
    /* See PIC18F4620 datasheet, section 17.4 for I2C configuration. */
    SSPSTAT = 0b10000000; // Disable slew rate control for cleaner signals
    SSPCON1 = 0b00101000; // Clear errors & enable the serial port in master mode
    SSPCON2 = 0b00000000; // Set entire I2C operation to idle
    
    /* See section 17.4.6 in the PIC18F4620 datasheet for master mode details.
     * Below, the baud rate is configured by writing to the SSPADD<6:0>
     * according to the formula given on page 172. */
    SSPADD = (_XTAL_FREQ / (4 * 100000)) - 1;
}

void useSPI(void){
    spiInit(4);
}

#endif	/* XC_HEADER_TEMPLATE_H */

