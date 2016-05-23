int condition = 0;
float oldval = 0;
void stateMachine2()
{  
  long t = micros();
  //int ir = getIrReadFloor(MIDDLEIR,MIDDLEIRSENS);
  //irSum = (ir + irSum)*ir;
  //Serial.println(IRDists[1]);
  if(t%4 == 0) 
  {
    oldval= IRDists[1];    
  }
  float temp = (IRDists[1]+oldval)/2;
  condition = ((temp>0.89) + condition)*(temp > 0.89);
  
  if (condition > 3 && behaviourState != reverse){ 
    behaviourState = reverse;
    safeState = 0;
    
  }
  if(condition > 3)Serial.println(condition);
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
