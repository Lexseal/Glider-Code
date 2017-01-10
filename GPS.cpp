#include "GPS.h"

bool GPS::getGPSInfo()
{
  while (Serial.available())
  {
    if (Serial.read() == '$')
    {
      if (Serial.read() == 'G')
      {
        if (Serial.read() == 'P')
        {
          if (Serial.read() == 'R')
          {
            if (Serial.read() == 'M')
            {
              if (Serial.read() == 'C')
              {
                if (Serial.read() == ',')
                {
                  char NMEAChar[61];
                  Serial.readBytesUntil('*', NMEAChar, 61);

                  String _string = getLoc(NMEAChar);

                  _GPSInfo.fixStatus = getFixStatus(_string);
                  _string = _string.substring(_string.indexOf(',')+1, 61);

                  _GPSInfo.latitude = getLatitude(_string);
                  _string = _string.substring(_string.indexOf(',')+1, 61);
                  _string = _string.substring(_string.indexOf(',')+1, 61);

                  _GPSInfo.longitude = getLongitude(_string);
                  _string = _string.substring(_string.indexOf(',')+1, 61);
                  _string = _string.substring(_string.indexOf(',')+1, 61);

                  _GPSInfo.craftSpeed = getCraftSpeed(_string);
                  _string = _string.substring(_string.indexOf(',')+1, 61);

                  _GPSInfo.heading = getHeading(_string);

                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

String GPS::getLoc(String str)
{
  return str.substring(str.indexOf(',')+1, 61);
}

bool GPS::getFixStatus(String str)
{
  if (str[0] = 'A')
  {
    return true;
  }
  
  return false;
}

float GPS::getLatitude(String str)
{
  char tmpChar[10];
  str.substring(0, str.indexOf(',')).toCharArray(tmpChar, 10);

  str = str.substring(str.indexOf(',')+1, 61);

  /*if (str[0] = 'N')
  {
    latiNS = true;
  }
  else
  {
    latiNS = false;
  }*/

  return atof(tmpChar);
}

float GPS::getLongitude(String str)
{
  char tmpChar[11];
  str.substring(0, str.indexOf(',')).toCharArray(tmpChar, 11);

  str = str.substring(str.indexOf(',')+1, 61);

  /*if (str[0] = 'W')
  {
    longiWE = true;
  }
  else
  {
    longiWE = false;
  }*/

  return atof(tmpChar);
}

float GPS::getCraftSpeed(String str)
{
  char tmpChar[5];
  str.substring(0, str.indexOf(',')).toCharArray(tmpChar, 5);

  return atof(tmpChar)*0.514;
}

int GPS::getHeading(String str)
{
  return str.substring(0, str.indexOf(',')).toInt();
}
