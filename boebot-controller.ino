#include <Servo.h>


//Pin definitions
const int LEFTIR = 10;
const int RIGHTIR = 2;
const int MIDDLEIR =7;
const int LEFTIRSENS = 11;
const int RIGHTIRSENS = 4;
const int MIDDLEIRSENS = 8;

const int PINGPIN = 3;

const int LEFTMOTOR = 13;
const int RIGHTMOTOR = 12;
const int SONARMOTOR = 10;

//Sensor values
float rightSens = 0;
float leftSens = 0;
float middleSens = 0;

//Low pass filter stuff
const int LPLength = 4;
float leftIRLP[LPLength];
float rightIRLP[LPLength]; 
int lpPointer = 0;

//State machine stuff
int state = 0;
long waitTime = 0;
int numberOfTurns = 0;
int sumRight = 0;
int sumLeft = 0;
int irSum = 0; //Robustness for finding safe zone
boolean isReversing = false;


//Sonar state machine stuff 

const long initPulseTime = 5000;
const int nDir = 32;
const long servoWaitTime = 100000;
const long sonarLinger = 100000;


int sonarState = 0;
int currDir = 0;
unsigned long startDelay = 0;
unsigned long tSonar = 0;

bool newSonarPulse = false;
volatile unsigned long pulseStart, pulseEnd;
bool doSonarSweep = false;


int millimeters[nDir];

// Ramp generator stuff
unsigned long lastToneChange = 0;
const long freqStep = 500;
const long baseFreq = 38000;
const long maxFreq = 44000;
const float divider = (maxFreq - baseFreq)/freqStep;
const long rampStepLength = 4000;
const long IRWaitTime = 1000;
long currFreq = baseFreq;
enum IRDirection {LEFT, RIGHT, STOPLEFT, STOPRIGHT};
IRDirection currIRDir = STOPLEFT;

int IRDistLeft = 0;
int IRDistRight = 0;

int tempIRLeft = 0;
int tempIRRight = 0;

bool measuredFlag = false;

long t = micros();

const float turnRate = 2;
const int robotRadius = 51;
const int wheelRadius = 33;

Servo leftMotor;
Servo rightMotor;
Servo sonarMotor;

void setup() {
  
  Serial.begin(115200);
  pinMode(LEFTIR, OUTPUT);
  pinMode(RIGHTIR, OUTPUT);
  pinMode(MIDDLEIR, OUTPUT);
  pinMode(LEFTIRSENS, INPUT);
  pinMode(RIGHTIRSENS, INPUT);
  pinMode(MIDDLEIRSENS, INPUT);

  leftMotor.attach(LEFTMOTOR);
  rightMotor.attach(RIGHTMOTOR);
  sonarMotor.attach(SONARMOTOR);

 /* attachInterrupt(digitalPinToInterrupt(PINGPIN), sonarISR, CHANGE);
  
  Serial.println("N IR_left IR_right");
  Serial.println("L 0 60");
  Serial.println("!");
  */
}

void loop() {


  long oldT = t;
  t = micros();

//  Serial.print("dt = ");
//  Serial.println(t-oldT);
//
//  Serial.print(" sonarState = ");
  

  //leftSpeed(50);
  stateMachine();
  //sonarStateMachine();
  //rampGenerator();

  int ir = getIrRead(MIDDLEIR,MIDDLEIRSENS);
  Serial.println(ir);
  delay(100);
  //turnAngle(1.57);
  //delay(1000);
  //turnAngle(-1.57);
  //delay(1000);

  
}


