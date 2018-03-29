#ifndef ACTUATORSHANDLER_H
#define  ACTUATORSHANDLER_H

#include <Servo.h>

/* PIN ASSIGNMENTS */
const int backArmPin = 4;
const int firstArmPin = 10; //topmost
const int secondArmPin = 6;
const int thirdArmPin = 7; 
const int fourthArmPin = 8; //bottommost

const int rCounter = 15;
const int fCounter = 17;
const int lCounter = 16; 
const int topCounters = 14; 
const int pwmCounters = 5;

const int stepperPin = 0;
const int stepperDirectionPin = 1;
const int stepperSleepPin = 9;

Servo backArm;
Servo firstArm; //topmost
Servo secondArm;
Servo thirdArm;
Servo fourthArm; //bottommost2

int backArmLimit = 70;
int armLimit = 130;
int armDelay = 25;

int topRun = 0;
int rRun = 0;
int fRun = 0;
int lRun = 0;

const int dcBase = 50;
const int dcStep = 10;

int currentPos = 0;
int positions[] = {0, 52, 110, 167};

int stepperDelayHigh = 1;
int stepperDelayLow = 30;

int servoBools[] = {0,0,0,0,0,0}; //[0, 1, 2, 3, A, B]

int needsToMove = 0;
int posToMove = -1;

void initActuators(void) {
    backArm.attach(backArmPin);
    firstArm.attach(firstArmPin);
    secondArm.attach(secondArmPin);
    thirdArm.attach(thirdArmPin);
    fourthArm.attach(fourthArmPin);

    pinMode(rCounter, OUTPUT);
    pinMode(fCounter, OUTPUT);
    pinMode(lCounter, OUTPUT);
    pinMode(topCounters, OUTPUT);

    pinMode(pwmCounters,OUTPUT);
    analogWrite(pwmCounters,dcBase+(3*topRun+rRun+fRun+lRun)*dcStep);

    pinMode(stepperPin,OUTPUT); 
    pinMode(stepperDirectionPin,OUTPUT);
    pinMode(stepperSleepPin,OUTPUT); 
    
    digitalWrite(stepperSleepPin,LOW);

    backArm.write(backArmLimit);
    firstArm.write(armLimit);
    secondArm.write(0);
    thirdArm.write(0);
    fourthArm.write(0);
}

void updateGlcd(void) {
  int cartPos = currentPos+1;
  if (needsToMove) cartPos = posToMove+1;
  
  displayRunmodePrompt(topRun, rRun, fRun, lRun, cartPos, needsToMove, servoBools[0], servoBools[1], servoBools[2], servoBools[3], servoBools[5]);
}

void processCounter(char counter, int onoff) {
    if (counter == 'T') {
      topRun = onoff;
      digitalWrite(topCounters, onoff);
    }
    else if (counter == 'R') {
      rRun = onoff;
      digitalWrite(rCounter, onoff);
    }
    else if (counter == 'F') {
      fRun = onoff;
      digitalWrite(fCounter, onoff);
    }
    else if (counter == 'L') {
      lRun = onoff;
      digitalWrite(lCounter, onoff);
    }
   analogWrite(pwmCounters,dcBase+(3*topRun+rRun+fRun+lRun)*dcStep);
   updateGlcd();
}

void runArms(){
  for (int pos = armLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    firstArm.write(armLimit-pos);              // tell servo to go to position in variable 'pos'
    secondArm.write(pos);
    thirdArm.write(pos);
    fourthArm.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
   for (int pos = 0; pos <= armLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    firstArm.write(armLimit-pos);              // tell servo to go to position in variable 'pos'
    secondArm.write(pos);
    thirdArm.write(pos);
    fourthArm.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}


void runArm(Servo arm, int isFlipped){
  for (int pos = 0; pos <= armLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    if (isFlipped) arm.write(armLimit-pos);
    else arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = armLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    if (isFlipped) arm.write(armLimit-pos);
    else arm.write(pos);                 // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}

void runBackArm() {
  for (int pos = 0; pos <= backArmLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    backArm.write(backArmLimit-pos);
    delay(10);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = backArmLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    backArm.write(backArmLimit-pos);                // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
}

void processArm(char inChar) {
  if (inChar == '0') servoBools[0] = 1;
  else if (inChar == '1') servoBools[1] = 1;
  else if (inChar == '2') servoBools[2] = 1;
  else if (inChar == '3') servoBools[3] = 1;
  else if (inChar == 'A') servoBools[4] = 1;
  else if (inChar == 'B') servoBools[5] = 1;
  updateGlcd();
}

void checkServos(void) {
  for (int i = 0; i<6; i++) {
    if  (servoBools[i]) {
      if (i == 0) runArm(firstArm, 1);
      else if (i == 1) runArm(secondArm, 0);
      else if (i == 2) runArm(thirdArm, 0);
      else if (i == 3) runArm(fourthArm, 0);
      else if (i == 4) runArms();
      else if (i == 5) runBackArm();
      servoBools[i] = 0;
      updateGlcd();
    }
  }
}

void processMoving(int pos) {
  needsToMove = 1;
  posToMove = pos;
  updateGlcd();
}

void checkMoving(void) {
  if (needsToMove) {
    digitalWrite(stepperSleepPin,HIGH);
    int posToGo = positions[posToMove];
  
    if (posToGo > currentPos) digitalWrite(stepperDirectionPin,HIGH);
    else if (posToGo < currentPos) digitalWrite(stepperDirectionPin,LOW);
    
    for(int x = 0; x < abs(posToGo-currentPos); x++) {
      digitalWrite(stepperPin,HIGH); 
      delay(stepperDelayHigh); 
      digitalWrite(stepperPin,LOW); 
      delay(stepperDelayLow); 
    }
    digitalWrite(stepperSleepPin,LOW);
    currentPos = posToGo;
    needsToMove = 0;
    posToMove = -1;
    updateGlcd();
  }
}

#endif  /* ACTUATORSHANDLER_H */
