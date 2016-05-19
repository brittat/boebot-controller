
void randomWalk()
{
long t = micros();
switch(randomState)
  {
    case 0: 

    if (t - randomWalkStart > randomWalkTime && behaviourState == beaconExplore)
    {   
      behaviourState = beaconFind;
      randomState = 0;
    } else if (doSonarSweep)
    {
      rightSpeed(0);
      leftSpeed(0);
      timeSinceLastScan = t;
    } else if (t - timeSinceLastScan > scanInterval && behaviourState != beaconExplore)
    {
      doSonarSweep = true;
      rightSpeed(0);
      leftSpeed(0);
      randomState = 0;
    } else {
      randomState = 1;
    }
    break;
    
    case 1:
      leftSpeed(moveSpeed);
      rightSpeed(moveSpeed);
      rightSens = getIrRead(RIGHTIR,RIGHTIRSENS);
      leftSens = getIrRead(LEFTIR,LEFTIRSENS);
      if(rightSens < 1) //Too far right
      {
        randomState = 2;
        turnTime = random(300000, 1500000);
        delayStart = t;
      }
      else if (leftSens < 1) //Too far left
      {
        randomState = 3;
        turnTime = random(300000, 1500000);
        delayStart = t;
      } else {
        randomState = 0;
      }
      break;
    case 2: //turn left
      leftSpeed(-turnSpeed);
      rightSpeed(turnSpeed);
      if(t - delayStart > turnTime)
      {
        randomState = 0;
      }
      break;

    case 3: //turn right
      leftSpeed(turnSpeed);
      rightSpeed(-turnSpeed);
      if(t - delayStart > turnTime)
      {
        randomState = 0;
      }
      break;  
  }
}
