/* 
 * File:   GLCD_PIC.h
 * Author: Tyler Gamvrelis
 *
 * Created on August 13, 2017, 6:00 PM
 */

#ifndef GLCD_PIC_H
#define	GLCD_PIC_H

/********************************** Includes **********************************/
#include <xc.h>
#include <stdint.h>
#include "configBits.h"

/*********************************** Macros ***********************************/
/* Display write commands (one-liners and 2-liners only). */
#define __NOP()     glcdTransfer(INST_NOP, CMD)      // Empty processor cycle
#define __SWRESET(){\
    /* All registers to default state. */   \
    glcdTransfer(INST_SWRESET, CMD); /* Issue command. */   \
    __delay_ms(130); /* Delay specified on pg. 83 of datasheet. */  \
}
#define __SLPIN(){\
    /* Enter sleep mode. */ \
    glcdTransfer(INST_SLPIN, CMD); /* Issue command. */ \
    __delay_ms(130); /* Delay specified on pg. 93 of datasheet to stabilize
                        power circuits. */  \
}
#define __SLPOUT(){\
    /* Exit sleep mode. */ \
    glcdTransfer(INST_SLPOUT, CMD); /* Issue command. */ \
    __delay_ms(130); /* Delay specified on pg. 94 of datasheet to stabilize
                        timing for supply voltages and clock circuits. */  \
}
#define __SETMADCTL(){\
    /* Set the mirror/exchange parameters. */ \
    glcdTransfer(INST_MADCTL, CMD);\
    glcdTransfer(MADCTLbits.reg, MEMWRITE);\
}
#define __PTLON()   glcdTransfer(INST_PTLON, CMD)    // Partial mode on
#define __NORON()   glcdTransfer(INST_NORON, CMD)    // Partial mode off (normal)      
#define __INVOFF()  glcdTransfer(INST_INVOFF, CMD)   // Display inversion off   
#define __INVON()   glcdTransfer(INST_INVON, CMD)    // Display inversion on      
#define __DISPOFF() glcdTransfer(INST_DISPOFF, CMD)  // Turn off display
#define __DISPON()  glcdTransfer(INST_DISPON, CMD)   // Turn on display
#define __RAMWR()   glcdTransfer(INST_RAMWR, CMD)    // Enable display data RAM writing
#define __TEOFF()   glcdTransfer(INST_TEOFF, CMD);   // Tearing effect off
#define __IDMOFF()  glcdTransfer(INST_IDMOFF, CMD)   // Idle mode off
#define __IDMON()   glcdTransfer(INST_IDMON, CMD)    // Idle mode on

#define __glcd_clear() glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, GLCD_SIZE_VERT, WHITE);

/*********************************** Defines **********************************/
/* RD1 is RS, which is DCX (data/command flag) in the datasheet.
 * 
 * RS = 0 --> command to display controller
 * RS = 1 --> data for display data RAM
 */
#define RS_GLCD          LATDbits.LATD1
#define TRIS_RS_GLCD     TRISDbits.TRISD1

/* RD0 is CS, which is CSX (chip enable) in the datasheet. 
 * 
 * CS = 0 --> display selected for control via SPI
 * CS = 1 --> display not selected for control
 */
#define CS_GLCD          LATDbits.LATD0
#define TRIS_CS_GLCD     TRISDbits.TRISD0

/* Arguments for low-level driver, glcdTransfer. */
#define CMD             1
#define MEMWRITE        0

/* Origin locations (relative to the display when mounted on the DevBugger). */
#define ORIGIN_TOP_LEFT        0
#define ORIGIN_TOP_RIGHT       1
#define ORIGIN_BOTTOM_RIGHT    2
#define ORIGIN_BOTTOM_LEFT     3

/********************************** Constants *********************************/
/* Display dimensions addressable in ST7735 controller display data RAM. */
const unsigned char GLCD_ADDRESSABLE_SIZE_HORZ = 128; // 128 pixels
const unsigned char GLCD_ADDRESSABLE_SIZE_VERT = 160; // 160 pixel

/* Display dimensions as seen in the real world. */
const unsigned char GLCD_SIZE_HORZ = 128;    // 0 to 127 --> 128 pixels
const unsigned char GLCD_SIZE_VERT = 128;    // 0 to 127 --> 128 pixels

/* Display command codes (write only, since we don't have hardware to read).
 * These are static which makes them only visible to this compilation unit. */
static const unsigned char INST_NOP = 0x00;         // Empty processor cycle
static const unsigned char INST_SWRESET = 0x01;     // All registers to default state
static const unsigned char INST_SLPIN = 0x10;       // Enter sleep mode
static const unsigned char INST_SLPOUT = 0x11;      // Exit sleep mode
static const unsigned char INST_PTLON = 0x12;       // Partial mode on
static const unsigned char INST_NORON = 0x13;       // Partial mode off (normal)
static const unsigned char INST_INVOFF = 0x20;      // Display inversion off
static const unsigned char INST_INVON = 0x21;       // Display inversion on
static const unsigned char INST_GAMSET = 0x26;      // Set gamma
static const unsigned char INST_DISPOFF = 0x28;     // Turn off display
static const unsigned char INST_DISPON = 0x29;      // Turn on display
static const unsigned char INST_CASET = 0x2A;       // Set column address
static const unsigned char INST_RASET = 0x2B;       // Set row address
static const unsigned char INST_RAMWR = 0x2C;       // Enables RAM writes
static const unsigned char INST_PTLAR = 0x30;       // Partial start/end address
static const unsigned char INST_TEOFF = 0x34;       // Tearing effect off
static const unsigned char INST_TEON = 0x35;        // Tearing effect on
static const unsigned char INST_MADCTL  = 0x36;     // Memory data access control
static const unsigned char INST_IDMOFF = 0x38;      // Idle mode off
static const unsigned char INST_IDMON = 0x39;       // Idle mode on
static const unsigned char INST_COLMOD = 0x3A;      // Interface pixel format
static const unsigned char INST_FRMCTR1 = 0xB1;     // Frame rate control (normal mode/full colors)
static const unsigned char INST_FRMCTR2 = 0xB2;     // Frame rate control (idle mode/8-colors)
static const unsigned char INST_FRMCTR3 = 0xB3;     // Frame rate control (partial mode/full colors)
static const unsigned char INST_INVCTR = 0xB4;      // Display inversion control
static const unsigned char INST_PWCTR1 = 0xC0;      // Power control1
static const unsigned char INST_PWCTR2 = 0xC1;      // Power control2
static const unsigned char INST_PWCTR3 = 0xC2;      // Power control3
static const unsigned char INST_PWCTR4 = 0xC3;      // Power control4
static const unsigned char INST_PWCTR5 = 0xC4;      // Power control5
static const unsigned char INST_VMCTR1 = 0xC5;      // VCOM control 1
static const unsigned char INST_VMOFCTR2 = 0xC7;    // VCOM control 2	

/* 18-bit color depth information here.
 * 
 * Each color is encoded by 3 bytes. For each byte, the 6 most significant bits
 * contain useful information and the two lest significant bits are don't cares.
 * Color = B2B1B0 = RGB.
 * 
 * Thus, for each color component (red, green and blue or RGB for short), there 
 * are 2^6 possible values. Since there are three different colors, there are
 * thus (2^6)^3 = 2^18 = 262,144 colors.
 * 
 * The RGB color model is additive, that is, an arbitrary color in the RGB color
 * space is formed by superimposing the intensities of the separate components.
 * Thus, black is zero intensity for all components, and white is full intensity
 * for all components.
 */
const unsigned long BLACK = 0x000000;
const unsigned long GREY = 0x808080;
const unsigned long WHITE = 0xFFFFFF;
const unsigned long RED = 0xFF0000;
const unsigned long ORANGE = 0xFF8C00;
const unsigned long YELLOW = 0xFFFF00;
const unsigned long GREEN = 0x00FF00;
const unsigned long BLUE = 0x0000FF;
const unsigned long INDIGO = 0x4B0082;
const unsigned long VIOLET = 0x9400D3;

/************************************ Types ***********************************/
/* Define union for MADCTL for easy bit addressing. This will be useful for 
 * determining if rotations are in effect. A union is basically used to provide
 * several different ways of using the same space in memory. In this case, we 
 * are using the same space in memory to define a struct with individual bits we
 * can address, and an unsigned char (byte) that encompasses these bits. This
 * way, we can easily set and test the individual bits while still being able to
 * pass the entire byte into a function without sacrificing efficiency.
 * 
 *  Bits:       MY, mirror y-axis (0 --> top to bottom)
 *              MX, mirror x-axis (0 --> left to right)
 *              MV, row/column exchange (0 --> normal)
 *              ML, scan direction parameter (0 --> refresh top to bottom)
 *              RGB, red, green, and blue pixel position change (0 --> RGB)
 *              MH, horizontal refresh order (0 --> refresh left to right)
 * Unsigned char (byte):
 *              reg, encompasses all the bytes in the struct
 */
typedef union{
    struct{
        unsigned        :1; // LSb
        unsigned        :1;
        unsigned MH     :1;
        unsigned RGB    :1;
        unsigned ML     :1;
        unsigned MV     :1;
        unsigned MX     :1;
        unsigned MY     :1; // MSb 
    };
    unsigned char reg; // Alternate way of accessing the above 8 bits, as a byte
}MADCTLbits_t;


typedef struct {
    uint32_t part[2];
} px_char;


/**************************** External variables ******************************/
extern MADCTLbits_t MADCTLbits;

/****************************** Public Interfaces *****************************/
/* Fundamental communication interface. */
void glcdTransfer(unsigned char byte, unsigned char cmd);

/* Fundamental drawing interfaces. */
void glcdDrawRectangle(unsigned char XS, unsigned char XE,\
                        unsigned char YS, unsigned char YE,\
                        unsigned long color);
void glcdDrawPixel(unsigned char XS, unsigned char YS, unsigned long color);
void draw_px_char(int x, int y, px_char p);

/* The following two functions may be used by students who:
 *  1. Want to maximize write efficiency to the GLCD by minimizing color depth
 *  2. Want to change the location of the origin (i.e. rotate the display). */
void glcdSetCOLMOD(unsigned char numBitsPerPixel);
void glcdSetOrigin(unsigned char corner);

/* Initialization sequence. */
void initGLCD(void);



#define PX_CHAR_WIDTH 5
#define PX_CHAR_HEIGHT 12

// <editor-fold defaultstate="collapsed" desc="PX Defines">
#define PX_32 new_px_char(0b00000000000000000000000000000000,0b00000000000000000000000000000000)
#define PX_33 new_px_char(0b00000000000000001000000000100001,0b00001000010000100001000000000000)
#define PX_34 new_px_char(0b00000000000000000000000000000000,0b00000000101001010010100000000000)
#define PX_35 new_px_char(0b00000000000000010100101011111010,0b10111110101001010000000000000000)
#define PX_36 new_px_char(0b00000000000100011111010010100011,0b10001010010111110001000000000000)
#define PX_37 new_px_char(0b00000000001001101010101000010001,0b00010000101010101100100000000000)
#define PX_38 new_px_char(0b00000000010110010011100100101000,0b10001010010100010000000000000000)
#define PX_39 new_px_char(0b00000000000000000000000000000000,0b00000000010000100001000000000000)
#define PX_40 new_px_char(0b00000100000100001000001000010000,0b10000100001000100001000100000000)
#define PX_41 new_px_char(0b00000001000100001000100001000010,0b00010000100000100001000001000000)
#define PX_42 new_px_char(0b00000000000000000000000000000001,0b00101010111010101001000000000000)
#define PX_43 new_px_char(0b00000000000000000000010000100111,0b11001000010000000000000000000000)
#define PX_44 new_px_char(0b00000001000100011000000000000000,0b00000000000000000000000000000000)
#define PX_45 new_px_char(0b00000000000000000000000000000111,0b11000000000000000000000000000000)
#define PX_46 new_px_char(0b00000010001110001000000000000000,0b00000000000000000000000000000000)
#define PX_47 new_px_char(0b00000000000001000010001000010001,0b00010000100010000100000000000000)
#define PX_48 new_px_char(0b00000000000000001000101010001100,0b01100011000110001010100010000000)
#define PX_49 new_px_char(0b00000000000000111110010000100001,0b00001000010000101001100010000000)
#define PX_50 new_px_char(0b00000000000000111110000100010001,0b00010001000010001100010111000000)
#define PX_51 new_px_char(0b00000000000000011101000110000100,0b00011100010001000100001111100000)
#define PX_52 new_px_char(0b00000000000000010000100011111010,0b01010100101001100010000100000000)
#define PX_53 new_px_char(0b00000000000000011101000110000100,0b00100110110100001000011111100000)
#define PX_54 new_px_char(0b00000000000000011101000110001100,0b01011110000100001100010111000000)
#define PX_55 new_px_char(0b00000000000000000100001000010001,0b00001000100001000100001111100000)
#define PX_56 new_px_char(0b00000000000000011101000110001100,0b01011101000110001100010111000000)
#define PX_57 new_px_char(0b00000000000000011101000110000100,0b00111101000110001100010111000000)
#define PX_58 new_px_char(0b00000000000000001000111000100000,0b00000000010001110001000000000000)
#define PX_59 new_px_char(0b00000001000100011000000000000001,0b00011100010000000000000000000000)
#define PX_60 new_px_char(0b00000000010000010000010000010000,0b01000100010001000100000000000000)
#define PX_61 new_px_char(0b00000000000000000001111100000000,0b00111110000000000000000000000000)
#define PX_62 new_px_char(0b00000000000001000100010001000100,0b00010000010000010000010000000000)
#define PX_63 new_px_char(0b00000000000100000000010000100010,0b00100001000110001011100000000000)
#define PX_64 new_px_char(0b00000000011110000010110110101101,0b01110011000110001011100000000000)
#define PX_65 new_px_char(0b00000000000000100011000110001111,0b11100011000110001010100010000000)
#define PX_66 new_px_char(0b00000000000000011111001010010100,0b10011101001010010100100111100000)
#define PX_67 new_px_char(0b00000000000000011101000100001000,0b01000010000100001100010111000000)
#define PX_68 new_px_char(0b00000000000000011111001010010100,0b10100101001010010100100111100000)
#define PX_69 new_px_char(0b00000000000000111110000100001000,0b01011110000100001000011111100000)
#define PX_70 new_px_char(0b00000000000000000010000100001000,0b01011110000100001000011111100000)
#define PX_71 new_px_char(0b00000000000000011101000110001110,0b01000010000100001100010111000000)
#define PX_72 new_px_char(0b00000000000000100011000110001100,0b01111111000110001100011000100000)
#define PX_73 new_px_char(0b00000000000000011100010000100001,0b00001000010000100001000111000000)
#define PX_74 new_px_char(0b00000000000000001100100101000010,0b00010000100001000010001110000000)
#define PX_75 new_px_char(0b00000000000000100011000101001001,0b01000110010101001100011000100000)
#define PX_76 new_px_char(0b00000000000000111110000100001000,0b01000010000100001000010000100000)
#define PX_77 new_px_char(0b00000000000000100011000110001100,0b01101011010111011100011000100000)
#define PX_78 new_px_char(0b00000000000000100011000111001110,0b01101011010110011100111000100000)
#define PX_79 new_px_char(0b00000000000000011101000110001100,0b01100011000110001100010111000000)
#define PX_80 new_px_char(0b00000000000000000010000100001000,0b01011111000110001100010111100000)
#define PX_81 new_px_char(0b00000000010000011101010110001100,0b01100011000110001100010111000000)
#define PX_82 new_px_char(0b00000000000000100011000101001001,0b01011111000110001100010111100000)
#define PX_83 new_px_char(0b00000000000000011101000110000100,0b00011100000100001100010111000000)
#define PX_84 new_px_char(0b00000000000000001000010000100001,0b00001000010000100001001111100000)
#define PX_85 new_px_char(0b00000000000000011101000110001100,0b01100011000110001100011000100000)
#define PX_86 new_px_char(0b00000000000000001000010001010010,0b10100011000110001100011000100000)
#define PX_87 new_px_char(0b00000000000000010101010110101101,0b01101011000110001100011000100000)
#define PX_88 new_px_char(0b00000000000000100011000101010010,0b10001000101001010100011000100000)
#define PX_89 new_px_char(0b00000000000000001000010000100001,0b00001000101001010100011000100000)
#define PX_90 new_px_char(0b00000000000000111110000100010000,0b10001000100001000100001111100000)
#define PX_91 new_px_char(0b00000111000010000100001000010000,0b10000100001000010000100111000000)
#define PX_92 new_px_char(0b00000000010000100000100001000001,0b00000100001000001000010000000000)
#define PX_93 new_px_char(0b00000111001000010000100001000010,0b00010000100001000010000111000000)
#define PX_94 new_px_char(0b00000000000000000000000000000000,0b00000001000101010001000000000000)
#define PX_95 new_px_char(0b00001111100000000000000000000000,0b00000000000000000000000000000000)
#define PX_96 new_px_char(0b00000000000000000000000000000000,0b00000000000001000001000011000000)
#define PX_97 new_px_char(0b00000000000000101101100110001111,0b10100000111000000000000000000000)
#define PX_98 new_px_char(0b00000000000000011111000110001100,0b01100010111100001000010000100000)
#define PX_99 new_px_char(0b00000000000000011101000100001000,0b01100010111000000000000000000000)
#define PX_100 new_px_char(0b00000000000000111101000110001100,0b01100011111010000100001000000000)
#define PX_101 new_px_char(0b00000000000000011101000100001111,0b11100010111000000000000000000000)
#define PX_102 new_px_char(0b00000000000000000100001000010000,0b10011110001000010100100110000000)
#define PX_103 new_px_char(0b00000111010001100001111010001100,0b01100010111000000000000000000000)
#define PX_104 new_px_char(0b00000000000000100011000110001100,0b01100110110100001000010000100000)
#define PX_105 new_px_char(0b00000000000000011100010000100001,0b00001000011000000001000000000000)
#define PX_106 new_px_char(0b00000011001001010010100001000010,0b00010000100001100000000100000000)
#define PX_107 new_px_char(0b00000000000000100010100100101000,0b11001010100100001000010000100000)
#define PX_108 new_px_char(0b00000000000000011100010000100001,0b00001000010000100001000011000000)
#define PX_109 new_px_char(0b00000000000000100011010110101101,0b01101010101100000000000000000000)
#define PX_110 new_px_char(0b00000000000000100011000110001100,0b01100110110100000000000000000000)
#define PX_111 new_px_char(0b00000000000000011101000110001100,0b01100010111000000000000000000000)
#define PX_112 new_px_char(0b00000000100001000010111110001100,0b01100010111100000000000000000000)
#define PX_113 new_px_char(0b00001000010000100001111010001100,0b01100011111000000000000000000000)
#define PX_114 new_px_char(0b00000000000000000010000100001000,0b01100110110100000000000000000000)
#define PX_115 new_px_char(0b00000000000000011101000101000001,0b10100010111000000000000000000000)
#define PX_116 new_px_char(0b00000000000000011001001000010000,0b10000100111100010000100000000000)
#define PX_117 new_px_char(0b00000000000000101101100110001100,0b01100011000100000000000000000000)
#define PX_118 new_px_char(0b00000000000000001000101001010100,0b01100011000100000000000000000000)
#define PX_119 new_px_char(0b00000000000000010101010110101101,0b01100011000100000000000000000000)
#define PX_120 new_px_char(0b00000000000000100010101000100001,0b00010101000100000000000000000000)
#define PX_121 new_px_char(0b00000111010001100001011011001100,0b01100011000100000000000000000000)
#define PX_122 new_px_char(0b00000000000000111110000100010001,0b00010001111100000000000000000000)
#define PX_123 new_px_char(0b00001100000100001000010000100000,0b11001000010000100001001100000000)
#define PX_124 new_px_char(0b00000000000100001000010000100001,0b00001000010000100001000000000000)
#define PX_125 new_px_char(0b00000001100100001000010000100110,0b00001000010000100001000001100000)
#define PX_126 new_px_char(0b00000000000000000000000000000000,0b00000000100110101100100000000000)
// </editor-fold>

px_char new_px_char(uint32_t a, uint32_t b);
inline px_char px_map(char c);
void print_px_char(int x, int y, char c);
void print_px_string(int x, int y, char *s);


#endif	/* GLCD_PIC_H */
