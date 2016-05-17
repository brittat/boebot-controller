const float dirAngle = degreeAperture/nDir;
int verifyAngle = 10;
void sonarStateMachine()
{ 
  t = micros();
  if(newLowSonarPulse && verifyCylinder)
  {
    verifyLow = calcLowSonarDistance();
    newLowSonarPulse = false;
  } else if (newLowSonarPulse)
  {
    lowMillimeters[currDir] = calcLowSonarDistance();
    newLowSonarPulse = false;
    //serialPrintArray(millimeters, nDir);
  }
  if(newHighSonarPulse && verifyCylinder)
  {
    verifyHigh = calcHighSonarDistance();
    newHighSonarPulse = false;
  } else if (newHighSonarPulse)
  {
    highMillimeters[currDir] = calcHighSonarDistance();
    newHighSonarPulse = false;
    //serialPrintArray(millimeters, nDir);
  }
  switch(sonarState)
  {
    case -1:
      if(doSonarSweep)
      {
        sonarState = 0;
      }
      else if (verifyCylinder)
      {
        sonarState = 100;
      }
      break;
    case 0: // Move servo into position
      //sonarMotor.write(90);
      sonarMotor.write(currDir*dirAngle + 15);
      sonarDelay = t;
      if(currDir == 0)
      {
        sonarDelay = t + 1000000; //Wait an extra second if turning back
      }
      sonarState = 11;
      break;
      
    case 11: //Wait for servo
      if(t - sonarDelay > servoWaitTime)
      {
        sonarState = 12;
      }      
      break;

    case 12: // Send init pulse
      lowSonarPulse();
      sonarDelay = t;
      sonarState = 21;
      break;

     case 21: //Wait for servo
      if(t - sonarDelay > sonarWaitTime)
      {
        sonarState = 22;
      }
      
      break;

    case 22: // Send init pulse
      highSonarPulse();
      sonarDelay = t;
      sonarState = 3;
      break;

    case 3:
      if(t-sonarDelay > sonarLinger)
      {
        sonarState = 4;
      }
      break;

    case 4: // Set next direction and repeat
      currDir++;
      currDir = currDir%nDir;
      sonarState = 0;
      
      if(currDir == 0)
      {
        detectCylinders();
        doSonarSweep = false;
        sonarState = -1;
      }      
      
      break;

      case 100:
      Serial.println("I check for grabbed cylinders");
      sonarMotor.write(verifyAngle);
      sonarDelay = t;
      
      sonarState = 101;
      break;
      
    case 101: //Wait for servo
      if(t - sonarDelay > servoWaitTime)
      {
        sonarState = 102;
      }      
      break;

    case 102: // Send init pulse
      lowSonarPulse();
      sonarDelay = t;
      sonarState = 103;
      break;

     case 103: //Wait for servo
      if(t - sonarDelay > sonarWaitTime)
      {
        sonarState = 104;
      }
      
      break;

    case 104: // Send init pulse
      highSonarPulse();
      sonarDelay = t;
      sonarState = 105;
      break;

    case 105:
      if(t-sonarDelay > sonarLinger)
      {
        sonarState = 106;
      }
      break;

    case 106: // Done 
          Serial.println(verifyHigh);
          Serial.println(verifyLow);
         if(verifyHigh-verifyLow > 30){

          cylinderGrabbed = true;
          Serial.println("I found a cylinder!");
          Serial.println("And I grabbed it!");
          verifyCylinder = false;
          sonarState = -1;
          
         }
         else
         {
          verifyAngle += 20;
          if(verifyAngle < degreeAperture)
          {
            sonarMotor.write(verifyAngle);
            sonarState = 101;
            sonarDelay = t;
          }
          else
          {
            cylinderGrabbed = false;
            verifyCylinder = false;
            sonarState = -1;
          }
         }
      break;
  }
}

