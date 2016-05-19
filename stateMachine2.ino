
void stateMachine2()
{  
  long t = micros();
  int ir = getIrReadFloor(MIDDLEIR,MIDDLEIRSENS);
  irSum = (ir + irSum)*ir;
  Serial.println(irSum);
  if (irSum>6 && behaviourState != reverse){    
    behaviourState = reverse;
    safeState = 0;
  }
  if (cylinderFound)
  {
   approachState = 0;
   behaviourState = approachCylinder; 
   cylinderFound = false;
  }
  switch(behaviourState)
  {
    case rWalk:
      randomWalk();
      break;
    case approachCylinder:
      cylinderApproach();
      break;
    case beaconFind:
      findBeacon();
      break;
    case reverse:
      safePlaceFound();
      break;
    case beaconExplore:
      randomWalk();
      break;
  }
    
}
