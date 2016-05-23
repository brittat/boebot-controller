// Needs some work still, we can use getDistanceIR for now
const int IR_PINS[] = {LEFTIR, MIDDLEIR, RIGHTIR};
//LEFTIR = 9;
//RIGHTIR = 4;
//MIDDLEIR = 7;
const int IR_SENS_PINS[] = {LEFTIRSENS, MIDDLEIRSENS, RIGHTIRSENS};
void rampGenerator()
{
  t = micros();
  
  if(t%1024 == 0) //Data printing every once in a while for debug
  {
    Serial.print("D ");
    Serial.print(t/1000);
    Serial.print(" ");
    Serial.print(IRDists[0]);
    Serial.print(" ");
    Serial.print(IRDists[1]);
    Serial.print(" ");
    Serial.println(IRDists[2]);
  }
  
  //Turn off the LED if we're looking for the beacon.
  if(!beaconCheck) 
  {
    tone(IR_PINS[currIRDir], currFreq);
  }
  else
  {
    noTone(RIGHTIR);
    noTone(MIDDLEIR);
    noTone(LEFTIR);
  }
  
  // Every rampStepLength microsecs change the frequency 
  if(t-lastToneChange >= rampStepLength) 
  {
    measuredFlag = false;
    lastToneChange = t;
    currFreq += freqStep;

    if(currFreq > maxFreq)// If we're done with the ramp, do a check for the beacon.
    {      
      currFreq = 0;
      beaconCheck = true;     
      
      noTone(RIGHTIR);
      noTone(MIDDLEIR);
      noTone(LEFTIR);
    }
  }

  //Check if the reading persists even when the LED is off.
  //If so, set the reading to -1 to indicate the beacon
  if(t-lastToneChange >= IRWaitTime && beaconCheck)
  {
    if(!digitalRead(IR_SENS_PINS[currIRDir]))
    {
      tempIRDists[currIRDir] = -divider; //This value is divided by divider later. Not pretty.
    }
    lastToneChange = t;
    currFreq = baseFreq;
    beaconCheck = false;
    IRDists[currIRDir] = tempIRDists[currIRDir]/divider;
    tempIRDists[currIRDir] = 0;
    currIRDir = (IRDirection)((currIRDir + 1) % NUM_IR_DIR);
  }
  
  
  //Here's where the measurements are taken if nothing else is going on.
  if(t-lastToneChange >= IRWaitTime && !beaconCheck && !measuredFlag && digitalRead(IR_SENS_PINS[currIRDir]))
  {
     measuredFlag = true;
     tempIRDists[currIRDir]++;
     
  }
}
