
const int rCountTopPin = 4;
const int rCountBottomPin = 7;
const int lCountTopPin = 6;
const int lCountBottomPin = 9;
const int fCountTopPin = 5;
const int fCountBottomPin = 8;

int rTopCount = 0;
int rBottomCount = 0;
int lTopCount = 0;
int lBottomCount = 0;
int fTopCount = 0;
int fBottomCount = 0;

int rTopSensorState = 0, rTopLastState=0;
int rBottomSensorState = 0, rBottomLastState=0;
int lTopSensorState = 0, lTopLastState=0;
int lBottomSensorState = 0, lBottomLastState=0;
int fTopSensorState = 0, fTopLastState=0;
int fBottomSensorState = 0, fBottomLastState=0;

const int pwm = 5;  //initializing pin 2 as pwm

const int tPin = 14;
const int rPin = 15;
const int lPin = 16;
const int fPin = 17;

int tRun = 1;
int rRun = 1;
int lRun = 1;
int fRun = 1;

int dcRunning;
const int dcBase = 50;
const int dcStep = 10;


int period = 500;

unsigned long rTopTime = 0;
unsigned long rBottomTime = 0;
unsigned long fTopTime = 0;
unsigned long fBottomTime = 0;
unsigned long lTopTime = 0;
unsigned long lBottomTime = 0;

void setup() {

  Serial.begin(9600);

//  pinMode(rCountTopPin, INPUT);    
//  digitalWrite(rCountTopPin, HIGH); // turn on the pullup
//  pinMode(rCountBottomPin, INPUT);    
//  digitalWrite(rCountBottomPin, HIGH); // turn on the pullup
//
//  pinMode(lCountTopPin, INPUT);    
//  digitalWrite(lCountTopPin, HIGH); // turn on the pullup
//  pinMode(lCountBottomPin, INPUT);    
//  digitalWrite(lCountBottomPin, HIGH); // turn on the pullup
//
//  pinMode(fCountTopPin, INPUT);    
//  digitalWrite(fCountTopPin, HIGH); // turn on the pullup
//  pinMode(fCountBottomPin, INPUT);    
//  digitalWrite(fCountBottomPin, HIGH); // turn on the pullup
//  
  pinMode(pwm,OUTPUT);   //we have to set PWM pin as output
  pinMode(tPin,OUTPUT);  //Logic pins are also set as output
  pinMode(rPin,OUTPUT);  //Logic pins are also set as output
  pinMode(lPin,OUTPUT);  //Logic pins are also set as output
  pinMode(fPin,OUTPUT);  //Logic pins are also set as output

  digitalWrite(tPin,HIGH);
  digitalWrite(rPin,HIGH);
  digitalWrite(lPin,HIGH);
  digitalWrite(fPin,HIGH);

  dcRunning = 6;
  analogWrite(pwm, dcBase + dcStep*dcRunning);
}

void loop() {
//  rTopSensorState = digitalRead(rCountTopPin);
//  rBottomSensorState = digitalRead(rCountBottomPin);
//  lTopSensorState = digitalRead(lCountTopPin);
//  lBottomSensorState = digitalRead(lCountBottomPin);
//  fTopSensorState = digitalRead(fCountTopPin);
//  fBottomSensorState = digitalRead(fCountBottomPin);
//  
//  if (!rTopSensorState && rTopLastState) {
//    if(millis() > rTopTime + period){
//        rTopTime = millis();
//        rTopCount++;
//        Serial.print("R Top Count: ");
//        Serial.println(rTopCount);
//    }
//  }
//  rTopLastState = rTopSensorState;
//  
//  if (!lTopSensorState && lTopLastState) {
//    if(millis() > lTopTime + period){
//        lTopTime = millis();
//        lTopCount++;
//        Serial.print("L Top Count: ");
//        Serial.println(lTopCount);
//    }
//  }
//  lTopLastState = lTopSensorState;
//
// 
//  if (!fTopSensorState && fTopLastState) {
//    if(millis() > fTopTime + period){
//        fTopTime = millis();
//        fTopCount++;
//        Serial.print("F Top Count: ");
//        Serial.println(fTopCount);
//    }
//  }
//  fTopLastState = fTopSensorState;
//
//  
//  if (!rBottomSensorState && rBottomLastState) {
//    if(millis() > rBottomTime + period){
//        rBottomTime = millis();
//        rBottomCount++;
//        Serial.print("R Bottom Count: ");
//        Serial.println(rBottomCount);
//    }
//  }
//  rBottomLastState = rBottomSensorState;
//  
//  if (!lBottomSensorState && lBottomLastState) {
//    if(millis() > lBottomTime + period){
//        lBottomTime = millis();
//        lBottomCount++;
//        Serial.print("L Bottom Count: ");
//        Serial.println(lBottomCount);
//    }
//  }
//  lBottomLastState = lBottomSensorState;
//
//  if (!fBottomSensorState && fBottomLastState) {
//    if(millis() > fBottomTime + period){
//        fBottomTime = millis();
//        fBottomCount++;
//        Serial.print("F Bottom Count: ");
//        Serial.println(fBottomCount);
//    }
//  }
//  fBottomLastState = fBottomSensorState;
}

void serialEvent() {
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
  
    if (inChar == 'T') {
      tRun = !tRun;
      digitalWrite(tPin,tRun);
    }
    else if (inChar == 'R') {
      rRun = !rRun;
      digitalWrite(rPin,rRun);
    }
    else if (inChar == 'F') {
      fRun = !fRun;
      digitalWrite(fPin,fRun);
    }
    else if (inChar == 'L') {
      lRun = !lRun;
      digitalWrite(lPin,lRun);
    }
  }
  dcRunning = 3*tRun + rRun + fRun + lRun;
  analogWrite(pwm, dcBase + dcStep*dcRunning);
}
