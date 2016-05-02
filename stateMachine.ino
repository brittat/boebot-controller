const float closeThresh = 0.5;
const float farThresh = 0.9;
long delayStart = 0;
long turnTime = 1000;
long reverseTime = 3000;
int turnSpeed = 50;
int moveSpeed = 75;
boolean beaconSearch = true;

void stateMachine()
{
  long t = millis();
  int ir = getIrRead(MIDDLEIR,MIDDLEIRSENS);
  /*irSum = (ir + irSum)*ir;
  if (irSum>3 && isReversing){
    state = 4;
    isReversing = true;
    leftSpeed(0);
    rightSpeed(0);
    delay(100);
    delayStart = t;
  }*/
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
      
    case 4: //Safe place found, move backwards to release cylinder  
       leftSpeed(-moveSpeed);
       rightSpeed(-moveSpeed);    
       if(t - delayStart > reverseTime)
       {
        state = 0;
        leftSpeed(0);
        rightSpeed(0);
        state = random(1) + 1; //Choose left or right at random
        turnTime = random(1500);
        isReversing = false;
       delayStart = t;
       }      
       break;

    case 5: //Beacon search initiated, stop for scan
      leftSpeed(0);
      rightSpeed(0);
      state = 6;
      break;

    case 6: //Scan for beacon
      beaconSearch = true;
      sumRight = sumIR(RIGHTIRSENS);
      sumLeft = sumIR(LEFTIRSENS);
      if (sumRight < 1 && sumLeft < 1){
        leftSpeed(moveSpeed);
        rightSpeed(moveSpeed);
        delayStart = t;
        waitTime = reverseTime;
        numberOfTurns = 0;
        state = 7;
      }else if(sumRight == 1 && sumLeft == 1){ 
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        numberOfTurns++;
        if (numberOfTurns < 8){ 
          delayStart = t;
          waitTime = turnTime;
          state = 7;
        }else{ //If it has checked 360 degrees it goes into random walk
          state = 0;
          delayStart = t;  
        }
      }else if(sumRight == 1){
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        delayStart = t;
        state = 7;
        numberOfTurns = 0;
      }else{
        leftSpeed(turnSpeed);
        rightSpeed(-turnSpeed);
        delayStart = t;
        state = 7;
        waitTime = turnTime;
        numberOfTurns = 0;
      }
      break;
      
    case 7: //Wait
      if (t - delayStart > waitTime){
        state = 6;
      }
      break;      
  }
}

