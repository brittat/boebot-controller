

void randomWalk()
{
long t = micros();
switch(state)
  {
    case 0: 
    
      doSonarSweep = true;

    break;
  }
}
//
//void randomWalk()
//{
//long t = micros();
//switch(state)
//  {
//    case 0: 
//    if (t - randomWalkStart > randomWalkTime && behaviourState == beaconExplore)
//    {   
//      Serial.println("Hej");
//      behaviourState = beaconFind;
//    } else if (doSonarSweep)
//    {
//      rightSpeed(0);
//      leftSpeed(0);
//      timeSinceLastScan = t;
//    } else if (t - timeSinceLastScan > scanInterval && behaviourState != beaconExplore)
//    {
//      doSonarSweep = true;
//      rightSpeed(0);
//      leftSpeed(0);
//      Serial.println(4);
//    } else {
//      state = 1;
//    }
//    break;
//    
//    case 1:
//      leftSpeed(moveSpeed);
//      rightSpeed(moveSpeed);
//      rightSens = getIrRead(RIGHTIR,RIGHTIRSENS);
//      //delay(5);
//      leftSens = getIrRead(LEFTIR,LEFTIRSENS);
//      if(rightSens < 1) //Too far right
//      {
//        state = 2;
//        turnTime = random(300000, 1500000);
//        delayStart = t;
//      }
//      else if (leftSens < 1) //Too far left
//      {
//        state = 3;
//        turnTime = random(300000, 1500000);
//        delayStart = t;
//      } else {
//        state = 0;
//      }
//      break;
//    case 2: //turn left
//      leftSpeed(-turnSpeed);
//      rightSpeed(turnSpeed);
//      if(t - delayStart > turnTime)
//      {
//        state = 0;
//      }
//      break;
//
//    case 3: //turn right
//      leftSpeed(turnSpeed);
//      rightSpeed(-turnSpeed);
//      if(t - delayStart > turnTime)
//      {
//        state = 0;
//      }
//      break;  
//  }
//}





