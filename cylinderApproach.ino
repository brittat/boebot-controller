void cylinderApproach()
{
  switch (approachState)
  {
    case 0: //Approach the cylinder
      
      turn(targetHeading-0.15);
      driveDistance(targetDistance - 0);
      approachState = 1;

      break;
    //if (targetDistance > dangerZone){

    //turn(targetHeading);
    //driveDistance(targetDistance - dangerZone-10);
    //state = 1;
    //Serial.println("approach 0");
    //} else {

    //turn(targetHeading);
    //driveDistance(targetDistance - 0);
    //state = 1;
    //}


    case 1: //Wait during cylinder approach
      if (drivingFinished) {
        Serial.println("cylinder approached");
        approachState = 2;
      }
      break;

    case 2: //Verify that the cylinder is grabbed
      verifyCylinder = true;
      approachState = 3;
      break;
      
    case 3:

      if (!verifyCylinder) {
        if (cylinderGrabbed) {
          Serial.println("cylinder is grabbed");
          behaviourState = beaconFind;



        } else {
          Serial.println("cylinder is not grabbed");
          behaviourState = rWalk;

        }

        approachState = 0;
      }

      break;
  }
}
