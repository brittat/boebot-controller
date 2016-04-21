const int maxFeatureDepth = 30;
const float aperture = 3.14/nDir;
void detectFeatures()
{
  int groups[nDir];
  int cGroup = 1;
  groups[0] = cGroup;

  for(int i = 1; i < nDir; i++)
  {
    if(abs(millimeters[i] - millimeters[i-1]) < maxFeatureDepth)
    {
      groups[i] = cGroup;
    }
    else
    {
      cGroup++;
      groups[i] = cGroup;
    }
  }

  for(int iGroup = 1; iGroup <= cGroup; iGroup++)
  {
    int groupLength = getGroupLength(groups, iGroup);
    if(groupLength > 1)
    {
      int angle = aperture * groupLength;
      int dist = getMeanGroupDist(groups, iGroup);
      int width = 2 * dist * sin(angle/2);
      float heading = getGroupHeading(groups, iGroup);

      Serial.print("Feature ");
      Serial.print(iGroup);
      Serial.print(":");
      Serial.print("angle = ");
      Serial.print(angle);
      Serial.print(", distance = ");
      Serial.print(dist);
      Serial.print(", width = ");
      Serial.print(width);
      Serial.print(", heading = ");
      Serial.println(heading);
      
    }
  }
}

int getGroupLength(int groups[], int group)
{
  int nPoints = 0;
  for(int i = 0; i < nDir; i++)
  {
    if(groups[i] == group)
    {
      nPoints++;
    }
  }
  return nPoints;
}

int getMeanGroupDist(int groups[], int group)
{
  int distSum = 0;
  for(int i = 0; i < nDir; i++)
  {
    if(groups[i] == group)
    {
      distSum += millimeters[i];
    }
  }
  return distSum/getGroupLength(groups, group);
}

float getGroupHeading(int groups[], int group)
{
  float heading = 0;
  int gLength = 0;
  for(int i = 0; i < nDir; i++)
  {
    if(groups[i] == group)
    {
      heading += i;
      gLength += 1;
    }
  }
  return aperture*heading/gLength;
}


