



void sonarStateMachine()
{ 
  tSonar = micros();
  switch(sonarState)
  {
    case 0: // Move servo into position
      sonarMotor.write(180*currDir/nDir);
      pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
      digitalWrite(pingPin, LOW);
      startDelay = tSonar;
      sonarState = 1;
      break;
      
    case 1: //Wait for servo
      if(tSonar - startDelay > servoWaitTime)
      {
        sonarState = 2;
        startDelay = tSonar;
      }
      break;

    case 2: // Send init pulse
      digitalWrite(pingPin, HIGH);
      if(tSonar - startDelay > initPulseTime)
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
        startDelay = tSonar;
      }
      break;

    case 4: // Wait for input pin to go low

      if(digitalRead(pingPin) == LOW)
      {
        long duration = tSonar - startDelay;
        centimeters[currDir] = duration / 59.2 ;
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

