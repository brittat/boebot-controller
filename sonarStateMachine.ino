


const int dirAngle = 180/nDir;
void sonarStateMachine()
{ 
  if(newSonarPulse)
  {
    millimeters[currDir] = calcSonarDistance();
    newSonarPulse = false;
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
      sonarMotor.write(currDir*dirAngle);
      pinMode(PINGPIN, OUTPUT);          // Set pin to OUTPUT
      digitalWrite(PINGPIN, LOW);
      startDelay = t;
      if(currDir == 0)
      {
        startDelay += 1000000; //Wait an extra second if turning back
      }
      sonarState = 1;
      break;
      
    case 1: //Wait for servo
      if(t - startDelay > servoWaitTime)
      {
        sonarState = 2;
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
      
      if(currDir == 0)
      {
        detectFeatures();
        sonarState = -1;
      }      
      
      break;
  }
}

