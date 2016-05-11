void findBeacon()
{
long t = millis();
switch(state)
{
    case 0: //Beacon search initiated, stop for scan
      leftSpeed(0);
      rightSpeed(0);
      state = 1;
      break;

    case 1: //Scan for beacon
      sumRight = sumIR(RIGHTIRSENS);
      sumLeft = sumIR(LEFTIRSENS);
      if (sumRight < 100 && sumLeft < 100){
        leftSpeed(moveSpeed);
        rightSpeed(moveSpeed);
        delayStart = t;
        waitTime = reverseTime*2;
        numberOfTurns = 0;
        state = 7;
      }else if(sumRight == 100 && sumLeft == 100){ 
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        numberOfTurns++;
        if (numberOfTurns < 8){ 
          delayStart = t;
          waitTime = turnTime;
          state = 7;
        }else{ //If it has checked 360 degrees it goes into random walk
          walkingRandomly = true;
          state = 0;
          randomWalkStart = t;
          numberOfTurns = 0;  
        }
      }else if(sumRight == 100){ //If no detection on the right, rotate to the left
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        delayStart = t;
        state = 7;
        numberOfTurns = 0;
      }else{ //If no detection on the left, rotate to the right
        leftSpeed(turnSpeed);
        rightSpeed(-turnSpeed);
        delayStart = t;
        state = 7;
        waitTime = turnTime;
        numberOfTurns = 0;
      }
      break;
      
    case 2: //Wait
      if (t - delayStart > waitTime){
        state = 5;
      }
      break;      
  }
}
