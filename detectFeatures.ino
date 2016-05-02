const int maxFeatureDepth = 30;
const float aperture = 3.14/nDir;

//The coefficients below are used for detecting the cylinders
//Coefficients for the polynomial approximating the width2-distance relation
const float P0 = 87.07;
const float P1 = 0.8082;

//Coefficients for the polynomial approximating the angle-distance relation
const float p0 = 1.0507;
const float p1 = -1.6639/10000;

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
      float angle2 = p0 + p1*dist;
      
      if(dist < 2000 && abs(width-width2) < 150 && abs(angle-angle2) < 0.2)
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
      Serial.print(", width2 = ");
      Serial.print(width2);
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
  return -aperture*heading/gLength + M_PI_2;
}


