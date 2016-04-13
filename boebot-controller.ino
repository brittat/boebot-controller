#include <Servo.h>

//Jag har skrivit

//Pin definitions
const int LEFTIR = 9;
const int RIGHTIR = 2;
const int LEFTIRSENS = 10;
const int RIGHTIRSENS = 3;

const int LEFTMOTOR = 13;
const int RIGHTMOTOR = 12;
const int SONARMOTOR = 10;

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


//Sonar state machine stuff 

const long initPulseTime = 5;
const int nDir = 8;
const long servoWaitTime = 1000000;
const int pingPin = 11;

int sonarState = 0;
int currDir = 0;
long startDelay = 0;
long tSonar = 0;

int centimeters[nDir];

Servo leftMotor;
Servo rightMotor;
Servo sonarMotor;

void setup() {
  
  Serial.begin(115200);
  pinMode(LEFTIR, OUTPUT);
  pinMode(RIGHTIR, OUTPUT);
  pinMode(LEFTIRSENS, INPUT);
  pinMode(RIGHTIRSENS, INPUT);

  leftMotor.attach(LEFTMOTOR);
  rightMotor.attach(RIGHTMOTOR);
  sonarMotor.attach(SONARMOTOR);
}

void loop() {

  rightSens = 0;//getDistanceIR(RIGHTIR, RIGHTIRSENS);
  leftSens = 0;//getDistanceIR(LEFTIR, LEFTIRSENS);

  leftIRLP[lpPointer%LPLength] = leftSens;
  rightIRLP[lpPointer%LPLength] = rightSens;
  lpPointer++;

  rightSens = arraySum(rightIRLP, LPLength)/(1.0*LPLength);
  leftSens = arraySum(leftIRLP, LPLength)/(1.0*LPLength);


//  Serial.print(state);
//  Serial.print(" ");
//  Serial.print(leftSens);
//  Serial.print(" ");
//  Serial.println(rightSens);

  Serial.print(sonarState);
  Serial.print(" ");
  Serial.print(centimeters[0]);
  Serial.print(" ");
  Serial.println(centimeters[1]);

  stateMachine();
  sonarStateMachine();
  
}


