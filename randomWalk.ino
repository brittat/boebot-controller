
void randomWalk()
{
long t = micros();
switch(state)
  {
    case 0:
      leftSpeed(moveSpeed);
      rightSpeed(moveSpeed);
      rightSens = getIrRead(RIGHTIR,RIGHTIRSENS);
      //delay(5);
      leftSens = getIrRead(LEFTIR,LEFTIRSENS);
      if(rightSens < 1) //Too far right
      {
        state = 1;
        turnTime = random(300000, 1500000);
        delayStart = t;
      }
      else if (leftSens < 1) //Too far left
      {
        state = 2;
        turnTime = random(300000, 1500000);
        delayStart = t;
      }
      break;
    case 1: //turn left
      leftSpeed(-turnSpeed);
      rightSpeed(turnSpeed);
      if(t - delayStart > turnTime)
      {
        state = 0;
      }
      break;

    case 2: //turn right
      leftSpeed(turnSpeed);
      rightSpeed(-turnSpeed);
      if(t - delayStart > turnTime)
      {
        state = 0;
      }
      break;  
  }
}
