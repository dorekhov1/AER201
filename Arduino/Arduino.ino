#include <Wire.h>
#include <string.h>
#include "glcdHandler.h"
#include "actuatorsHandler.h"

volatile boolean runMode = 0;

void setup(){

//  Serial.begin(9600);
  
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register a function to be called when this slave device receives a transmission from a master

  initGlcd();
  initActuators();
}

void loop(){
  if (runMode) {
    checkMoving();
    checkServos();
  }
}

char buffer[200];
uint8_t b_count = 0;
uint8_t c_count = 0;

void receiveEvent(void){
  char b = Wire.read();
  if (b == 'C') c_count++;
  else c_count = 0;

  if (c_count == 3) {
    processBuffer();
    c_count = 0;
  }
  else {
    buffer[b_count] = b;
    b_count++;
  }
}

void processBuffer(void) {
  if(buffer[0] == 'T') processTime();
  else if(buffer[0] == 'M') processMode();
  memset(buffer, 0, b_count);
  b_count = 0;
}

void processTime(void) {
    char timeString[50];
    sprintf(timeString, "%02x/%02x/%02x    %02x:%02x:%02x", buffer[7],buffer[6],buffer[5],buffer[3],buffer[2],buffer[1]);
    printTime(timeString);
}

void processMode(void) {
    if (buffer[1] ==  'R') runMode = 1;
    else runMode = 0;
    
    if (buffer[1] == 'S') processStandby();
    else if (buffer[1] == 'I') processInput();
    else if (buffer[1] ==  'R') processRun();
    else if (buffer[1] == 'L') processLogs();
    else if (buffer[1] == 'D') displayOperationInfo(buffer);
}

void processInput(void) {
    if (buffer[2] == 'E') displayEmptyInput();
    else if (buffer[2] == 'D') displayDestinationInput();
    else if (buffer[2] == 'F') displayDietInput();
    else if (buffer[2] == 'Q') displayDietNumInput(buffer[3]);
    else if (buffer[2] == 'P') displayPromptInput();
    else if (buffer[2] == 'S') displayShowInput();
}

void processLogs(void) {
    if (buffer[2] == 'O') displayOperationsComplete();
    else if (buffer[2] == 'P') displayLogsPrompt();
    else if (buffer[2] == 'L') displayLogging();
    else if (buffer[2] == 'D') displayLoggingComplete();
    else if (buffer[2] == 'V') displayViewLogs();
    else if (buffer[2] == 'T') displayTransferringLogs();
}

void processRun(void) {
  if (buffer[2] == 'K') processCounter(buffer[3], buffer[4]);
  else if (buffer[2] == 'A') processArm(buffer[3]); 
  else if (buffer[2] == 'M') processMoving(buffer[3]);
}

