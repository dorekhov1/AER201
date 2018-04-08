#ifndef ACTUATORSHANDLER_H
#define  ACTUATORSHANDLER_H

#include <Servo.h>

Servo backArm;
Servo firstArm; //topmost
Servo secondArm;
Servo thirdArm;
Servo fourthArm; //bottommost2

const uint8_t rCounter = 15;
const uint8_t fCounter = 17;
const uint8_t lCounter = 16; 
const uint8_t topCounters = 14; 
const uint8_t pwmCounters = 5;

const uint8_t stepperPin = 0;
const uint8_t stepperDirectionPin = 1;
const uint8_t stepperSleepPin = 9;

const uint8_t backArmLimit = 100;
const uint8_t armLimit = 150;
const uint8_t armDelay = 25;

boolean topRun = 0;
boolean rRun = 0;
boolean fRun = 0;
boolean lRun = 0;

const uint8_t dcBase = 50;
const uint8_t dcStep = 10;

uint8_t currentPos = 0;
//const uint8_t positions[] = {0, 0, 50, 65, 110, 130, 172, 190};
const uint8_t positions[] = {0, 0, 65, 65, 130, 130, 172, 190};

const uint8_t stepperDelayHigh = 1;
const uint8_t stepperDelayLow = 30;
  
boolean servoBools[] = {0,0,0,0,0}; //[0, 1, 2, 3, B]
boolean needsToMove = 0;
uint8_t posToMove = -1;

short int r = 0;
short int f = 0;
short int l = 0;

void initActuators(void) {
  
    backArm.attach(4);
    firstArm.attach(10);
    secondArm.attach(6);
    thirdArm.attach(7);
    fourthArm.attach(8);

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

void reset(void) {
  r = 0;
  f = 0;
  l = 0;
}

void updateGlcd(void) {
  uint8_t cartPos = currentPos+1;
  if (needsToMove) cartPos = posToMove+1;
  int armRun = -1;
  for (int i=0; i<5; i++) {
    if (servoBools[i]) armRun = i;
  }
  displayRunmodePrompt(topRun, rRun, fRun, lRun, cartPos, needsToMove, armRun, r, f, l);
}

void processCount(short int rCount, short int fCount, short int lCount) {
  r = rCount;
  f = fCount;
  l = lCount;
  updateGlcd();
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
 if (!topRun && !rRun && !fRun && !lRun) analogWrite(pwmCounters,0);
 else analogWrite(pwmCounters,dcBase+(3*topRun+rRun+fRun+lRun)*dcStep);
 
 updateGlcd();
}

void runArm(Servo arm, boolean isFlipped, uint8_t limit){
  for (short int pos = 0; pos <= limit; pos += 1) { // goes from 0 degrees to 180 degrees
    if (isFlipped) arm.write(limit-pos);
    else arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (short int pos = limit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    if (isFlipped) arm.write(limit-pos);
    else arm.write(pos);                 // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void processArm(int i) {
  servoBools[i] = 1;
  updateGlcd();
  if (i == 0) runArm(firstArm, 1, armLimit);
  else if (i == 1) runArm(secondArm, 0, armLimit);
  else if (i == 2) runArm(thirdArm, 0, armLimit);
  else if (i == 3) runArm(fourthArm, 0, armLimit);
  else if (i == 4) runArm(backArm, 1, backArmLimit);
  servoBools[i] = 0;
  updateGlcd();
}

void processMoving(uint8_t pos) {
  needsToMove = 1;
  posToMove = pos;
  updateGlcd();
  
  digitalWrite(stepperSleepPin, HIGH);
  int posToGo = positions[posToMove];

  digitalWrite(stepperDirectionPin,posToGo > positions[currentPos]);

  for(int x = 0; x < abs(posToGo-positions[currentPos]); x++) {
    digitalWrite(stepperPin,HIGH); 
    delay(stepperDelayHigh); 
    digitalWrite(stepperPin,LOW); 
    delay(stepperDelayLow); 
  }
  digitalWrite(stepperSleepPin,LOW);
  currentPos = posToMove;
  needsToMove = 0;
  posToMove = -1;
  updateGlcd();
}

#endif  /* ACTUATORSHANDLER_H */
