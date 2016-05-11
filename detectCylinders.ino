const int maxFeatureDepth = 30;


//The coefficients below are used for detecting the cylinders
//Coefficients for the polynomial approximating the width2-distance relation
const float P0 = 87.07;
const float P1 = 0.8082;

//Coefficients for the polynomial approximating the angle-distance relation
const float p0 = 1.0507;
const float p1 = -1.6639/10000;

void detectCylinders()
{
  int groups[nDir]; //Array to contain group assignments
  int cGroup = 1;
  groups[0] = cGroup;

  for(int i = 1; i < nDir; i++)
  {
    diffMillimeters[i] = highMillimeters[i] - lowMillimeters[i];  
  }

  for(int i = 1; i < nDir; i++)
  {
    if(abs(diffMillimeters[i] - diffMillimeters[i-1]) < maxFeatureDepth) // find discontinuities in range data
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
  float maxSymmComp = 0;
  int targetGroup = 0;

  for(int iGroup = 1; iGroup <= cGroup; iGroup++)
  {
    int groupLength = getGroupLength(groups, iGroup);
    //Serial.print("group length: ");
    //Serial.println(groupLength);
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
      int maximumDistDiff = getMaxDiff(groups, iGroup);
      float groupSymmComparison = getSymmComparison(groups, iGroup);
      
      if(dist < 2000 && abs(width-width2) < 150 && groupSymmComparison > 100)
      {
        if (groupSymmComparison > maxSymmComp)
        {
          Serial.print("Potential target! ");
          cylinderFound = true;
          targetHeading = heading;
          targetDistance = dist;
          maxSymmComp = groupSymmComparison;
        }
        
      }

      Serial.print("Feature ");
      Serial.print(iGroup);
      Serial.print(": ");
      Serial.print("angle = ");
      Serial.print(angle);
      Serial.print(" , angle2 = ");
      Serial.print(angle2);
      Serial.print(", distance = ");
      Serial.print(dist);
      Serial.print(", width = ");
      Serial.print(width);
      Serial.print(", width2 = ");
      Serial.print(width2);
      Serial.print(", heading = ");
      Serial.print(heading);
      Serial.print(", symmetry comparison value = ");
      Serial.println(maxSymmComp);
    }
  }
  serialPrintArray(diffMillimeters, nDir);
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
      distSum += lowMillimeters[i];
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
  return -aperture*(heading/gLength+0.5) + M_PI_2;
}

int getMaxDiff(int groups[], int group)
{
  int minDist = 10000;
  int maxDist = 0;
  for(int i = 0; i < nDir; i++)
  {
    if(groups[i] == group)
    {
      if (diffMillimeters[i] > maxDist)
      {
        maxDist = diffMillimeters[i];
      }

      if (diffMillimeters[i] < minDist)
      {
        minDist = diffMillimeters[i];
      }
    }
  }
  return maxDist - minDist;
}

float getSymmComparison(int groups[], int group)
{
  float maxDiff = getMaxDiff(groups, group);
  int lengthOfGroup = getGroupLength(groups, group);
  float m = sign(maxDiff)*maxDiff;
  float symmVec[lengthOfGroup];
  float tmpVec[lengthOfGroup];
  //serialPrintArray(int(tmpVec), lengthOfGroup)
  int j = 0;
  for(int i = 0; i < nDir; i++)
  {
    if(groups[i] == group)
    {
      tmpVec[j] = diffMillimeters[i];
      j++;
    }
  }

  float maxDistOfGroup = arrayMaxFloat(tmpVec, lengthOfGroup);
  float minDistOfGroup = arrayMinFloat(tmpVec, lengthOfGroup);
  float symmVal=0; 
  for(int i = 0; i < lengthOfGroup; i++)
  {

    symmVal = symmVal + pow((tmpVec[i]-tmpVec[lengthOfGroup-i-1])/(maxDistOfGroup-minDistOfGroup),2);  

  }
return (float(lengthOfGroup)*m)/symmVal;
  
}

