


const float dirAngle = 150/nDir;
void sonarStateMachine()
{ 
  t = micros();
  if(newLowSonarPulse)
  {
    lowMillimeters[currDir] = calcLowSonarDistance();
    newLowSonarPulse = false;
    //serialPrintArray(millimeters, nDir);
  }
  if(newHighSonarPulse)
  {
    highMillimeters[currDir] = calcHighSonarDistance();
    newHighSonarPulse = false;
    //serialPrintArray(millimeters, nDir);
  }
  switch(sonarState)
  {
    case -1:
      if(doSonarSweep)
      {
        sonarState = 0;
      }
      break;
    case 0: // Move servo into position
      sonarMotor.write(currDir*dirAngle + 30 );
      sonarDelay = t;
      if(currDir == 0)
      {
        sonarDelay = t + 1000000; //Wait an extra second if turning back
      }
      sonarState = 11;
      break;
      
    case 11: //Wait for servo
      if(t - sonarDelay > servoWaitTime)
      {
        sonarState = 12;
      }      
      break;

    case 12: // Send init pulse
      lowSonarPulse();
      sonarDelay = t;
      sonarState = 21;
      break;

     case 21: //Wait for servo
      if(t - sonarDelay > sonarWaitTime)
      {
        sonarState = 22;
      }
      
      break;

    case 22: // Send init pulse
      highSonarPulse();
      sonarDelay = t;
      sonarState = 3;
      break;

    case 3:
      if(t-sonarDelay > sonarLinger)
      {
        sonarState = 4;
      }
      break;

    case 4: // Set next direction and repeat
      currDir++;
      currDir = currDir%nDir;
      sonarState = 0;
      
      if(currDir == 0)
      {
        detectCylinders();
        sonarState = -1;
      }      
      
      break;
  }
}

