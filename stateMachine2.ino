

void stateMachine2()
{  
  long t = micros();
  int ir = getIrReadFloor(MIDDLEIR,MIDDLEIRSENS);
  irSum = (ir + irSum)*ir;
  if (irSum>3 && !isReversing){    
    beaconSearch = false;
    isReversing = true; 
    walkingRandomly = false;
    state = 0;
  }
  else if (isReversing && t - reverseStart > reverseTime)
  {
    walkingRandomly = true;
    isReversing = false;
    beaconSearch = false;
    state = random(1) + 1; //Choose left or right at random
    turnTime = random(1500);
    delayStart = t;  
  }
  else if (walkingRandomly && beaconSearch && t - randomWalkStart > randomWalkTime)
  {   
    walkingRandomly = false;
    state = 0;
  } else if (timeToScan)
  { 
    sonarState = 0;
    sonarStateMachine();
    if (cylinderFound)
    {
      approachingCylinder = true;
    }
  }

  if (walkingRandomly){
    randomWalk();
  }else if (approachingCylinder){
    cylinderApproach();
  }else if (beaconSearch){
    findBeacon();
  }else if (isReversing)  {
    safePlaceFound();
  }
}
