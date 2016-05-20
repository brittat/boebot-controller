const float dirAngle = degreeAperture/nDir;
int verifyAngle = 30;
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

    case -2: //Aim sonars forward
      //Serial.println("Aim sonars forward, later detectCylinders()");
      sonarMotor.write(90);
      sonarDelay = t;
      sonarState = -33;
      break;

    case -22: //Aim sonars forward
      //Serial.println("Aim sonars forward, no detectCylinders()");
      sonarMotor.write(90);
      sonarDelay = t;
      sonarState = -3;
      break;

    case -3: //Wait for servo to aim sonars forward
      //Serial.println("Waiting for sonars to aim forward");
      if(t - sonarDelay > 2*servoWaitTime)
      {
        sonarState = -1;
        doSonarSweep = false;
      }      
      break;

    case -33: //Wait for servo to aim sonars forward
      //Serial.println("Waiting for sonars to aim forward, and then we detectCylinders()");
      if(t - sonarDelay > 2*servoWaitTime)
      {
        sonarState = -1;
        doSonarSweep = false;
        detectCylinders();
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
        sonarState = -2;
        Serial.println("I found no cylinders and it's time to aim the sonars forward");
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
          //Serial.println(verifyHigh);
          //Serial.println(verifyLow);
         if(verifyHigh-verifyLow > 30 && verifyLow < 100){

          cylinderGrabbed = true;
          Serial.println("I grabbed a cylinder!");
          verifyCylinder = false;
          sonarState = -22;
          
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
            verifyAngle = 10;
            cylinderGrabbed = false;
            verifyCylinder = false;
            Serial.println("I verified the measurements and it's time to aim the sonars forward");
            sonarState = -22; //Aim sonars forward
            
          }
         }
      break;
  }
}

