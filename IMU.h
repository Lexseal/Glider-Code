#ifndef IMU_h
#define IMU_h

#include "MPU6050.h"

#define mpu_6050 0x68
#define hmc5883l 0x1E

struct IMUData
{
	int xAxis, yAxis, orientation;
	float gyroRateX, gyroRateY, gyroRateZ;
};

class IMU
{
  public:
  	void init();
    struct IMUData updateIMU();

  	struct IMUData attiData;

  private:
  	void calibration();
  	void antiDistortion();
  	void readData();
  	void tiltCompensation();

  	int x = 0, y = 0, z = 0;
    int duration = 15000;
	  int avgX = -896, avgY = -304;
	  float scale = 0.99;

    MPU6050 MPU;
};

#endif
