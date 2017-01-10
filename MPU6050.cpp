#include "MPU6050.h" //Include the .h

void MPU6050::initialization() //Method initialization(public).
{
  Wire.begin(); //I^C start.

  Wire.beginTransmission(WHO_AM_I);
  Wire.write(PWR_MGMT_1); 
  Wire.write(0x00); //Activate the sensor.
  Wire.endTransmission();

  delay(5);

  Wire.beginTransmission(WHO_AM_I); //Get access to MPU.
  Wire.write(SMPLRT_DIV); //Get access to register.
  Wire.write(0x07); //Set sample rate to 1kHz(write 7 in Hex to the register).
  Wire.endTransmission(); //End this transmission.

  delay(5); //Delay 5ms to ensure the data is saved because I^2C uses time to set.

  Wire.beginTransmission(WHO_AM_I);
  Wire.write(LOW_PASS_FILTER);
  Wire.write(0x03); //Set Low Pass Filter to 44Hz.
  Wire.endTransmission(); 

  delay(5);

  Wire.beginTransmission(WHO_AM_I);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x00); //Set gyro sensitivity to +-250'/s.
  Wire.endTransmission();

  delay(5);

  Wire.beginTransmission(WHO_AM_I);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x00); //Set acc sensitivity to +-2g/s.
  Wire.endTransmission();
  

  pinMode(13, OUTPUT); //Just the on-board LED.

  digitalWrite(13, HIGH); //Just the on-board LED.
  
  for(int i=1; i<=100; i++) //Fetch data from gyro for 100 times just to warm it up.
  {
    testGyro();
  }

  digitalWrite(13, LOW); //Just the on-board LED.
  delay(100); //Just the on-board LED.
  digitalWrite(13, HIGH); //Just the on-board LED.

  for(int i=1; i<=1000; i++) //Fetch data from gyro for 1000 times to get the average drift rate.
  {
    testGyro(); //Fetch data from gyro.
    
    xGyroOffset = xGyroOffset+gyroXR; 
    yGyroOffset = yGyroOffset+gyroYR; 
    zGyroOffset = zGyroOffset+gyroZR; //Add up the errors.
  }

  xGyroOffset = -xGyroOffset/1000; 
  yGyroOffset = -yGyroOffset/1000;
  zGyroOffset = -zGyroOffset/1000; //Because we added up for 2000 times, divide it by 2000 to get how much it drift each time.

  digitalWrite(13, LOW); //Just the on-board LED.

  Serial.println("Offsets");
  Serial.println(xGyroOffset);
  Serial.println(yGyroOffset);
  Serial.println(zGyroOffset);
}

void MPU6050::testGyro() //Method testGyro(Private).
{
  Wire.beginTransmission(WHO_AM_I); //Access to MPU.
  Wire.write(0x43); //Access to number 0x43 register(gyro X-axis data).
  Wire.endTransmission(false); 

  Wire.requestFrom(WHO_AM_I,6,true); 
  
  gyroZR=Wire.read()<<8|Wire.read();
  gyroYR=Wire.read()<<8|Wire.read();
  gyroXR=-(Wire.read()<<8|Wire.read()); //Just to read the raw data directly from the sensor.
}

void MPU6050::readGyro() //Method readGyro(Private).
{
  Wire.beginTransmission(WHO_AM_I);
  Wire.write(0x43);
  Wire.endTransmission(false);

  Wire.requestFrom(WHO_AM_I,6,true);
  
  gyroZR = Wire.read()<<8|Wire.read();
  gyroZR = gyroZR+zGyroOffset; //Offset the drift. 
  gyroZR = gyroZR/131; //Divided by sensitivity to get the real-world data.
  
  gyroYR = Wire.read()<<8|Wire.read();
  gyroYR = gyroYR+yGyroOffset;
  gyroYR = gyroYR/131;
  
  gyroXR = -(Wire.read()<<8|Wire.read());
  gyroXR = gyroXR+xGyroOffset;
  gyroXR = gyroXR/131;
}

void MPU6050::readAcce() //Method readAcce(Private).
{
  Wire.beginTransmission(WHO_AM_I);
  Wire.write(0x3B);
  Wire.endTransmission(false);

  Wire.requestFrom(WHO_AM_I,6,true); //I^C grammer.
  
  accZR=Wire.read()<<8|Wire.read(); //Read raw data!
  accZR = accZR*0.6 + alastZ1*0.3 + alastZ2*0.1; //Weighted average to cancel the noise.
  alastZ2 = alastZ1;
  alastZ1 = accZR;
     
  accYR=Wire.read()<<8|Wire.read();
  accYR = accYR*0.6 + alastY1*0.3 + alastY2*0.1;
  alastY2 = alastY1;
  alastY1 = accYR;
  
  accXR=Wire.read()<<8|Wire.read();
  accXR = accXR*0.6 + alastX1*0.3 + alastX2*0.1;
  alastX2 = alastX1;
  alastX1 = accXR;
}

void MPU6050::calculation() //Process the raw data from the sensor.
{
  float timeSpan = millis()-lastTime; //Get the time interval between last calculation.
  lastTime = millis(); //Store current time for next use.
  timeSpan = timeSpan/1000; //Becuase the unit is microsecond, divide by 1 million.

  gyroX = gyroX+gyroXR*timeSpan; 
  gyroY = gyroY+gyroYR*timeSpan;
  gyroZ = gyroZ+gyroZR*timeSpan; //Add up the data to get the real time gyro data. 

  if(abs(accXR)+abs(accYR)>=19000 || abs(accXR)+abs(accZR)>=19000 || abs(accYR)+abs(accZR)>=19000 || abs(accXR)>=19000 ||  abs(accYR)>=19000 || abs(accZR)>=19000 || abs(accZR)<=1638)
  {
    //Strip the acc data when the value is rediculous.
  }
  else
  {
    accX = (atan2((float)accYR, (float)accZR))/3.14*180;
    accY = -(atan2((float)accXR, (float)accZR))/3.14*180; //Use acc data to calculate the angle.

    accX = (accX+pX)/2;
    accY = (accY+pY)/2; //Average the prediction data and the data measured.

    pX = accX+gyroXR*timeSpan;
    pY = accY+gyroYR*timeSpan; //Update the prediction.
    
    gyroX = gyroX*0.9+accX*0.1;
    gyroY = gyroY*0.9+accY*0.1; //Fuse the gyro+acc data to get the stable data.
  }
}

void MPU6050::getData() //A method to invoke other method.
{
  readGyro();
  
  readAcce();
  
  calculation();
}
