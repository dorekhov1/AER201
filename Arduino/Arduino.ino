#include <Wire.h>
#include <string.h>
#include "glcdHandler.h"
#include "actuatorsHandler.h"

unsigned char buffer[200];
uint8_t b_count = 0;
uint8_t c_count = 0;

boolean status = 0;

void setup(){
  
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register a function to be called when this slave device receives a transmission from a master
  Wire.onRequest(requestEvent); // Register a function to be called when a master requests data from this slave device

  initGlcd();
  initActuators();
}

void loop(){
  if (c_count == 3) {
    status = 0;
    processBuffer();
    c_count = 0;
  }
}

void receiveEvent(void){
  char b = Wire.read();
  
  if (b == 'C') c_count++;
  else c_count = 0;

  buffer[b_count] = b;
  b_count++;
}

void requestEvent(void){
  Wire.write(status);
  status = 0;
}

void processBuffer(void) {
  if(buffer[0] == 'T') processTime(buffer);
  else if(buffer[0] == 'M') processMode();
  else if(buffer[0] == 'K') processCount(buffer[1], buffer[2], buffer[3]); 
  else if(buffer[0] == 'D') {
    displayOperationInfo(buffer);
    reset();
  }
  memset(buffer, 0, b_count);
  b_count = 0;
  status = 1;
}

void processMode(void) {
    if (buffer[1] == 'S') processStandby();
    else if (buffer[1] == 'I') processInput();
    else if (buffer[1] ==  'R') processRun();
    else if (buffer[1] == 'L') processLogs();
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
    else if (buffer[2] == 'V') displayTransferLogs();
    else if (buffer[2] == 'T') displayTransferringLogs();
    else if (buffer[2] == 'Q') displayViewLogs();
}

void processRun(void) {
  if (buffer[2] == 'K') processCounter(buffer[3], buffer[4]-'0');
  else if (buffer[2] == 'A') processArm(buffer[3]-'0'); 
  else if (buffer[2] == 'M') processMoving(buffer[3] - '0');
}

