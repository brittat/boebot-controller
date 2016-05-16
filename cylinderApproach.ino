void cylinderApproach()
{
  switch(state)
  {
      case 0: //Approach the cylinder
        if (targetDistance > dangerZone){
  
          turn(targetHeading);
          driveDistance(targetDistance - dangerZone-10); 
          state = 1; 
        } else {
  
          turn(targetHeading);
          driveDistance(targetDistance - 10); 
          state = 1;
        }
        break;
  
      case 2: //Wait during cylinder approach
  
        if (drivingFinished) {
          state = 3;
          behaviourState = beaconFind;
          cylinderFound = false;
        }
        break;
  
      case 4: //Verify that the cylinder is grabbed
        state = 4;
        break;
  
      case 13:
        break;
  
  }
}
