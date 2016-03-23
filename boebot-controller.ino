#include <Servo.h>

//Pin definitions
const int LEFTIR = 9;
const int RIGHTIR = 2;
const int LEFTIRSENS = 10;
const int RIGHTIRSENS = 3;

const int LEFTMOTOR = 13;
const int RIGHTMOTOR = 12;

//Sensor values
float rightSens = 0;
float leftSens = 0;

//Low pass filter stuff
const int LPLength = 4;
float leftIRLP[LPLength];
float rightIRLP[LPLength]; 
int lpPointer = 0;

//State machine stuff
int state = 0;

Servo leftMotor;
Servo rightMotor;

void setup() {
  
  Serial.begin(115200);
  pinMode(LEFTIR, OUTPUT);
  pinMode(RIGHTIR, OUTPUT);
  pinMode(LEFTIRSENS, INPUT);
  pinMode(RIGHTIRSENS, INPUT);

  leftMotor.attach(LEFTMOTOR);
  rightMotor.attach(RIGHTMOTOR);
}

void loop() {

  rightSens = getDistanceIR(RIGHTIR, RIGHTIRSENS);
  leftSens = getDistanceIR(LEFTIR, LEFTIRSENS);

  leftIRLP[lpPointer%LPLength] = leftSens;
  rightIRLP[lpPointer%LPLength] = rightSens;
  lpPointer++;

  rightSens = arraySum(rightIRLP, LPLength)/(1.0*LPLength);
  leftSens = arraySum(leftIRLP, LPLength)/(1.0*LPLength);
  
  Serial.print(leftSens);
  Serial.print(" ");
  Serial.println(rightSens);

  stateMachine();

  
}


