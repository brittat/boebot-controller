void cylinderApproach()
{
  switch (state)
  {
    case 0: //Approach the cylinder

      turn(targetHeading);
      driveDistance(targetDistance - 10);
      state = 1;

      break;
    //if (targetDistance > dangerZone){

    //turn(targetHeading);
    //driveDistance(targetDistance - dangerZone-10);
    //state = 1;
    //Serial.println("approach 0");
    //} else {

    //turn(targetHeading);
    //driveDistance(targetDistance - 10);
    //state = 1;
    //}


    case 1: //Wait during cylinder approach
      if (drivingFinished) {
        Serial.println("cylinder approached");
        state = 2;
      }
      break;

    case 2: //Verify that the cylinder is grabbed
      if(cylinderGrabbed){

        behaviourState = beaconFind;
        
      }else{
        
        behaviourState = rWalk;        
      
      }

      Serial.println("approach 4");

      break;
  }
}
