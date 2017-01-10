#include "IMU.h"
#include "outPut.h"
#include "GPS.h"
#include "Barometer.h"

struct pidData _pidData;

const int rx = A2;
const int tx = 4;

SoftwareSerial serialGPS(rx, tx);

IMU attiSensor;
outPut servoOutPut;
//baroMeter auxAltTemp;
//GPS _GPS(serialGPS);

struct IMUData attiData;

int desiredX = -10, desiredY = 0, desiredZ = 90;
float desiredXCordinate = 12137.66210, desiredYCordinate = 3136.16162;

float gpsTime;

//bool released = false;

int n = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(rx, INPUT);
  pinMode(tx, OUTPUT);
  
  Serial.begin(9600);
  
  attiSensor.init();
  servoOutPut.initServos();
  //auxAltTemp.calib();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  attiData = attiSensor.updateIMU();

  //_GPS._GPSInfo.altitude = auxAltTemp.getAlt();
  //_GPS._GPSInfo.temp = auxAltTemp.getTemp();

  /*Serial.print(" ");
  Serial.println(auxAltTemp.getTemp());*/

  /*if (millis()-960 > gpsTime)
  {
    if (_GPS.getGPSInfo())
    {
      gpsTime = millis();

      desiredZ = atan2(-(desiredXCordinate-_GPS._GPSInfo.longitude), (desiredYCordinate-_GPS._GPSInfo.latitude))/3.14*180;
    }
    //gpsTime = millis();

    //desiredZ = atan2(-(desiredXCordinate-12136), (desiredYCordinate-3108))/3.14*180;
  }*/

  //Serial.print("  Latitude: "); Serial.print(_GPS._GPSInfo.latitude, 5);
  //Serial.print("  Longitude: "); Serial.print(_GPS._GPSInfo.longitude, 5);
  //Serial.print("  Speed: "); Serial.print(_GPS._GPSInfo.craftSpeed, 3);

  Serial.print("  X = "); Serial.print(attiData.xAxis);
  Serial.print("  Y = "); Serial.print(attiData.yAxis);
  Serial.print("  Orientation: "); Serial.print(attiData.orientation);
  Serial.print("  desiredOri: "); Serial.println(desiredZ);
  
  _pidData.errorX = attiData.xAxis-desiredX;
  _pidData.errorY = attiData.yAxis-desiredY;
  _pidData.errorZ = attiData.orientation-desiredZ;

  _pidData.rateX = attiData.gyroRateX;
  _pidData.rateY = attiData.gyroRateY;
  _pidData.rateZ = attiData.gyroRateZ;
  
  servoOutPut.updatePID(_pidData); 

  //n++; Serial.println(n);
}
