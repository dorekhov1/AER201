
#ifndef EEP_H
#define	EEP_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "main.h"
#include "math.h"

void write_octet_eep( unsigned char address, unsigned char data )
{
    while( EECON1bits.WR  )     // make sure it's not busy with an earlier write.
    {}
    // EEPROM_WRITE( address, data ); Yet more Microchip stuff that doesn't work!
    //                                  ... so try doing it directly as the datasheet defines.
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS  = 0;
    EECON1bits.WREN  = 1;
    INTCONbits.GIE   = 0;
    // required sequence start
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    // required sequence end
    INTCONbits.GIE = 1;
        __delay_ms(100);
}

unsigned char read_octet_eep( unsigned short address ){
    while( EECON1bits.WR  )     // make sure it's not busy with an earlier write.
    {}
    // return( EEPROM_READ( address ); Yet more Microchip stuff that doesn't work!
    //                                  ... so back to do it myself.
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS  = 0;
    EECON1bits.RD    = 1;
        __delay_ms(100);
    return( EEDATA );
}

void cleanEEPROM(void){
    write_octet_eep(0,1);
}

void addLines(int num) {
    write_octet_eep(0, read_octet_eep(0)+num);
}

int getCurrentAddress(void) {
    return read_octet_eep(0);
}

void logOperationNum(int operationNum) {
    write_octet_eep(getCurrentAddress(), operationNum);
    addLines(1);
}

void logTime(void) {
    unsigned char time[7];
    readTime(time);
    
    int currentLine = getCurrentAddress();
    for (int i=0; i<7; i++) {
        write_octet_eep(currentLine+i, time[i]);
    }
    addLines(7);    
}

void logTapedDrawers(int* tapedDrawers, int tapedDrawersNum) {
    unsigned int byte1 = 0;
    unsigned int byte2 = 0;

    for (int i=0; i<tapedDrawersNum; i++) {
        if (tapedDrawers[i] <= 8) byte1 += pow(2, tapedDrawers[i]-1);
        else byte2 += pow(2, tapedDrawers[i]-9);
    }
    
    write_octet_eep(getCurrentAddress(), byte1);
    write_octet_eep(getCurrentAddress()+1, byte2);
    addLines(2);
}

void logOperation(int destination, int dietType, int dietNum) {
    int currentLine = getCurrentAddress();
    write_octet_eep(currentLine, destination);
    write_octet_eep(currentLine+1, dietType);
    write_octet_eep(currentLine+2, dietNum);
    addLines(3);
}

void logCounts(int rTotalCount, int fTotalCount, int lTotalCount) {
    int currentLine = getCurrentAddress();
    write_octet_eep(currentLine, rTotalCount);
    write_octet_eep(currentLine+1, fTotalCount);
    write_octet_eep(currentLine+2, lTotalCount);
    addLines(3);
}

void logEmergency(int emergencySignal) {
    write_octet_eep(getCurrentAddress(), emergencySignal);
    addLines(1);
}

void sendLogsToPC(void) {
    int linesNum = read_octet_eep(0);
    unsigned char byte;
    
    for (int i=0; i<linesNum; i++) {
        byte = read_octet_eep(i);
        TXREG =  byte;
        __delay_ms(10);
    }
}

#endif	/* XC_HEADER_TEMPLATE_H */