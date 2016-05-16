

void stateMachine2()
{  
  long t = micros();
  int ir = getIrReadFloor(MIDDLEIR,MIDDLEIRSENS);
  irSum = (ir + irSum)*ir;
 /* if (irSum>3 && behaviourState != reverse){    
    behaviourState = reverse;
    state = 0;
  }*/
  if (cylinderFound)
  {
   behaviourState = approachCylinder; 
  }
  switch(behaviourState)
  {
    case rWalk:
      randomWalk();
    case approachCylinder:
      cylinderApproach();
    case beaconFind:
      findBeacon();
    case reverse:
      safePlaceFound();
    case beaconExplore:
      randomWalk;
  }
    
}
