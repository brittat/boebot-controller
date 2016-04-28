const int maxFeatureDepth = 30;
const float aperture = 3.14/nDir;

const float P0 = 0.6771;
const float P1 = 147.2347;


void detectFeatures()
{
  int groups[nDir]; //Array to contain group assignments
  int cGroup = 1;
  groups[0] = cGroup;

  for(int i = 1; i < nDir; i++)
  {
    if(abs(millimeters[i] - millimeters[i-1]) < maxFeatureDepth) // find discontinuities in range data
    {
      groups[i] = cGroup;
    }
    else
    {
      cGroup++;
      groups[i] = cGroup;
    }
  }
  Serial.println("Features found:");
  for(int iGroup = 1; iGroup <= cGroup; iGroup++)
  {
    int groupLength = getGroupLength(groups, iGroup);
    if(groupLength > 1)
    {
      float angle = aperture * groupLength; //Angle taken up by the feature
      int dist = getMeanGroupDist(groups, iGroup); //Average distance to feature
      //float width = 2 * dist * sin((angle-dist/1000.0)/2); //Actual width of feature as seen by sonar. currently very unreliable
      float theta = groupLength*aperture/2;
      float width = 2*dist*tan(theta); //This one is approximately linear against the distance to the group
      float heading = getGroupHeading(groups, iGroup); // Direction to feature.
      float width2 = P0 + P1*dist;
      
      if(angle >= 0.58 && angle <= 1 && dist > 350 && dist < 3000)
      {
        Serial.print("Potential target! ");
        targetHeading = heading;
      }
      
      Serial.print("Feature ");
      Serial.print(iGroup);
      Serial.print(": ");
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
  serialPrintArray(millimeters, nDir);
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
  return aperture*heading/gLength - M_PI_2;
}


