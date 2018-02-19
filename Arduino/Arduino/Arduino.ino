
#include <Wire.h>
#include <string.h>
#include "glcdHandler.h"

void setup(){
  Wire.begin(8); // Join I2C bus with address 8
  Wire.onReceive(receiveEvent); // Register a function to be called when this slave device receives a transmission from a master
  Wire.onRequest(requestEvent); // Register a function to be called when a master requests data from this slave device

  Serial.begin(9600); // Open serial port to PC (hardware UART)     
  initGlcd();
}

void loop(){

}

char buffer[10];
uint8_t b_count = 0;
void receiveEvent(void){
  /* This function is called whenver a transmission is received from a master.
   * 
   * Arguments: none
   * 
   * Returns: none
   */

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
    if (buffer[1] == 'I') processInput();
}

void processInput(void) {
    if (buffer[2] == 'E') displayEmptyInput();
    else if (buffer[2] == 'D') displayDestinationInput();
    else if (buffer[2] == 'F') displayDietInput();
    else if (buffer[2] == 'Q') displayDietNumInput(buffer[3]);
    else if (buffer[2] == 'P') displayPromptInput();
    else if (buffer[2] == 'S') displayShowInput();
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
