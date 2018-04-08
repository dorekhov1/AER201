#ifndef INPUTHANDLER_H
#define	INPUTHANDLER_H

#include <string.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

const uint8_t cursorX = 5;
const uint8_t cursorY = 15;

Adafruit_ST7735 tft = Adafruit_ST7735(2, 3, -1);

static const char* DIETS[] = {"R", "F", "L", "RF", "RL", "FL", "RRF", "RRL", "RFF", "RLL", "RFL", "FFL", "FLL", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"};

void initGlcd(void) {
  tft.initR(INITR_144GREENTAB);    
  tft.fillScreen(ST7735_WHITE);
}

void printTime(char * timeString) {
    tft.fillRect(0, 0, tft.width(), 10, ST7735_BLUE);
    tft.setCursor(cursorX, 1);
    tft.setTextColor(ST7735_WHITE);
    tft.setTextWrap(true);
    tft.print(timeString);
    tft.setTextColor(ST7735_BLACK);
}

void processTime(char* buffer) {
    char timeString[50];
    sprintf(timeString, "%02x/%02x/%02x    %02x:%02x:%02x", buffer[7],buffer[6],buffer[5],buffer[3],buffer[2],buffer[1]);
    printTime(timeString);
}

void clearScreen(){
    tft.fillRect(0, 10, tft.width(), tft.height()-10, ST7735_WHITE);
}

void processStandby(void) {    
	clearScreen();
    tft.fillRect(0, 10, tft.width(), tft.height()-10, ST7735_WHITE);
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Welcome! \n\n Please load the food into the reservoirs. \n\n Then, press A \n to begin.\n\n Press # to \n transfer logs to PC."));
}

void drawLegend(void) {
    tft.fillRect(0, tft.height()-20, tft.width(), tft.height(), ST7735_BLUE);
    tft.setCursor(5, tft.height()-19);
    tft.print(F("Legend: B-go back, \n C-continue, D-delete"));
}

void displayEmptyInput(void) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Press * to create a \n new operation. \n\n Press B to go back  \n to standby."));
}

void displayDestinationInput(void) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Input the \n destination drawer. \n\n Value must be \n from 1 to 16."));

    drawLegend();
}

void displayDietInput(void) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Input diet type: \n\n R-1 F-2 L-3 RF-4 \n RL-5 FL-6 RRF-7 \n RRL-8 RFF-9 RLL-10 \n RFL-11 FFL-12 FLL-13\n RRFL-14 RFFL-15 \n RFLL-16 RLLL-17 \n FLLL-18"));

    drawLegend();
}

void displayDietNumInput(uint8_t num) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    char s[] = "Input the number of \n times to repeat diet \n\n Value must be \n From 1 to";
    sprintf(s, "%s %d", s, num);
    tft.print(s);
    
    drawLegend();
}

void displayPromptInput(void) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Press * to create a\n new operation.\n Press # to view\n created operations.\n Press A to begin\n execution of the\n operations.\n\n To go back to\n standby you must \n delete all created\n operations."));
}

void displayShowInput(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Press B to go back.\n Press C to view next\n operation.\n Press D to delete\n this operation."));
}

void displayOperationsComplete(void) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Press C to log \n operations.\n\n Press * to view logs\n\n Press # to transfer  logs\n\n Press B to go to\n standby."));
}

void displayLogsPrompt(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("EPPROM memory is\n filled. \n\n Press D to clear\n memory. \n\n Press # to view logs\n\n Press B to go to\n standby."));
}

void displayLogging(void) { 
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Logging is in\n progress. \n\n Do not turn off \n the machine!"));
}

void displayLoggingComplete(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Logging is complete! \n\n Press # to view logs\n\n Press B to go to\n standby."));
}

void displayTransferLogs(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Connect the USB port\n with PC's COM1 \n USB port and \n run the script. \n\n Then, press C to \n begin log transfer. \n\n Press B to go to\n standby."));
}

void displayViewLogs(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Press * to \n view logs on GLCD. \n\n Press # to transfer\n logs with USB. \n\n Press B to go to\n standby."));
}

void displayTransferringLogs(void) {  
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    tft.print(F("Transferring logs \n is in progress! \n\n Do not detach the \n USB or turn off \n the machine or PC."));
}

void displayRunmodePrompt(uint8_t top, uint8_t r, uint8_t f, uint8_t l, uint8_t cartPos, uint8_t cartMoving, uint8_t armRun, int rCount, int fCount, int lCount) {
    clearScreen();
    tft.setCursor(cursorX, cursorY);
    char s[200] = "Counters running:\n";

    if (top) sprintf(s, "%s %s", s, "Top");
    if (r) sprintf(s, "%s %s", s, "R");
    if (f) sprintf(s, "%s %s", s, "F");
    if (l) sprintf(s, "%s %s", s, "L");
    if (!top && !r && !f && !l) sprintf(s, "%s %s", s, "None");

    if (cartMoving) sprintf(s, "%s\n\n Cart moving \n to position: %d", s, cartPos);
    else sprintf(s, "%s\n\n Cart position: %d", s, cartPos);
    
    if (armRun == 4) sprintf(s, "%s\n\n Opening drawers", s);
    else if (armRun != 255) sprintf(s, "%s\n\n Closing drawer: \n row %d", s, armRun+1);
  
    sprintf(s, "%s\n\n R:%d F:%d L:%d", s, rCount, fCount, lCount); 

    tft.print(s);
}

void displayOperationInfo(unsigned char *buffer) {
  clearScreen();
  tft.setCursor(cursorX, cursorY);
  int operationNum = buffer[1];
  char s[200]="Start time:";
  
  sprintf(s, "%s\n %02x/%02x/%02x   %02x:%02x:%02x \n Operations:",s, buffer[8],buffer[7],buffer[6],buffer[4],buffer[3],buffer[2]);

  for (uint8_t i=0; i<operationNum; i++) {
    short int dest = buffer[9+3*i];
    short int num = buffer[11+3*i];
    boolean taped = 0;
    if (dest > 100) {
      taped = 1;
      dest -= 100;
    }
    if (num == 1) sprintf(s, "%s\n   %s into %d", s, DIETS[buffer[10+3*i]-1],dest);
    else sprintf(s, "%s\n   %sx%d into %d", s, DIETS[buffer[10+3*i]-1], num,dest);
    if (taped) sprintf(s, "%s tape", s);
  }
  unsigned int runTime = buffer[15+(operationNum-1)*3];
  sprintf(s, "%s\n R:%d F:%d L:%d",s, buffer[12+(operationNum-1)*3], buffer[13+(operationNum-1)*3], buffer[14+(operationNum-1)*3]); 
  sprintf(s, "%s\n Run Duration: %u s", s, runTime);

  tft.print(s);
}

#endif	/* INPUTHANDLER_H */
