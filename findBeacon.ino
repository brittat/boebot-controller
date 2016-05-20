void findBeacon()
{
//long t = micros();
switch(beaconState)
{
    case 0: //Beacon search initiated, stop for scan
      leftSpeed(0);
      rightSpeed(0);
      beaconState = 1;
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
        waitTime = 10*waitTimeLong;
        numberOfTurns = 0;
        beaconState = 2;
      }else if(summedBeaconRead == 0){ //No detection
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        numberOfTurns++;
        if (numberOfTurns < 8){ 
          waitTime = waitTimeLong;
          delayStart = t;
          beaconState = 2;
        }else{ //If it has checked 360 degrees it goes into random walk
          behaviourState = beaconExplore;
          randomState = 0;
          beaconState = 0;
          randomWalkStart = t;
          numberOfTurns = 0;  
        }
      }else if(summedBeaconRead == 1){ //If no detection on the right, rotate to the left
        leftSpeed(-turnSpeed);
        rightSpeed(turnSpeed);
        delayStart = t;
        beaconState = 2;
        numberOfTurns = 0;
        waitTime = waitTimeShort;
      }else{ //If no detection on the left, rotate to the right
        leftSpeed(turnSpeed);
        rightSpeed(-turnSpeed);
        delayStart = t;
        beaconState = 2;
        numberOfTurns = 0;
        waitTime = waitTimeShort;
      }
      break;
      
    case 2: //Wait, keep on doing what you're doing
      if (t - delayStart > waitTime){
        if (summedBeaconRead ==3)
        {
          verifyCylinder = true;
          leftSpeed(0);
          rightSpeed(0);          
          beaconState = 3;
        }else {
          beaconState = 0;
        }
      }
      break;
    case 3:
    if(verifyCylinder == false)
     {
      if(cylinderGrabbed)
      {
      beaconState = 0;
      }
      else
      {
        behaviourState = rWalk;
        beaconState = 0; 
      }
     }
  }
}
