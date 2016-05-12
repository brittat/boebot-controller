/*const float closeThresh = 0.5;
  const float farThresh = 0.9;
  long delayStart = 0;
  long turnTime = 1000;
  long reverseTime = 3000;
  int turnSpeed = 30;
  int moveSpeed = 50;*/


void stateMachine()
{
  long t = millis();
  //int ir = getIrReadFloor(MIDDLEIR,MIDDLEIRSENS);
  //irSum = (ir + irSum)*ir;
  /*if (irSum>3 && !isReversing){
    state = 4;
    beaconSearch = false;
    isReversing = true;
    leftSpeed(0);
    rightSpeed(0);
    delay(100);
    delayStart = t;
    }*/
  /*if (walkingRandomly && t - delayStart > reverseTime * 2) // Random walk when looking for beacon
  {
    state = 5;
    walkingRandomly = false;
  }*/
  switch (state)
  {
    case 0:
      leftSpeed(moveSpeed);
      rightSpeed(moveSpeed);
      rightSens = getIrRead(RIGHTIR, RIGHTIRSENS);
      delay(5);
      leftSens = getIrRead(LEFTIR, LEFTIRSENS);
      Serial.print(leftSens);
      Serial.println(rightSens);
      if (rightSens < 1) //Too far right
      {
        state = 1;
        turnTime = random(300, 1500);
        delayStart = t;
      }
      else if (leftSens < 1) //Too far left
      {
        state = 2;
        turnTime = random(300, 1500);
        delayStart = t;
      }
      break;
    case 1: //turn left
      delay(1000000);
      leftSpeed(-turnSpeed);
      rightSpeed(turnSpeed);
      if (t - delayStart > turnTime)
      {
        state = 0;
      }
      break;

    case 2: //turn right
      leftSpeed(turnSpeed);
      rightSpeed(-turnSpeed);
      if (t - delayStart > turnTime)
      {
        state = 0;
      }
      break;

    case 4: //Safe place found, move backwards to release cylinder
      leftSpeed(-moveSpeed);
      rightSpeed(-moveSpeed);
      if (t - delayStart > reverseTime)
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
      //Serial.print(sumLeft);
      //Serial.println(sumRight);
      if (sumRight < 100 && sumLeft < 100) {
        leftSpeed(moveSpeed);
        rightSpeed(moveSpeed);
        delayStart = t;
        waitTime = reverseTime * 2;
        numberOfTurns = 0;
        state = 7;
      } else if (sumRight == 100 && sumLeft == 100) {
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        numberOfTurns++;
        if (numberOfTurns < 8) {
          delayStart = t;
          waitTime = turnTime;
          state = 7;
        } else { //If it has checked 360 degrees it goes into random walk
          walkingRandomly = true;
          state = 0;
          delayStart = t;
          numberOfTurns = 0;
        }
      } else if (sumRight == 100) {
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        delayStart = t;
        state = 7;
        numberOfTurns = 0;
      } else {
        leftSpeed(turnSpeed);
        rightSpeed(-turnSpeed);
        delayStart = t;
        state = 7;
        waitTime = turnTime;
        numberOfTurns = 0;
      }
      break;

    case 7: //Wait
      if (t - delayStart > waitTime) {
        state = 5;
      }
      break;

    case 8: //Time to scan
      doSonarSweep = true;
      state = 9;
      break;

    case 9: //Waiting for the scan to complete
      if (doSonarSweep == false) {
        if (cylinderFound) {
          cylinderFound = false;
          state = 10;
        } else {
          //set state to random walk
        }

      }
      break;

    case 10: //Approach the cylinder
      if (targetDistance > dangerZone){

        turn(targetHeading);
        driveDistance(targetDistance - dangerZone-10); 
        state = 11;
      } else {

        turn(targetHeading);
        driveDistance(targetDistance - 10); 
        
      }
      break;

    case 11: //Wait during cylinder approach

      if (drivingFinished) {
        state = 8;
      }
      break;

    case 12: //Verify that the cylinder is grabbed
      state = 13;
      break;

    case 13:
      break;
      
    
    
      
  }
  
}

