#include <Servo.h>
 
/* PIN ASSIGNMENTS */
const int stepPin = 10; 
const int dirPin = 9; 

const int backArmPin = 4;
const int firstArmPin = 12; //topmost
const int secondArmPin = 6;
const int thirdArmPin = 7; 
const int fourthArmPin = 8;

int backArmRunning = 0;

Servo firstArm;
Servo secondArm;
Servo thirdArm;
Servo fourthArm;
Servo backArm;

int armLimit = 126;

int backArmUpperLimit = 70;
int backArmLowerLimit = 0;

int currentPos = 0;
int positions[] = {0, 130, 250, 371};

int stepperDelay = 1000;

void setup() {
    Serial.begin(9600);
    
    firstArm.attach(firstArmPin);
    secondArm.attach(secondArmPin);
    thirdArm.attach(thirdArmPin);
    fourthArm.attach(fourthArmPin);
    backArm.attach(backArmPin);
    
    backArm.write(backArmUpperLimit);

    pinMode(stepPin,OUTPUT); 
    pinMode(dirPin,OUTPUT); 

    digitalWrite(dirPin, LOW);
    digitalWrite(stepPin, LOW);   
    
    firstArm.write(armLimit);
    secondArm.write(0);
    thirdArm.write(0);
    fourthArm.write(0);
}

void loop() {
}

void runArms(){
   for (int pos = 0; pos <= armLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    firstArm.write(armLimit-pos);              // tell servo to go to position in variable 'pos'
    secondArm.write(pos);
    thirdArm.write(pos);
    fourthArm.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (int pos = armLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    firstArm.write(armLimit-pos);              // tell servo to go to position in variable 'pos'
    secondArm.write(pos);
    thirdArm.write(pos);
    fourthArm.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void runBackArm(int forward) {
  for (int pos = backArmLowerLimit; pos <= backArmUpperLimit; pos += 1) { // goes from 0 degrees to 180 degrees
    if (forward) backArm.write(backArmLowerLimit+backArmUpperLimit-pos);
    else backArm.write(pos);// tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15ms for the servo to reach the position
  }
//  for (int pos = backArmLimit; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    backArm.write(backArmLimit-pos);                // tell servo to go to position in variable 'pos'
//    delay(10);                       // waits 15ms for the servo to reach the position
//  }
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

void moveCart(int pos) {
  int posToGo = positions[pos];

  if (posToGo > currentPos) digitalWrite(dirPin,LOW); //Forward
  else if (posToGo < currentPos) digitalWrite(dirPin,HIGH); //Backward
  
  for(int x = 0; x < abs(posToGo-currentPos); x++) {
    digitalWrite(stepPin,HIGH); 
    delayMicroseconds(stepperDelay); 
    digitalWrite(stepPin,LOW); 
    delayMicroseconds(stepperDelay); 
  }
  currentPos = posToGo;
}

void serialEvent() {
  
  while (Serial.available()) {
    char inChar = (char)Serial.read();
  
    if (inChar == '1') runArm(firstArm, 1);
    else if (inChar == '2') runArm(secondArm, 0);
    else if (inChar == '3') runArm(thirdArm, 0);
    else if (inChar == '4') runArm(fourthArm, 0);
    else if (inChar == 'A') runArms();
    else if (inChar == 'F') runBackArm(1);
    else if (inChar == 'B') runBackArm(0);
    
    else if (inChar == 'O') moveCart(0);
    else if (inChar == 'T') moveCart(1);
    else if (inChar == 'H') moveCart(2);
    else if (inChar == 'U') moveCart(3);
  }
}
