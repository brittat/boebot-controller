
int dangerZone = 100;

void dockCylinder()
{

  if (!cylinderGrabbed)
  {

    if (targetDistance < 20) {
      turn(targetHeading);
      //cylinderGrabbed = true;
      leftSpeed(0);
      rightSpeed(0);

    } else {

      if (targetDistance < 110) {

        dangerZone = 10;

      }

      turn(targetHeading);
      driveDistance(targetDistance - dangerZone);

    }

  }

}
