
//    Performs a frequency sweep in order to get a
//    rough idea of distance using IR LEDs. 
//      Returns float in [0, 1]
float getDistanceIR(int outPin, int sensPin)
{
  int dist = 0;
  const long freqStep = 500;
  const long baseFreq = 38000;
  const long maxFreq = 44000;
  const float divider = (maxFreq - baseFreq)/freqStep;
  
  for(long f = baseFreq; f < maxFreq; f = f + freqStep)
  {
    tone(outPin, f, 4);
    delay(1);
    if(digitalRead(sensPin))
    {
      dist = dist+1;
    }
    delay(1);
  }
  return dist/divider ;
}

// Wrappers for servo motors
void leftSpeed(int spd)
{
  leftMotor.writeMicroseconds(1498 + spd);
}

void rightSpeed(int spd)
{
  rightMotor.writeMicroseconds(1499 - spd);
}

void turn(float deltaAngle)
{
  Serial.println("time to turn");
  int angleSign =  sign(deltaAngle);
  deltaAngle = fmod(deltaAngle, 2 * M_PI);
  long turnTime = angleSign*deltaAngle*1000000;
  Serial.println(turnTime/1000000.0);
  //if(angleSign > 0)
  //{
    turnTime = turnTime*0.64;
  //}else{
    //turnTime = turnTime*0.64;
  //}
  turnSetTime(turnTime, angleSign);
}

void turnSetTime(long turnTime1, int angleSign)
{
  long turnStart = micros();
  Serial.println(turnTime1);
  while(micros() - turnStart < turnTime1)
  {
    leftSpeed(-angleSign*turnRate);
    rightSpeed(angleSign*turnRate);
  }

  leftSpeed(0);
  rightSpeed(0);
  
}

void driveDistance(int distance)
{
  leftSpeed(50);
  rightSpeed(50);
  drivingStart = t;
  drivingTime = (long)distance * 1000 * 10.7;
  Serial.println(drivingTime);
  drivingFinished = false;
}

int getIrRead(int diodID, int sensorID)
{
  tone(diodID, 38000, 8);                
  delay(1);                         // Wait 1 ms
  int ir = digitalRead(sensorID); 
  noTone(diodID);
  delay(10);
  return ir;
}

int getIrReadFloor(int diodID, int sensorID)
{
  tone(diodID, 15000, 8);                // Frequency to be tested in arena,18000
  delay(1);                         // Wait 1 ms
  int ir = digitalRead(sensorID);         // IR receiver -> ir variable
  noTone(diodID);
  delay(10); //To avoid overload
  return ir;
}

int sumIR(int irID)
{
  int reading = 0;
  int nPoints = 100;
  int scanTime = 500; //ms
  for (int i = 0; i < nPoints; i++)
  {
    int ir = digitalRead(irID);
    reading = reading + ir;
    delay(scanTime/nPoints);    
  }
  return reading;
}
//Check the left and right sensors for beacon signal. 
//Returns 0 for no read, 1 left read, 2 right read and 3 for both
int beaconRead()
{
  int sumLeft = 0;
  int sumRight = 0;
  int nPoints = 100;
  int scanTime = 1000; //ms
  for (int i = 0; i < nPoints; i++)
  {
    int irLeft = digitalRead(LEFTIRSENS);
    int irRight = digitalRead(RIGHTIRSENS);
    sumLeft = sumLeft + irLeft;
    sumRight = sumRight+ irRight;
    delay(scanTime/nPoints);    
  }
  if (sumLeft == nPoints && sumRight == nPoints){
    return 0;
  }else if (sumLeft < nPoints && sumRight == nPoints){
    return 1;
  }else if (sumLeft == nPoints && sumRight < nPoints){
    return 2;
  }else{
    return 3;
  }
}
//Send out a pulse from the sonar by sending a 5 us pulse on the data line.
//Note that interrupts are temporarily turned off to avoid reading the init pulse.
void lowSonarPulse()
{
  noInterrupts();
  pinMode(LOWPINGPIN, OUTPUT);
  digitalWrite(LOWPINGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(LOWPINGPIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(LOWPINGPIN, LOW);
  pinMode(LOWPINGPIN, INPUT);
  interrupts();
}

void highSonarPulse()
{
  noInterrupts();
  pinMode(HIGHPINGPIN, OUTPUT);
  digitalWrite(HIGHPINGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(HIGHPINGPIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(HIGHPINGPIN, LOW);
  pinMode(HIGHPINGPIN, INPUT);
  interrupts();
}


//Interrupt service routine for the sonar. 
//It is run every time an edge is detected on the sonar data line.
void lowSonarISR() 
{
  if(digitalRead(LOWPINGPIN) == HIGH)
  {
    lowPulseStart = micros();
  } else 
  {
    newLowSonarPulse = true;
    lowPulseEnd = micros();
  }
}

void highSonarISR() 
{
  if(digitalRead(HIGHPINGPIN) == HIGH)
  {
    highPulseStart = micros();
  } else 
  {
    newHighSonarPulse = true;
    highPulseEnd = micros();
  }
}


//Get distance from sonar timing data
int calcLowSonarDistance()
{
  return (lowPulseEnd - lowPulseStart) * 0.169;
}

int calcHighSonarDistance()
{
  return (highPulseEnd - highPulseStart) * 0.169;
}


//Various array functions. 
//C does not like to pass arrays, so the length has to be passed as well.
void serialPrintArray(int array[], int l)
{
  for(int i = 0; i < l; i++)
  {
    Serial.print(array[i]);
    Serial.print(' ');
  }
  Serial.println();  
}

void serialPrintFloatArray(float array[], int l)
{
  for(int i = 0; i < l; i++)
  {
    Serial.print(array[i]);
    Serial.print(' ');
  }
  Serial.println();  
}

int arrayMax(int list[], int l)
{
  int maxVal = 0;
  for(int i = 0; i < l; i++)
  {
    if(list[i] > maxVal)
    {
      maxVal = list[i];
    }
  }
  return maxVal;
}

int arrayMin(int list[], int l)
{
  int minVal = list[0];
  for(int i = 1; i < l; i++)
  {
    if(list[i] < minVal)
    {
      minVal = list[i];
    }
  }
  return minVal;
}


float arrayMaxFloat(float list[], int l)
{
  float maxVal = 0;

  for(int i = 0; i < l; i++)
  {
    if(list[i] > maxVal)
    {
      maxVal = list[i];
    }
  }
  return maxVal;
}

float arrayMinFloat(float list[], int l)
{
  float minVal = list[0];

  for(int i = 1; i < l; i++)
  {
    if(list[i] < minVal)
    {
      minVal = list[i];
    }
  }
  return minVal;
}



int sign(float checkSignFloat) 
{
  return (checkSignFloat > 0) - (checkSignFloat < 0);
}
// Takes sum of array, used for low pass filter
float arraySum(float array[], int l)
{
  float ret = 0;
  for(int i = 0; i < l; i++)
  {
    //Serial.println(array[i]);
    ret += array[i];
  }
  return ret;
}
