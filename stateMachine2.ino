
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
   state = 0;
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
