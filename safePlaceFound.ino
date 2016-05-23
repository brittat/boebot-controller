void safePlaceFound()
{
long t = micros();
switch(safeState)
{
  case 0: //Stop
    leftSpeed(0);
    rightSpeed(0);
    delay(200);
    safeState = 1;
    reverseStart = t;
    break;

  case 1: //Reverse
    leftSpeed(-randMoveSpeed);
    rightSpeed(-randMoveSpeed);    
    if (t - reverseStart > reverseTime)
    {
      behaviourState = rWalk;
      //randomState = random(1) + 2; //Choose left or right at random
      randomState = 0;
      safeState = 0;
      //turnTime = random(1500000);
      //delayStart = t;
      turn(M_PI);  
    } 
    break;
}          
}
