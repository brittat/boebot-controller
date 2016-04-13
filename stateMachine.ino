const float closeThresh = 0.5;
const float farThresh = 0.9;
long delayStart = 0;
long turnTime = 1000;
int turnSpeed = 75;
int moveSpeed = 100;

void stateMachine()
{
  long t = millis();
  switch(state)
  {
    case 0:
      leftSpeed(moveSpeed);
      rightSpeed(moveSpeed);
      if(rightSens < farThresh) //Too far right
      {
        state = 1;
        turnTime = random(300, 1500);
        delayStart = t;
      }
      else if (leftSens < farThresh) //Too far left
      {
        state = 2;
        turnTime = random(1500);
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

