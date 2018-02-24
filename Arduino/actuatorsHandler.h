#ifndef ACTUATORSHANDLER_H
#define  ACTUATORSHANDLER_H

#include <Servo.h>

/* PIN ASSIGNMENTS */
const int backArmPin = 3;
const int firstArmPin = 10; //topmost
const int secondArmPin = 9;
const int thirdArmPin = 6; 
const int fourthArmPin = 5; //bottommost

const int rCounter1 = 14;
const int rCounter2 = 15;
const int fCounter1 = 16;
const int fCounter2 = 17; 
const int lCounter1 = 8; 
const int lCounter2 = 7; 
const int topCounters1 = 0; 
const int topCounters2 = 1;

Servo backArm;
int backArmPos = 0;
Servo firstArm; //topmost
int firstArmPos = 0;
Servo secondArm;
int secondArmPos = 0;
Servo thirdArm;
int thirdArmPos = 0;
Servo fourthArm; //bottommost
int fourthArmPos = 0;

int armLimit = 180; //NEED EXPERIMENTATION, NUMBER OF DEGREED SERVO NEEDS TO GO TO OPEN/CLOSE DRAWERS

void initActuators(void) {
    backArm.attach(backArmPin);
    firstArm.attach(firstArmPin);
    secondArm.attach(secondArmPin);
    thirdArm.attach(thirdArmPin);
    fourthArm.attach(fourthArmPin);
}

void setCounter(char counter, int onoff) {
    if (counter == 'T') {
      //turn on all top counters
    }
    else if (counter == 'R') {
      
    }
    else if (counter == 'F') {
      
    }
    else if (counter == 'L') {
      
    }
}

void processCounter(char counter, char state){
    if (state == 'N') setCounter(counter, 1);
    else if (state == 'F') setCounter(counter, 0);
}

void runArm(Servo arm, int pos){
   for (pos = 0; pos <= armLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = armLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    arm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void processArm(char arm) {
     if (arm == '0') runArm(backArm, backArmPos);
     else if (arm == '1') runArm(firstArm, firstArmPos);
     else if (arm == '2') runArm(secondArm, secondArmPos);
     else if (arm == '3') runArm(thirdArm, thirdArmPos);
     else if (arm == '4') runArm(fourthArm, fourthArmPos);
}

#endif  /* ACTUATORSHANDLER_H */
