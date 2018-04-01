// defines pins numbers
const int stepPin = 10; 
const int dirPin = 9; 
const int sleepPin = 3;

int currentPos = 0;
  int positions[] = {0, 52, 110, 167};

int stepperDelayHigh = 1;
int stepperDelayLow = 30;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT); 
  pinMode(sleepPin,OUTPUT); 
  digitalWrite(sleepPin,LOW);
  
  Serial.begin(9600);
}
void loop() {
}

void moveCartPosition(int pos) {
  digitalWrite(sleepPin,HIGH);
  int posToGo = positions[pos];

  if (posToGo > currentPos) digitalWrite(dirPin,HIGH); //Forward
  else if (posToGo < currentPos) digitalWrite(dirPin,LOW); //Backward
  
  for(int x = 0; x < abs(posToGo-currentPos); x++) {
    digitalWrite(stepPin,HIGH); 
    delay(stepperDelayHigh); 
    digitalWrite(stepPin,LOW); 
    delay(stepperDelayLow); 
  }
  currentPos = posToGo;
  digitalWrite(sleepPin,LOW);
}


void moveCart(int steps){
  digitalWrite(sleepPin,HIGH);
  for(int x = 0; x < steps; x++) {
    digitalWrite(stepPin,HIGH);
    delay(stepperDelayHigh);
    digitalWrite(stepPin,LOW);
    delay(stepperDelayLow);
  }
  digitalWrite(sleepPin,LOW);
}

void serialEvent() {
  
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    if (inChar == 'M') moveCart(52);
    else if (inChar == 'H') digitalWrite(dirPin,HIGH);
    else if (inChar == 'L') digitalWrite(dirPin,LOW);
    else if (inChar == '1') moveCartPosition(0);
    else if (inChar == '2') moveCartPosition(1);
    else if (inChar == '3') moveCartPosition(2);
    else if (inChar == '4') moveCartPosition(3);
  }
}
