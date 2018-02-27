#ifndef INPUTHANDLER_H
#define	INPUTHANDLER_H

#include <string.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS     2
#define TFT_RST    -1
#define TFT_DC     3

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void initGlcd(void) {
  tft.initR(INITR_144GREENTAB);    
  tft.fillScreen(ST7735_WHITE);
}

void printTime(char * timeString) {
    tft.fillRect(0, 0, tft.width(), 10, ST7735_BLUE);
    tft.setCursor(5, 1);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextWrap(true);
    tft.print(timeString);
    tft.setTextColor(ST7735_BLACK);
}

void clearScreen(){
    tft.fillRect(0, 10, tft.width(), tft.height()-10, ST7735_WHITE);
}

void processStandby(void) {    
	clearScreen();
    tft.fillRect(0, 10, tft.width(), tft.height()-10, ST7735_WHITE);
    tft.setCursor(5, 15);
    tft.print(F("Welcome! \n\n Please load the food into the reservoirs. \n\n Then, press A \n to begin.\n\n Press # to \n transfer logs to PC."));
}

void drawLegend(void) {
    tft.fillRect(0, tft.height()-20, tft.width(), tft.height(), ST7735_BLUE);
    tft.setCursor(5, tft.height()-19);
    tft.print(F("Legend: B-go back, \n C-continue, D-delete"));
}

void displayEmptyInput(void) {
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Press * to create a \n new operation. \n\n Press B to go back  \n to standby."));
}

void displayDestinationInput(void) {
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Input the \n destination drawer. \n\n Value must be \n from 1 to 16."));

    drawLegend();
}

void displayDietInput(void) {
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Input diet type: \n\n R-1 F-2 L-3 RF-4 \n RL-5 FL-6 RRF-7 \n RRL-8 RFF-9 RLL-10 \n RFL-11 FFL-12 FLL-13\n RRFL-14 RFFL-15 \n RFLL-16 RLLL-17 \n FLLL-18"));

    drawLegend();
}

void displayDietNumInput(int num) {
    clearScreen();
    tft.setCursor(5, 15);
    char s[100];
    sprintf(s, "Input the number of \n times to repeat diet \n\n Value must be \n From 1 to %d", num);
    tft.print(s);
    
    drawLegend();
}

void displayPromptInput(void) {
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Press * to create a\n new operation.\n Press # to view\n created operations.\n Press A to begin\n execution of the\n operations.\n\n To go back to\n standby you must \n delete all created\n operations."));
}

void displayShowInput(void) {  
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Press B to go back.\n Press C to view next\n operation.\n Press D to delete\n this operation."));
}


void displayOperationsComplete(void) {
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Press C to log \n operations.\n\n Press # to view logs\n\n Press B to go to\n standby."));
}

void displayLogsPrompt(void) {  
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("EPPROM memory is \n\n filled. \n Press D to clear\n memory. \n\n Press # to view logs\n\n Press B to go to\n standby."));
}

void displayLogging(void) { 
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Logging is in\n progress. \n\n Do not turn off \n the machine!"));
}

void displayLoggingComplete(void) {  
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Logging is complete! \n\n Press # to view logs\n\n Press B to go to\n standby."));
}

void displayViewLogs(void) {  
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Connect the USB port\n with PC's COM1 \n USB port and \n run the script. \n\n Then, press C to \n begin log transfer. \n\n Press B to go to\n standby."));
}

void displayTransferringLogs(void) {  
    clearScreen();
    tft.setCursor(5, 15);
    tft.print(F("Transferring logs \n is in progress! \n\n Do not detach the \n USB or turn off \n the machine or PC."));
}

#endif	/* INPUTHANDLER_H */
