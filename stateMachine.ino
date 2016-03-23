const float closeThresh = 0.5;
const float farThresh = 0.9;

void stateMachine()
{
  switch(state)
  {
    case 0:
      leftSpeed(50);
      rightSpeed(50);
      if(rightSens < closeThresh) //Too far right
      {
        state = 1;
      }
      else if (rightSens > farThresh) //Too far left
      {
        state = 2;
      }
      break;
    case 1: //turn left
      leftSpeed(10);
      rightSpeed(50);
      if(rightSens > closeThresh)
      {
        state = 0;
      }
      break;

    case 2: //turn right
      leftSpeed(50);
      rightSpeed(10);
      if(rightSens < farThresh)
      {
        state = 0;
      }
      break;
      
  }
}

