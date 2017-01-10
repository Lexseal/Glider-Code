#ifndef MPU6050_h
#define MPU6050_h //Arduino library format.

#include "Arduino.h"
#include <Wire.h> //Wire I^C library.

class MPU6050 //Nominate a Class.
{
  public: //Public variables and method.
    void initialization(); //Initialize the senor.
    void getData(); //Fetch the calculated(Useable) angular data.

    float gyroX, gyroY, gyroZ; //Calculated gyro data.
    float gyroXR, gyroYR, gyroZR; //Raw data from the gyro sensor. The "R" stands for raw(gyro + R).
    
  private: 
    #define WHO_AM_I 0x68 
    #define PWR_MGMT_1 0x6B
    #define SMPLRT_DIV 0x19
    #define LOW_PASS_FILTER 0x1A
    #define GYRO_CONFIG 0x1B
    #define ACCEL_CONFIG 0x1C //MPU registers.

    int accXR, accYR, accZR; //Acceleration Raw data.

    float accX, accY; //Acceleration calculated angular data.

    float xGyroOffset, yGyroOffset, zGyroOffset; //Gyro  drift offset.

    int alastX1, alastX2;
    int alastY1, alastY2;
    int alastZ1, alastZ2; //Weighted average variables.
    
    float pX, pY; //Prediction varaibles. Store the angular data predicted using last time's data.
    
    float lastTime; 
  
    void testGyro(); //Method to test the gyro drift.
    
    void readGyro(); //Read gyro data.
    void readAcce(); //Read acc data.
    void calculation(); //Do the calculation and get the final data.
};

#endif //Arduino library format.
