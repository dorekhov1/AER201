
#ifndef EEP_H
#define	EEP_H

#include <xc.h> // include processor files - each processor file is guarded.  

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
    return( EEDATA );
}

#endif	/* XC_HEADER_TEMPLATE_H */

