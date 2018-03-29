#include <Wire.h>
#include <string.h>
#include "glcdHandler.h"
#include "actuatorsHandler.h"

void setup(){
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register a function to be called when this slave device receives a transmission from a master
  Wire.onRequest(requestEvent); // Register a function to be called when a master requests data from this slave device

  initGlcd();
  initActuators();
    Serial.begin(9600);
}

void loop(){
  checkServos();
  checkMoving();
}

char buffer[9];
uint8_t b_count = 0;

void receiveEvent(void){
  char b = Wire.read();

  if (b == 'C') processBuffer();
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
    else if (buffer[2] == 'V') displayViewLogs();
    else if (buffer[2] == 'T') displayTransferringLogs();
}

void processRun(void) {
  if (buffer[2] == 'K') processCounter(buffer[3], buffer[4]);
  else if (buffer[2] == 'A') processArm(buffer[3]); 
  else if (buffer[2] == 'M') processMoving(buffer[3]);
}

void requestEvent(void){
   /* This function is called whenver a master requests data.
   * 
   * Arguments: none
   * 
   * Returns: none
   */
   
  //Wire.write(incomingByte); // Respond with message of 1 byte
  //incomingByte = 0; // Clear output buffer
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
  
    if (inChar == '1') runArm(firstArm, 1);
    else if (inChar == '2') runArm(secondArm, 0);
    else if (inChar == '3') runArm(thirdArm, 0);
    else if (inChar == '4') runArm(fourthArm, 0);
    else if (inChar == 'A') runArms();
    else if (inChar == 'B') runBackArm();
    else if (inChar == 'O') processMoving(0);
    else if (inChar == 'T') processMoving(1);
    else if (inChar == 'H') processMoving(2);
    else if (inChar == 'F') processMoving(3);
  }
}
