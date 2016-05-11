void drivingTimer()
{
  if(t - drivingStart > drivingTime && drivingStart != -1)
  {
    leftSpeed(0);
    rightSpeed(0);
    drivingStart = -1;
  }  
}

