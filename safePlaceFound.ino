void safePlaceFound()
{
long t = micros();
switch(state)
{
  case 0: //Stop
    leftSpeed(0);
    rightSpeed(0);
    delay(200);
    state = 1;
    reverseStart = t;
    break;

  case 1: //Reverse
    leftSpeed(-moveSpeed);
    rightSpeed(-moveSpeed);
    break;
    if (t - reverseStart > reverseTime)
    {
      behaviourState = rWalk;
      state = random(1) + 1; //Choose left or right at random
      turnTime = random(1500);
      delayStart = t;  
    } 
}          
}
