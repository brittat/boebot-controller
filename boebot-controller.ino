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
unsigned long startDelay = 0;
unsigned long tSonar = 0;

int centimeters[nDir];

// Ramp generator stuff
unsigned long lastToneChange = 0;
const long freqStep = 500;
const long baseFreq = 38000;
const long maxFreq = 44000;
const float divider = (maxFreq - baseFreq)/freqStep;
const long rampStepLength = 4;
const long IRWaitTime = 1;
long currFreq = baseFreq;
enum IRDirection {LEFT, RIGHT, STOPLEFT, STOPRIGHT};
IRDirection currIRDir = STOPLEFT;

int IRDistLeft = 0;
int IRDistRight = 0;

int tempIRLeft = 0;
int tempIRRight = 0;

bool measuredFlag = false;




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

  rightSens = getDistanceIR(RIGHTIR, RIGHTIRSENS);
  leftSens = getDistanceIR(LEFTIR, LEFTIRSENS);

  leftIRLP[lpPointer%LPLength] = leftSens;
  rightIRLP[lpPointer%LPLength] = rightSens;
  lpPointer++;

  rightSens = arraySum(rightIRLP, LPLength)/(1.0*LPLength);
  leftSens = arraySum(leftIRLP, LPLength)/(1.0*LPLength);
  
  stateMachine();
  sonarStateMachine();
  rampGenerator();

//  Serial.print(state);
//  Serial.print(" ");
//  Serial.print(leftSens);
//  Serial.print(" ");
//  Serial.println(rightSens);

  Serial.print(sonarState);
  Serial.print(" ");
  Serial.print(IRDistLeft);
  Serial.print(" ");
  Serial.println(IRDistRight);

  
}


