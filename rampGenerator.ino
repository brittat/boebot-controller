//long lastToneChange = 0;
//const long freqStep = 500;
//const long baseFreq = 38000;
//const long maxFreq = 44000;
//const float divider = (maxFreq - baseFreq)/freqStep;
//const int rampStepLength = 4;
//long currFreq = baseFreq;
//enum IRDirection {LEFT, RIGHT, STOP};
//IRDirection currDir = STOP;
//
//void rampGenerator()
//{
//  int t = millis();
//  if(t-lastToneChange >= rampStepLength)
//  {
//    lastToneChange = t;
//    currFreq += freqStep;
//    if(currFreq > maxFreq)
//    {
//      currFreq = baseFreq;
//      currDir = STOP;
//    }
//    switch(currDir)
//    {
//      case STOP:
//        noTone(RIGHTIR);
//        noTone(LEFTIR);
//        break;
//      case LEFT:
//        tone(LEFTIR, currFreq);
//        break;
//      case RIGHT:
//        tone(RIGHTIR, currFreq);
//    }
//  }
//  
//  for(long f = baseFreq; f < maxFreq; f = f + freqStep)
//  {
//    tone(outPin, f, 4);
//    delay(1);
//    if(digitalRead(sensPin))
//    {
//      dist = dist+1;
//    }
//    delay(1);
//  }
//}
