
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
void leftSpeed(float omega)
{
  int servoSignal = 1500 + 0.0086 * omega + 0.0468;
  leftMotor.writeMicroseconds(servoSignal);
}

void rightSpeed(float omega)
{
  int servoSignal = 1500 - (0.0095 * omega + 0.0245);
  rightMotor.writeMicroseconds(servoSignal);
}

void turnAngle(float deltaAngle)
{
  float turnTime = deltaAngle*2*robotRadius/(wheelRadius*turnRate); 
    
  leftSpeed(-turnRate/2);
  rightSpeed(turnRate/2);
  delay(turnTime*1000);
  leftSpeed(0);
  rightSpeed(0);
  
}



int sumIR(int irID)
{
  int reading = 0;
  int nPoints = 10;
  int scanTime = 500; //ms
  for (int i = 0; i < nPoints; i++)
  {
    int ir = digitalRead(irID);
    reading = reading + ir;
    delay(scanTime/nPoints);    
  }
  return reading;
}

//Send out a pulse from the sonar by sending a 5 us pulse on the data line.
//Note that interrupts are temporarily turned off to avoid reading the init pulse.
void sonarPulse()
{
  noInterrupts();
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  interrupts();
}


//Interrupt service routine for the sonar. 
//It is run every time an edge is detected on the sonar data line.
void sonarISR() 
{
  if(digitalRead(pingPin) == HIGH)
  {
    pulseStart = micros();
  } else 
  {
    newSonarPulse = true;
    pulseEnd = micros();
  }
}


//Get distance from sonar timing data
int calcSonarDistance()
{
  return (pulseEnd - pulseStart) * 0.169;
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

int arrayMax(int array[], int l)
{
  int maxVal = 0;
  for(int i = 0; i < l; i++)
  {
    if(array[i] > maxVal)
    {
      maxVal = array[i];
    }
  }
  return maxVal;
}

int arrayMin(int array[], int l)
{
  int minVal = array[0];
  for(int i = 1; i < l; i++)
  {
    if(array[i] < minVal)
    {
      minVal = array[i];
    }
  }
  return minVal;
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
