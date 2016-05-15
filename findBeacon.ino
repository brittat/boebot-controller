void findBeacon()
{
//long t = micros();
switch(state)
{
    case 0: //Beacon search initiated, stop for scan
      leftSpeed(0);
      rightSpeed(0);
      state = 1;
      break;

    case 1: //Scan for beacon
      //sumRight = sumIR(RIGHTIRSENS);
      //sumLeft = sumIR(LEFTIRSENS);
      summedBeaconRead = beaconRead();
      t = micros();
      //Serial.println(summedBeaconRead);
      if (summedBeaconRead == 3){ //Detection on both sensors
        leftSpeed(moveSpeed);
        rightSpeed(moveSpeed);
        delayStart = t;
        numberOfTurns = 0;
        state = 2;
      }else if(summedBeaconRead == 0){ //No detection
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        numberOfTurns++;
        if (numberOfTurns < 8){ 
          delayStart = t;
          state = 2;
        }else{ //If it has checked 360 degrees it goes into random walk
          walkingRandomly = true;
          state = 0;
          randomWalkStart = t;
          numberOfTurns = 0;  
        }
      }else if(summedBeaconRead == 1){ //If no detection on the right, rotate to the left
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        delayStart = t;
        state = 2;
        numberOfTurns = 0;
      }else{ //If no detection on the left, rotate to the right
        leftSpeed(turnSpeed);
        rightSpeed(-turnSpeed);
        delayStart = t;
        state = 2;
        numberOfTurns = 0;
      }
      break;
      
    case 2: //Wait, keep on doing what you're doing
      Serial.println(t-delayStart);
      if (t - delayStart > waitTime){
        state = 0;
      }
      break;      
  }
}
