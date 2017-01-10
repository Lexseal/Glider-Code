#include "IMU.h"

void IMU::init()
{
  pinMode(13, OUTPUT);

  MPU.initialization();
  
  Wire.begin();

  delay(5);

  Wire.beginTransmission(mpu_6050);
  Wire.write(0x37);
  Wire.write(0x02);
  Wire.endTransmission();

  delay(5);

  Wire.beginTransmission(hmc5883l);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission();
  
  delay(5);

  Wire.beginTransmission(hmc5883l);
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();

  /*calibration();
  Serial.print(avgX); Serial.print(" ");
  Serial.print(avgY); Serial.print(" ");
  Serial.print(scale); Serial.println(" ");*/
}

void IMU::calibration()
{
  int maxX = -2000, minX = 2000, maxY = -2000, minY = 2000;
  int beginTime = millis();

  while ((millis()-beginTime) < duration)
  //while(1)
  {
    readData();

    tiltCompensation();

    Serial.print("Calibrating, ");
    Serial.print((duration-(millis()-beginTime))/1000);
    Serial.print(" Seconds left. ");

    if (minX > x)
    {
      minX = x;
      digitalWrite(13, HIGH);
    }
    if (maxX < x)
    {
      maxX = x;
      digitalWrite(13, HIGH);
    }
    if (minY > y)
    {
      minY = y;
      digitalWrite(13, HIGH);
    }
    if (maxY < y)
    {
      maxY = y;
      digitalWrite(13, HIGH);
    }

    digitalWrite(13, LOW);

    Serial.print(minX);
    Serial.print(" ");
    Serial.print(maxX);
    Serial.print(" ");
    Serial.print(minY);
    Serial.print(" ");
    Serial.println(maxY);
  }

  avgX = (maxX+minX)/2, avgY = (maxY+minY)/2;
  scale = (float)(maxY-minY)/(float)(maxX-minX);
  Serial.println(scale);

  if (abs(scale-1) > 0.08)
  {
    calibration();
  }
}

void IMU::tiltCompensation()
{
  MPU.getData();

  float xGr = MPU.gyroX/180*3.14;
  float yGr = MPU.gyroY/180*3.14;
  
  y = (y+sin(xGr)*650)*cos(xGr);
  x = (x+sin(yGr)*650)*cos(yGr);

  //x = x*cos(yGr)+z*sin(yGr);
  //y = x*sin(yGr)*sin(xGr)+y*cos(xGr)-z*sin(xGr)*cos(yGr);
}
  	
void IMU::antiDistortion()
{
  x = x-avgX;
  y = y-avgY;

  x = (float)x*scale;
}

void IMU::readData()
{
	Wire.beginTransmission(hmc5883l);
  Wire.write(0x03);
  Wire.endTransmission();

 	Wire.requestFrom(hmc5883l, 6);
  if (Wire.available())
  {
    z = Wire.read()<<8|Wire.read();
    x = Wire.read()<<8|Wire.read();
    y = Wire.read()<<8|Wire.read();
  }
}

struct IMUData IMU::updateIMU()
{
  readData();
  antiDistortion();
  tiltCompensation();

  MPU.gyroZ = (int)(MPU.gyroZ*0.6 + (atan2((float)x, (float)y)/3.14*180)*0.4);
  
  attiData.xAxis = MPU.gyroX;
  attiData.yAxis = MPU.gyroY;
  attiData.orientation = MPU.gyroZ;

  attiData.gyroRateX = MPU.gyroXR;
  attiData.gyroRateY = MPU.gyroYR;
  attiData.gyroRateZ = MPU.gyroZR;

  /*Serial.print("  X = "); Serial.print(x);
  Serial.print("  Y = "); Serial.print(y);
  Serial.print("  Z = "); Serial.print(z);*/

  return attiData;
}
