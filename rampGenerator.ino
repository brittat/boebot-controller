


void rampGenerator()
{
  
  long t = millis();
  if(t-lastToneChange >= rampStepLength)
  {
    measuredFlag = false;
    lastToneChange = t;
    currFreq += freqStep;
    Serial.println(currIRDir);
    if(currFreq > maxFreq && currIRDir == RIGHT)
    {
      
      currFreq = baseFreq;
      IRDistLeft = tempIRLeft;
      tempIRLeft = 0;
      currIRDir = STOPRIGHT;
    }

    
    if(currFreq > maxFreq && currIRDir == LEFT)
    {
      IRDistRight = tempIRRight;
      tempIRRight = 0;
      currFreq = baseFreq;
      currIRDir = STOPLEFT;
    }
    switch(currIRDir)
    {
      case STOPLEFT:
        noTone(RIGHTIR);
        noTone(LEFTIR);
        currIRDir = RIGHT;
        break;
        
      case STOPRIGHT:
        Serial.println("derp");
        noTone(RIGHTIR);
        noTone(LEFTIR);
        currIRDir = LEFT;
        break;
        
      case LEFT:
        tone(LEFTIR, currFreq);
        break;
        
      case RIGHT:
        tone(RIGHTIR, currFreq);
        break;
    }
  }
  if(currIRDir == LEFT)
  {
    
    if(t-lastToneChange >= IRWaitTime && !measuredFlag)
    {
      
      if(digitalRead(LEFTIRSENS))
      {
       
       measuredFlag == true;
       tempIRLeft = tempIRLeft + 1;
      }
    }
  }
  else if (currIRDir == RIGHT)
  {
    
    if(t-lastToneChange >= IRWaitTime && !measuredFlag)
    {
      if(digitalRead(RIGHTIRSENS))
      {
        measuredFlag == true;
        tempIRRight = tempIRRight + 1;
      }
    }
  }
}
  
