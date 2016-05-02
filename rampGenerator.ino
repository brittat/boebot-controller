// Needs some work still, we can use getDistanceIR for now
const int IR_PINS[] = {LEFTIR, MIDDLEIR, RIGHTIR};
const int IR_SENS_PINS[] = {LEFTIRSENS, MIDDLEIRSENS, RIGHTIRSENS};
void rampGenerator()
{
  if(t%1024 == 0)
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
  
  tone(IR_PINS[currIRDir], currFreq);
  
  if(t-lastToneChange >= rampStepLength)
  {
    measuredFlag = false;
    lastToneChange = t;
    currFreq += freqStep;

    if(currFreq > maxFreq)
    {      
      currFreq = baseFreq;     
      currIRDir = (IRDirection)((currIRDir + 1) % NUM_IR_DIR);

      noTone(RIGHTIR);
      noTone(MIDDLEIR);
      noTone(LEFTIR);

      IRDists[currIRDir] = tempIRDists[currIRDir]/divider;
      tempIRDists[currIRDir] = 0;
      
    }
  }

  if(t-lastToneChange >= IRWaitTime && !measuredFlag && digitalRead(IR_SENS_PINS[currIRDir]))
  {
     measuredFlag = true;
     tempIRDists[currIRDir]++;
     
  }
}
