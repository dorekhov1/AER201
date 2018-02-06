/* 
 * File:   GLCD_PIC.c
 * Author: Tyler Gamvrelis
 *
 * Created on August 13, 2017, 6:00 PM
 *
 * ST7735R-driven 128 x 128 GLCD, V1.1 red PCB
 */

/********************************** Includes **********************************/
#include "GLCD_PIC.h"
#include "SPI_PIC.h"

/**************************** External variables ******************************/
MADCTLbits_t MADCTLbits;

/****************************** Public Interfaces *****************************/
void glcdTransfer(unsigned char byte, unsigned char cmd){
   /* Driver to interface with the SPI module to send data to the GLCD.
    * 
    * Arguments: byte, the byte corresponding to the desired command/data value
    *            cmd, 1 --> command for display controller, 0 --> data for RAM
    * 
    * Returns: none
    */
    
    RS_GLCD = (cmd == 1) ? 0 : 1; // RS low for command, and high for data
    
    /* Enable serial interface and indicate the start of data transmission by
     * selecting the display (slave) for use with SPI. */
    CS_GLCD = 0;
    
    spiSend(byte); // Transmit data
    
    CS_GLCD = 1; // Deselect display
}

void glcdDrawRectangle(unsigned char XS, unsigned char XE,\
                        unsigned char YS, unsigned char YE,\
                        unsigned long color){
    /* Draws a solid rectangle in the specified window.
     *
     * Arguments: XS, the start position on the x-axis (min: 0, max: GLCD_SIZE_HORZ)
     *            XE, the end position on the x-axis (min: XS, max: GLCD_SIZE_HORZ)
     *            YS, the start position on the y-axis (min: 0, max GLCD_SIZE_VERT)
     *            YE, the end position on the y-axis (min: YS, max: GLCD_SIZE_VERT)
     *            color, the color specified
     * 
     * Returns: none
     */
    
    /* This first control flow structure is to issue "software fixes" for
     * the rotation settings. These adjustments are performed to ensure that
     * arguments for XS, XE, YS, and YE in the acceptable range will always
     * be placed in display RAM that's pixel-mapped on our display panel.
     * RECALL: The mirror/exchange effects (and effectively rotation) are 
     * configured by the MADCTL register. */
    if(MADCTLbits.MX == 1 && MADCTLbits.MV ==1){
        /* Case: origin top-left. */
        YS += 3;
        YE += 3;
        XS += 2;
        XE += 2;
    }
    else if(MADCTLbits.MY == 1){
        /* Case: origin top-right. */
        YS += 2;
        YE += 2;
        XS += 3;
        XE += 3;
    }
    else if(MADCTLbits.MX == 1){
        /* Case: origin bottom-left. */
        YS += 2;
        YE += 2;
        XS += 1;
        XE += 1;
    }
    else{
        /* Case: origin bottom-right. */
        YS += 1;
        YE += 1;
        XS += 2;
        XE += 2;
    }
    
    /* Set row address counter (specifies the start (XS) and end (XE) positions
     * of the drawing window. */
    glcdTransfer(INST_RASET, CMD);
    glcdTransfer(0x00, MEMWRITE); // XS[15:8]
    glcdTransfer(XS, MEMWRITE); // XS[7:0]
    glcdTransfer(0x00, MEMWRITE); // XE[15:8]
    glcdTransfer(XE - 1, MEMWRITE); // XE[7:0]
    
    /* Set column address counter (specifies the start (YS) and end (YE)
     * positions of the drawing window. */
    glcdTransfer(INST_CASET, CMD);
    glcdTransfer(0x00, MEMWRITE); // YS[15:8]
    glcdTransfer(YS, MEMWRITE); // YS[7:0]
    glcdTransfer(0x00, MEMWRITE); // YE[15:8]
    glcdTransfer(YE - 1, MEMWRITE); // YE[7:0]
    
    __RAMWR(); // Send the RAM write command to the display controller.
    
    /* If only drawing one pixel...save the PIC processor the time for computing
     * loop parameters. */
    if((XE == XS) && (YE == YS)){
    /* Provide color data. This data will be passed as inputs to a look-up table
     * (LUT). The LUT will then output 18 bits of color to the location in data
     * RAM specified by the row address pointer and column address pointer. The
     * memory-mapped pixel will then take on the specified color configuration.
     * Note that we do not have to send a command before doing this because the
     * previous commands take care of all the require configuration details. */
        glcdTransfer(color & 0xFF, MEMWRITE); // Blue pixel data
        glcdTransfer((color >> 8) & 0xFF, MEMWRITE); // Green pixel data
        glcdTransfer((color >> 16) & 0xFF, MEMWRITE); // Red pixel data
    }
    else{
    /* Write color data to the GLCD for all the pixels in the window. Note that
     * the GLCD controller auto-increments the addresses being written to in the
     * RAM, which is why we can continuously write after specifying a window. 
     *
     * Also, the following were done for efficiency:
     *  1. Pre-compute number of loops (very slow at runtime because of large
     *     multiplication!)
     *  2. Extract data for individual colors from the argument (also slow if
     *     you had to do this every single time you ran the loop!)
     *  3. Directly use the spiSend function to minimize function call overhead
     */
        unsigned short numLoops = (XE - XS) * (YE - YS);
        unsigned char colorData[3];
        colorData[0] = color & 0xFF;
        colorData[1] = (color >> 8) & 0xFF;
        colorData[2] = (color >> 16) & 0xFF;
        
        CS_GLCD = 0; // Select GLCD as slave device
        RS_GLCD = 1; // Select the display data RAM
        for(unsigned short i = 0; i < numLoops; i++){
            spiSend(colorData[0]); // Blue pixel data
            spiSend(colorData[1]); // Green pixel data
            spiSend(colorData[2]); // Red pixel data
        }
        CS_GLCD = 1; // Deselect the GLCD as slave device
    }
}

void glcdDrawPixel(unsigned char XS, unsigned char YS, unsigned long color){
    /* Draws the color specified at the coordinates specified relative to the 
     * origin (top-left).
     * 
     * Arguments: XS, the x-position of the pixel (min: 0, max: 127)
     *            YS, the y-position of the pixel (min: 0, max 127)
     *            color, the color specified
     * 
     * Returns: none
     */
    
    /* Take care of edge cases. Note that there is no less than zero edge case
     * due to the unsignedness of unsigned char causing overflow instead. */
    if(XS >= GLCD_SIZE_HORZ){   XS = GLCD_SIZE_HORZ - 1;    }
    if(YS >= GLCD_SIZE_VERT){   YS = GLCD_SIZE_HORZ - 1;    }
    
    /* Draw a rectangle in the window specified by a single pixel. */
    glcdDrawRectangle(XS, XS, YS, YS, color);
}

void glcdSetCOLMOD(unsigned char numBitsPerPixel){
    /* Sets the interface pixel format. Default is 18 bits per pixel (bpp). 
     * 
     * Arguments: numBitsPerPixel, parameter controlling the format of the RGB
     *              picture data. May be either 12, 16, or 18 bpp
     * 
     * Returns: none
     */
    
    unsigned short rawData;
    switch(numBitsPerPixel){
        case 12:
            rawData = 0b00000011; // 4 bits per color
            break;
        case 16:
            rawData = 0b00000101; // 5 bits per color
            break;
        case 18:
            rawData = 0b00000110; // 6 bits per color
            break;
        default:
            rawData = 0b00000110; // case 18
            break;
    }
    glcdTransfer(INST_COLMOD, CMD);
    glcdTransfer(rawData, MEMWRITE);
}

void glcdSetOrigin(unsigned char corner){
    /* Sets MADCTLbits and sends the byte to MADCTL on the GLCD to change the
     * mirror/exchange effects and thus rotate the display.
     * 
     * Arguments: corner, the corner in which the origin is to be set.
     *              Possible values: TOP_LEFT, TOP_RIGHT, 
     *                               BOTTOM_LEFT, BOTTOM_RIGHT
     * 
     * Returns: none
     */
    
    /* Set MADCTL bits to reflect the configuration. */
    switch(corner){
        case ORIGIN_TOP_LEFT:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 1;
            break;
        case ORIGIN_TOP_RIGHT:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 0;
            MADCTLbits.MV = 0;
            break;
        case ORIGIN_BOTTOM_RIGHT:
            MADCTLbits.MY = 0;
            MADCTLbits.MX = 0;
            MADCTLbits.MV = 1;
            break;
        case ORIGIN_BOTTOM_LEFT:
            MADCTLbits.MY = 0;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 0;
            break;
        default:
            MADCTLbits.MY = 1;
            MADCTLbits.MX = 1;
            MADCTLbits.MV = 1;
            break;            
    }
    
    __SETMADCTL(); // Push changes to GLCD
}

void draw_px_char(int x, int y, px_char p) {
    uint32_t m = 0b1;
    for(int i = 0; i < 32; i++) {
        if(m & p.part[0]) glcdDrawPixel(x+i%PX_CHAR_WIDTH, y+i/PX_CHAR_WIDTH, BLACK);
        if(m & p.part[1]) glcdDrawPixel(x+(i+32)%PX_CHAR_WIDTH, y+(i+32)/PX_CHAR_WIDTH, BLACK);
        m <<= 1;
    }
}

px_char new_px_char(uint32_t a, uint32_t b) {
    px_char p;
    p.part[1] = a;
    p.part[0] = b;
    return p;
}

inline px_char px_map(char c) {
    // <editor-fold defaultstate="collapsed" desc="PX Mapping">
    switch(c) {
        case 32: return PX_32;
        case 33: return PX_33;
        case 34: return PX_34;
        case 35: return PX_35;
        case 36: return PX_36;
        case 37: return PX_37;
        case 38: return PX_38;
        case 39: return PX_39;
        case 40: return PX_40;
        case 41: return PX_41;
        case 42: return PX_42;
        case 43: return PX_43;
        case 44: return PX_44;
        case 45: return PX_45;
        case 46: return PX_46;
        case 47: return PX_47;
        case 48: return PX_48;
        case 49: return PX_49;
        case 50: return PX_50;
        case 51: return PX_51;
        case 52: return PX_52;
        case 53: return PX_53;
        case 54: return PX_54;
        case 55: return PX_55;
        case 56: return PX_56;
        case 57: return PX_57;
        case 58: return PX_58;
        case 59: return PX_59;
        case 60: return PX_60;
        case 61: return PX_61;
        case 62: return PX_62;
        case 63: return PX_63;
        case 64: return PX_64;
        case 65: return PX_65;
        case 66: return PX_66;
        case 67: return PX_67;
        case 68: return PX_68;
        case 69: return PX_69;
        case 70: return PX_70;
        case 71: return PX_71;
        case 72: return PX_72;
        case 73: return PX_73;
        case 74: return PX_74;
        case 75: return PX_75;
        case 76: return PX_76;
        case 77: return PX_77;
        case 78: return PX_78;
        case 79: return PX_79;
        case 80: return PX_80;
        case 81: return PX_81;
        case 82: return PX_82;
        case 83: return PX_83;
        case 84: return PX_84;
        case 85: return PX_85;
        case 86: return PX_86;
        case 87: return PX_87;
        case 88: return PX_88;
        case 89: return PX_89;
        case 90: return PX_90;
        case 91: return PX_91;
        case 92: return PX_92;
        case 93: return PX_93;
        case 94: return PX_94;
        case 95: return PX_95;
        case 96: return PX_96;
        case 97: return PX_97;
        case 98: return PX_98;
        case 99: return PX_99;
        case 100: return PX_100;
        case 101: return PX_101;
        case 102: return PX_102;
        case 103: return PX_103;
        case 104: return PX_104;
        case 105: return PX_105;
        case 106: return PX_106;
        case 107: return PX_107;
        case 108: return PX_108;
        case 109: return PX_109;
        case 110: return PX_110;
        case 111: return PX_111;
        case 112: return PX_112;
        case 113: return PX_113;
        case 114: return PX_114;
        case 115: return PX_115;
        case 116: return PX_116;
        case 117: return PX_117;
        case 118: return PX_118;
        case 119: return PX_119;
        case 120: return PX_120;
        case 121: return PX_121;
        case 122: return PX_122;
        case 123: return PX_123;
        case 124: return PX_124;
        case 125: return PX_125;
        case 126: return PX_126;
        default: return PX_32;
    }
    // </editor-fold>
}


void print_px_char(int x, int y, char c) {
    draw_px_char(x, y, px_map(c));
}

void print_px_string(int x, int y, char *s) {
    int idx = 0;
    while(s[idx] != '\0') {
        print_px_char(x+idx*(PX_CHAR_WIDTH+1), y, s[idx]);
        
        idx++;
    }
}

void initGLCD(void){
    /* Initializes the GLCD.
     * 
     * Note: credits go to Sumotoy for the power initialization parameters.
     * These are not something that the datasheet tells you how to configure, as
     * their meanings are directly related to the panel-driving hardware. These
     * parts of initialization are in the section "Initialization ritual", while
     * the parts most interesting for the user are in the "User-defined options"
     * section.
     * 
     * ALSO: The most important things to understand about this function are:
     *  1. It sets up the SPI interface with the GLCD
     *  2. It issues commands to the GLCD that initialize its integrated panel-
     *     driving hardware
     *  3. It sets up the COLOR DEPTH when glcdSetCOLMOD is called. This is
     *     directly related to the performance of the GLCD (i.e. how long it
     *     takes to write data to it
     *
     * Arguments: none
     *
     * Returns: none
     */
        
    /* Ensure pin I/O is correct. */
    CS_GLCD = 1; // Deselect GLCD
    RS_GLCD = 1; // Set RS high
    TRIS_CS_GLCD = 0; // Set CS data direction to output
    TRIS_RS_GLCD = 0; // Set RS data direction to output
    
    /* Start SPI module with maximum available clock frequency (FOSC / 4) */
    spiInit(4);
    
    /* Wait 20 ms, just in case this function is called before the power supply
     * to the GLCD has stabilized. */
    __delay_ms(20);
    
    /************************** Initialization ritual *************************/
    __SWRESET(); // Issue a software reset
    
    __SLPOUT(); // Force exit from sleep mode
    
    /* Configure frame rate (FR) registers. */
    glcdTransfer(INST_FRMCTR1, CMD); // Issue command to configure normal mode FR
    glcdTransfer(0x00, MEMWRITE); // Parameter 1
    glcdTransfer(0x06, MEMWRITE); // Parameter 2
    glcdTransfer(0x03, MEMWRITE); // Parameter 3
    
    glcdTransfer(INST_FRMCTR2, CMD); // Issue command to configure idle mode FR
    glcdTransfer(0x01, MEMWRITE); // Parameter 1 (default)
    glcdTransfer(0x2C, MEMWRITE); // Parameter 2 (default)
    glcdTransfer(0x2D, MEMWRITE); // Parameter 3 (default)
    
    glcdTransfer(INST_FRMCTR3, CMD); // Issue command to configure partial mode FR
    glcdTransfer(0x01, MEMWRITE); // Parameter 1 (default)
    glcdTransfer(0x2C, MEMWRITE); // Parameter 2 (default)
    glcdTransfer(0x2D, MEMWRITE); // Parameter 3 (default)
    
    glcdTransfer(INST_INVCTR, CMD); // Issue command to configure display inversion control
    glcdTransfer(0x00, MEMWRITE); // No inversion
    
    /* Configure power control. */
    glcdTransfer(INST_PWCTR1, CMD); // Issue command to configure PWCTR1 register
    glcdTransfer(0xA2, MEMWRITE); // Set GVDD to 3.9 V and AVDD to 5 V
    glcdTransfer(0x02, MEMWRITE); // Set GVCL to -4.6 V
    glcdTransfer(0x84, MEMWRITE); // Set FUNCTION to AUTO
    
    glcdTransfer(INST_PWCTR2, CMD); // Issue command to configure power supply level
    glcdTransfer(0xC5, MEMWRITE); // See datasheet pg.132 (default)
    
    glcdTransfer(INST_PWCTR3, CMD); // Issue command to configure op amp current for normal mode
    glcdTransfer(0x0A, MEMWRITE); // See datasheet pg.134  (default)
    glcdTransfer(0x00, MEMWRITE); // See datasheet pg. 134 (default)
    
    glcdTransfer(INST_PWCTR4, CMD); // Issue command to configure op amp current for idle mode
    glcdTransfer(0x8A, MEMWRITE); // See datasheet pg.134  (default)
    glcdTransfer(0x2A, MEMWRITE); // Clock frequency for voltage booster circuit divided by 2
    
    glcdTransfer(INST_PWCTR5, CMD); // Issue command to configure op amp current for partial mode
    glcdTransfer(0x8A, MEMWRITE); // See datasheet pg.138  (default)
    glcdTransfer(0x2A, MEMWRITE); // Clock frequency for voltage booster circuit divided by 2
    
    /* VCOM Control. */
    glcdTransfer(INST_VMCTR1, CMD); // Issue command to configure VCOM voltage setting
    glcdTransfer(0x3C, MEMWRITE); // Important parameter for power circuits
    
    __INVOFF(); // Force no display inversion
    
    /************************** User-defined options **************************/
    /* Configure pixel interface format. NOTE: If it is desired to improve the
     * performance of the display, you can set the display to use 12 bits of
     * color instead. The color definitions provided would then all need to be
     * changed, as well as some modifications to the glcdTransfer function where
     * the color data is being sent. */
    glcdSetCOLMOD(18); // Enforce default: 18 bits of color per pixel
    
    /* Set gamma curve. */
    glcdTransfer(INST_GAMSET, CMD); // Issue command to set gamma curve
    glcdTransfer(0x01, MEMWRITE); // Gamma curve 2.2 (default)
    
    __IDMOFF(); // Force exit from idle mode (mandatory)
    
    __NORON(); // Force normal display mode (mandatory)
    
    /* Set mirror/exchange effects (datasheet pg. 63). */
    MADCTLbits.MX = 1;
    MADCTLbits.MY = 1;
    MADCTLbits.MV = 1;
    MADCTLbits.ML = 0;
    MADCTLbits.RGB = 0;
    MADCTLbits.MH = 0;
    __SETMADCTL();
    
    /* Fill black rectangle to overwrite default white value. This indicates the
     * GLCD is working properly. */
    glcdDrawRectangle(0, GLCD_SIZE_HORZ, 0, GLCD_SIZE_VERT, WHITE);
    
    __DISPON(); // Turn on display (enable output from frame memory; mandatory)
    __delay_ms(10); // Just in case the display needs time to turn on
}