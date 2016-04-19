


const int dirAngle = 180/nDir;
void sonarStateMachine()
{ 
  switch(sonarState)
  {
    case 0: // Move servo into position
      sonarMotor.write(dirAngle*dirAngle);
      pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
      digitalWrite(pingPin, LOW);
      startDelay = t;
      sonarState = 1;
      break;
      
    case 1: //Wait for servo
      if(t - startDelay > servoWaitTime)
      {
        sonarState = 2;
        startDelay = t;
      }
      break;

    case 2: // Send init pulse
      digitalWrite(pingPin, HIGH);
      if(t - startDelay > initPulseTime)
      {
        sonarState = 3;
        
        digitalWrite(pingPin, LOW);        
        pinMode(pingPin, INPUT);
      }
      break;

    case 3:
      if(digitalRead(pingPin) == HIGH)
      {
        sonarState = 4;
        startDelay = t;
      }
      break;

    case 4: // Wait for input pin to go low

      if(digitalRead(pingPin) == LOW)
      {
        long duration = t - startDelay;
        millimeters[currDir] = duration * 0.169 ;
        sonarState = 5;
      }
      break;

    case 5: // Set next direction and repeat
      currDir++;
      currDir = currDir%nDir;
      sonarState = 0;
      break;
  }
}

