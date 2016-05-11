#include <Servo.h>
#include <math.h>

//Pin definitions
const int LEFTIR = 9;
const int RIGHTIR = 1;
const int MIDDLEIR = 7;
const int LEFTIRSENS = 11;
const int RIGHTIRSENS = 4;
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
long waitTime = 3000;
int numberOfTurns = 0;
int sumRight = 0;
int sumLeft = 0;
int irSum = 0;
boolean isReversing = false;
boolean beaconSearch = false;
boolean randomWalk = false;


//Sonar state machine stuff 


const long initPulseTime = 5000;
const int nDir = 32;
const long servoWaitTime = 100000;
const long sonarWaitTime = 10000;
const long sonarLinger = 100000;


int sonarState = 0;
int currDir = 0;
long sonarDelay = 0;
long tSonar = 0;

bool newLowSonarPulse = false;
bool newHighSonarPulse = false;
volatile unsigned long highPulseStart, highPulseEnd;
volatile unsigned long lowPulseStart, lowPulseEnd;
bool doSonarSweep = false;


int highMillimeters[nDir];
int lowMillimeters[nDir];
int diffMillimeters[nDir];

// Ramp generator stuff
unsigned long lastToneChange = 0;
const long freqStep = 500;
const long baseFreq = 38000;
const long maxFreq = 58000;
const float divider = (maxFreq - baseFreq + 1)/freqStep;
const long rampStepLength = 4000;
const long IRWaitTime = 1000;
long currFreq = baseFreq;

int IRDistLeft = 0;
int IRDistRight = 0;

int tempIRLeft = 0;
int tempIRRight = 0;

const int NUM_IR_DIR = 3;

enum IRDirection {LEFT, MID, RIGHT};
IRDirection currIRDir = LEFT;

float IRDists[NUM_IR_DIR];
int tempIRDists[NUM_IR_DIR];

bool measuredFlag = false;

long t = micros();

const float turnRate = 40;
const int robotRadius = 51;
const int wheelRadius = 33;

float targetHeading = 0;
int targetDistance = 0;

long drivingTime = 0;
long drivingStart = 0;

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
  
  Serial.println("N IR_left IR_mid IR_right");
  Serial.println("L 0 60");
  Serial.println("!");

}

void loop() {
  long oldT = t;
  t = micros();


  
  //doSonarSweep = true;
  //sonarStateMachine();
  rampGenerator();
  drivingTimer();
}


