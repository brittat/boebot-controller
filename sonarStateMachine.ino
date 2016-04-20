


const int dirAngle = 180/nDir;
void sonarStateMachine()
{ 
  if(newSonarPulse)
  {
    millimeters[currDir] = calcSonarDistance();
    newSonarPulse = false;
    serialPrintArray(millimeters, nDir);
  }
  
  switch(sonarState)
  {
    case 0: // Move servo into position
      sonarMotor.write(currDir*dirAngle);
      pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
      digitalWrite(pingPin, LOW);
      startDelay = t;
      sonarState = 1;
      break;
      
    case 1: //Wait for servo
      if(t - startDelay > servoWaitTime)
      {
        sonarState = 2;
        startDelay = t;
      }
      break;

    case 2: // Send init pulse
      sonarPulse();
      startDelay = t;
      sonarState = 3;
      break;

    case 3:
      if(t-startDelay > sonarLinger)
      {
        sonarState = 4;
      }
      break;

    case 4: // Set next direction and repeat
      currDir++;
      currDir = currDir%nDir;
      sonarState = 0;
      break;
  }
}

