
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
void leftSpeed(int speed)
{
  //leftMotor.writeMicroseconds(1500+speed);
}

void rightSpeed(int speed)
{
  //rightMotor.writeMicroseconds(1500-speed);
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

