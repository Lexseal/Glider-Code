#ifndef GPS_h
#define GPS_h

#include "Arduino.h"
#include <SoftwareSerial.h>

struct GPSInfo
{
  float latitude;
  float longitude;
  int altitude;
  float craftSpeed;
  bool fixStatus;
  int heading;
  int temp;
};

class GPS
{
  public: 
    /*GPS(SoftwareSerial& softSerial) : serialGPS(softSerial) 
    {
      serialGPS.begin(38400);
    }*/
    
    bool getGPSInfo();

    struct GPSInfo _GPSInfo;
    
  private:
    String getLoc(String str);
    bool getFixStatus(String str);
    float getLatitude(String str);
    float getLongitude(String str);
    float getCraftSpeed(String str);
    int getHeading(String str);
    //SoftwareSerial serialGPS;
};

#endif
