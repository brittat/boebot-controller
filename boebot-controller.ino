#include <Servo.h>
#include <math.h>

//Pin definitions
const int LEFTIR = 9;
const int RIGHTIR = 4;
const int MIDDLEIR = 7;
const int LEFTIRSENS = 11;
const int RIGHTIRSENS = 5;
const int MIDDLEIRSENS = 8;

const int LOWPINGPIN = 3;
const int HIGHPINGPIN = 2;

const int LEFTMOTOR = 13;
const int RIGHTMOTOR = 12;
const int SONARMOTOR = 10;

//Sensor values
float rightSens = 0;
float leftSens = 0;
float middleSens = 0;
int rightSens2 = 0;
int leftSens2 = 0;

//Low pass filter stuff
const int LPLength = 4;
float leftIRLP[LPLength];
float rightIRLP[LPLength]; 
int lpPointer = 0;

//State machine stuff
int state = 0;
int summedBeaconRead = 0;
int numberOfTurns = 0;
int sumRight = 0;
int sumLeft = 0;
int irSum = 0;
bool isReversing = false;
bool beaconSearch = false;
bool walkingRandomly = false;
bool cylinderFound = false;
bool approachingCylinder = true;
bool timeToScan = false;
const float closeThresh = 0.5;
const float farThresh = 0.9;
long delayStart = 0;
long turnTime = 1000000;
long reverseStart = 0;
long reverseTime = 3000000;
long waitTimeLong = 750000;
long waitTimeShort = 150000;
long waitTime = 0;
long scanInterval = 3000000;
long timeSinceLastScan = 0;
long randomWalkStart = 0;
long randomWalkTime = 5000000;
int turnSpeed = 30;
int moveSpeed = 50;
int randomState = 0;
int beaconState = 0;
int safeState = 0;
int approachState = 0;


//Cylinder retrieval stuff
bool cylinderGrabbed = false;
bool drivingFinished = false;
float targetHeading = 0;
int targetDistance = 0;
long drivingStart = 0;
long drivingTime = 0;
int dangerZone = 100;


//Sonar state machine stuff 
const long initPulseTime = 5000;
const int nDir = 32;
const float degreeAperture = 150.0;
const float aperture = (degreeAperture/180)*M_PI/nDir;
const long servoWaitTime = 100000;
const long sonarWaitTime = 10000;
const long sonarLinger = 100000;

int sonarState = -1;
int currDir = 0;
long sonarDelay = 0;
long tSonar = 0;

bool newLowSonarPulse = false;
bool newHighSonarPulse = false;
volatile unsigned long highPulseStart, highPulseEnd;
volatile unsigned long lowPulseStart, lowPulseEnd;
bool doSonarSweep = false;
bool verifyCylinder = false;

int verifyLow = 0;
int verifyHigh = 0;

int highMillimeters[nDir];
int lowMillimeters[nDir];
int diffMillimeters[nDir];

// Ramp generator stuff
unsigned long lastToneChange = 0;
const long freqStep = 500;
const long baseFreq = 38000;
const long maxFreq = 58000;
const float divider = (maxFreq - baseFreq)/freqStep + 1;
const long rampStepLength = 2000;
const long IRWaitTime = 1000;
long currFreq = baseFreq;

int IRDistLeft = 0;
int IRDistRight = 0;

int tempIRLeft = 0;
int tempIRRight = 0;

const int NUM_IR_DIR = 3;

enum IRDirection {LEFT, MID, RIGHT};
IRDirection currIRDir = LEFT;
enum stateEnum {rWalk, reverse, beaconFind, approachCylinder, beaconExplore};
stateEnum behaviourState = rWalk;

float IRDists[NUM_IR_DIR];
int tempIRDists[NUM_IR_DIR];

bool measuredFlag = false;
bool beaconCheck = false;

long t = micros();

const float turnRate = 40;
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

  attachInterrupt(digitalPinToInterrupt(LOWPINGPIN), lowSonarISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(HIGHPINGPIN), highSonarISR, CHANGE);
  
  //Serial.println("N IR_left IR_mid IR_right");
  //Serial.println("L 0 60");
  //Serial.println("!");

}


void loop() {
 

  long oldT = t;
  t = micros();
  
  sonarStateMachine();
  rampGenerator();
  stateMachine2();  
  drivingTimer();
  
  //Serial.println(targetHeading);
  //Serial.println(targetDistance);
  //Serial.println(state);
  //Serial.println(behaviourState);
  //Serial.println(behaviourState);
  //stateMachine2();
 
  //randomWalk();
  //findBeacon();

 
  
}


